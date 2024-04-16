#!/usr/bin/php
<?php
include 'include/config.inc';

$filename = null;

if (isset($argv[1])) {
    $filename = $argv[1];
}

$p = new Picross($filename);
