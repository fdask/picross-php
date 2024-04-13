<?php
class Square {
    // we can be in the following states
    private $actual;
    public $revealed;
    public $guessedRight;
    public $suspected;

    public function __construct($actual = false) {
        $this->actual = $actual;
        $this->revealed = false;
        $this->suspected = false;
        $this->guessedRight = false;
    }

    public function isActual() {
        return $this->actual;
    }

    public function setActual($a) {
        $this->actual = a;
    }

    public function isRevealed() {
        return $this->revealed;
    }

    public function getSuspected() {
        return $this->suspected;
    }

    public function suspect() {
        if ($this->suspected == false) {
            $this->suspected = true;
        } else {
            $this->suspected = false;
        }
    }

    public function guess() {
        // clear suspected if set
        if ($this->suspected) {
            $this->suspected = false;

            return true;
        } 
        
        // reveal the square
        if (!$this->revealed) {
            $this->revealed = true;

            if ($this->actual) {
                $this->guessedRight = true;

                return true;
            } else {
                $this->guessedRight = false;

                return false;
            }
        }

        return true;
    }

    public function __toString() {
        $ret = "";
        
        if ($this->revealed) {
            if ($this->guessedRight) {
                $ret .= Colors::fg_color('bold_green', "O");
            } else {
                $ret .= Colors::fg_color('dark_gray', "X");
            }
        } else if ($this->suspected) {
            $ret .= Colors::fg_color('red', "X");
        } else {
            $ret .= " ";
        }

        return $ret;
    }
}