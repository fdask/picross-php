<?php
// the game of picross
class Picross {
    public $size;    
    public $coords;
    public $playerX;
    public $playerY;
    public $misses;
    public $maxMisses;
    public $viewActual;
    public $term;
    
    public function __construct() {
        // reset game
        $this->playerX = 0;
        $this->playerY = 0;
        $this->misses = 0;
        $this->maxMisses = rand(0, Config::$maxMisses);
        $this->viewActual = false;

        // set the size
        $this->size = rand(Config::$sizeMin, Config::$sizeMax);

        // populate the coords with squares
        for ($x = 0; $x < $this->size; $x++) {
            if (!isset($this->coords[$x])) {
                $this->coords[$x] = array();
            }

            for ($y = 0; $y < $this->size; $y++) {
                $this->coords[$x][$y] = new Square(rand(0, 1));
            }
        }

        // save the term
        $this->term = `stty -g`;
        system("stty -icanon -echo");
        
        // initial gameboard print
        echo "\n" . $this . "\n";

        // enter an endless loop waiting for keypress
        while (1) {
            $c = Config::waitForInput();

            $draw = false;

            switch ($c) {
                case ' ':
                    $this->coords[$this->playerX][$this->playerY]->suspect();

                    $draw = true;

                    break;
                case Config::enter():
                    $draw = true;

                    if (!$this->coords[$this->playerX][$this->playerY]->guess()) {
                        $this->misses++;

                        if ($this->misses >= $this->maxMisses) {
                            echo "Too many misses!\n";

                            $draw = false;

                            $this->quit();
                        }
                    } 

                    $this->checkStatus();
                    
                    break;
                case Config::upArrow():
                    if ($this->playerX > 0) {
                        $this->playerX--;

                        $draw = true;
                    }

                    break;
                case Config::downArrow():
                    if ($this->playerX < $this->size - 1) {
                        $this->playerX++;

                        $draw = true;
                    }

                    break;
                case Config::leftArrow():
                    if ($this->playerY > 0) {
                        $this->playerY--;

                        $draw = true;
                    }

                    break;
                case Config::rightArrow():
                    if ($this->playerY < $this->size - 1) {
                        $this->playerY++;

                        $draw = true;
                    }

                    break;
                case 'x':
                    if (Config::$debug) {
                        var_dump($this->coords[$this->playerX][$this->playerY]);
                    } else {
                        $this->quit();
                    }

                    break;
                case 'q':
                    $this->quit();

                    break;
                case 'v':
                    if (Config::$debug) {
                        $this->viewActual = $this->viewActual ? false : true;

                        $draw = true;
                    }

                    break;
                default:
                    if (Config::$debug) {
                        echo "Unhandled keypress\n" . ord($c) . "\n";
                    }
            }

            if ($draw) {
                echo $this . "\n";
            }
        }
    }
    
    public function checkStatus() {
        // has the user revealed all the occupied squares?
        for ($x = 0; $x < $this->size; $x++) {
            for ($y = 0; $y < $this->size; $y++) {
                $s = $this->coords[$x][$y];

                if ($s->isActual() && !$s->isRevealed()) {
                    return false;
                }
            }
        }

        echo "You win!\n";

        $this->quit();
    }

    public function quit() {
        system("stty " . $this->term);

        exit;
    }

    public function compareRowAndActual($row, $actual, $colors = false) {
        $started = false;
        $xs = array();
        $matched = true;
        $ret = array();

        for ($x = 0; $x < count($actual); $x++) {
            if ($actual[$x] == "X") {
                $started = true;
                $xs[] = $x;

                if ($row[$x] !== "X") {
                    $matched = false;
                }
            } else {
                $started = false;

                // check to see if we have a match
                if (count($xs) > 0) {
                    if ($matched && $colors) {
                        $ret[] = Colors::fg_color('green', count($xs));
                    } else {
                        $ret[] = count($xs);
                    }

                    $xs = array();
                    $matched = true;
                }
            }
        }

        if ($started && count($xs) > 0) {
            if ($matched && $colors) {
                $ret[] = Colors::fg_color('green', count($xs));
            } else {
                $ret[] = count($xs);
            }
        }

        return $ret;
    }

