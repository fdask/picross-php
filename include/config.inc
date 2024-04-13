<?php
class Config {
    public static $debug = false;

    public static $sizeMin = 5;
    public static $sizeMax = 10;
    
    public static $maxMisses = 3;

    public static function enter() {
        return chr(10);
    }

    public static function shiftUpArrow() {
        return chr(27) . chr(91) . chr(49) . chr(59) . chr(50) . chr(65);
    }

    public static function upArrow() {
        return chr(27) . chr(91) . chr(65);
    }

    public static function downArrow() {
        return chr(27) . chr(91) . chr(66);
    }

    public static function shiftDownArrow() {
        return chr(27) . chr(91) . chr(49) . chr(59) . chr(50) . chr(66);
    }

    public static function leftArrow() {
        return chr(27) . chr(91) . chr(68);
    }

    public static function shiftLeftArrow() {
        return chr(27) . chr(91) . chr(49) . chr(59) . chr(50) . chr(68);
    }

    public static function rightArrow() {
        return chr(27) . chr(91) . chr(67);
    }

    public static function shiftRightArrow() {
        return chr(27) . chr(91) . chr(49) . chr(59) . chr(50) . chr(67);
    }

    public static function autoload($class) {
        $file = __DIR__ . "/" . str_replace('\\', '/', strtolower($class)) . ".inc";
        
        if (file_exists($file)) {
            require $file;
        } else {
            if (Config::$debug) {
                debug_print_backtrace();
            }
            
            die("Couldnt find file: $file\n");
        }
    }

    public static function waitForInput() {
		$input = '';

		$read = [STDIN];
		$write = null;
		$except = null;

		readline_callback_handler_install('', function() {});

		// Read characters from the command line one at a time until there aren't any more to read
		do {
			$input .= fgetc(STDIN);
		} while (stream_select($read, $write, $except, 0, 1));

		readline_callback_handler_remove();

		return $input;
	}
}

spl_autoload_register("Config::autoload");