    public function getRowSegs($rowNum) {
        $seq = array();

        for ($x = 0; $x < $this->size; $x++) {
            for ($y = 0; $y < $this->size; $y++) {
                if ($x == $rowNum) {
                    $seq[] = ($this->coords[$x][$y]->isActual() && $this->coords[$x][$y]->isRevealed()) ? "X" : " ";
                }
            }
        }

        return $seq;
    }

    public function getActualRowSegs($rowNum) {
        $seq = array();

        for ($x = 0; $x < $this->size; $x++) {
            for ($y = 0; $y < $this->size; $y++) {
                if ($x == $rowNum) {
                    $seq[] = $this->coords[$x][$y]->isActual() ? "X" : " ";
                }
            }
        }

        return $seq;
    }

    public function getColSegs($colNum) {
        $seq = array();

        for ($x = 0; $x < $this->size; $x++) {
            for ($y = 0; $y < $this->size; $y++) {
                if ($y == $colNum) {
                    $seq[] = ($this->coords[$x][$y]->isActual() && $this->coords[$x][$y]->isRevealed()) ? "X" : " ";
                }
            }
        }

        return $seq;
    }

    public function getActualColSegs($colNum) {
        $seq = array();

        for ($x = 0; $x < $this->size; $x++) {
            for ($y = 0; $y < $this->size; $y++) {
                if ($y == $colNum) {
                    $seq[] = $this->coords[$x][$y]->isActual() ? "X" : " ";
                }
            }
        }

        return $seq;
    }

    public function header($buffer) {
        $ret = "";

        // need to get all the column segments
        $segs = array();

        $highCount = 0;

        for ($x = 0; $x < $this->size; $x++) {
            $selected = $this->getColSegs($x);
            $actual = $this->getActualColSegs($x);
            
            $cnums = $this->compareRowAndActual($selected, $actual);

            if (count($cnums) > $highCount) {
                $highCount = count($cnums);
            }

            $segs[] = $cnums;
        }

        for ($row = $highCount; $row > 0; $row--) {
            $ret .= $buffer;
            for ($x = 0; $x < $this->size; $x++) {
                $ret .= " ";

                if (count($segs[$x]) == $row) {
                    $ret .= array_shift($segs[$x]);
                } else {
                    $ret .= " ";
                }

                $ret .= " ";
            }

            $ret .= "\n";
        }

        return $ret;
    }

    public function __toString() {
        $ret = "";

        // prepare the leftwards data
        $highCount = 0;
        $leftLines = array();
        $colorLeftLines = array();
        
        for ($x = 0; $x < $this->size; $x++) {
            $selected = $this->getRowSegs($x);
            $actual = $this->getActualRowSegs($x);
            
            $colorLeftLines[] = implode(" ", $this->compareRowAndActual($selected, $actual, true));
            $leftLines[] = implode(" ", $this->compareRowAndActual($selected, $actual));
        }

        $padLength = 0;

        for ($x = 0; $x < count($leftLines); $x++) {
            if (strlen($leftLines[$x]) > $padLength) {
                $padLength = strlen($leftLines[$x]);
            }
        }

        $buffer = " ";

        for ($b = 0; $b < $padLength; $b++) {
            $buffer = $buffer . " ";
        }

        // add the header
        $ret .= $this->header($buffer);

        // add the main block
        for ($x = 0; $x < count($this->coords); $x++) {
            $left = "";

            for ($spaceCount = 0; (strlen($leftLines[$x]) + $spaceCount) < $padLength; $spaceCount++) {
                $colorLeftLines[$x] = " " . $colorLeftLines[$x];
            }
            
            $ret .= $colorLeftLines[$x] . " ";

            for ($y = 0; $y < count($this->coords[0]); $y++) {
                if ($x == $this->playerX && $y == $this->playerY) {
                    $ret .= Colors::bg_color('light_gray', '[');
                } else {
                    $ret .= "[";
                }

                if ($this->viewActual) {
                    if ($this->coords[$x][$y]->isActual()) {
                        $ret .= Colors::fg_color('bold_green', 'O');
                    } else {
                        $ret .= " ";
                    }
                } else {
                    $ret .= $this->coords[$x][$y];
                }

                if ($x == $this->playerX && $y == $this->playerY) {
                    $ret .= Colors::bg_color('light_gray', ']');
                } else {
                    $ret .= "]";
                }
            }

            $ret .= "\n";
        }

        $ret .= "misses: " . $this->misses . " / " . $this->maxMisses . "\n";

        return $ret;
    }
}

