<?php
include 'conf.inc';
include_once ROOT . '/inc/lib/BaseCtrl.inc';
session_start();
$c = 'user';
if (array_key_exists('c', $_GET)) {
	$c = $_GET['c'];
}
$a = 'index';
if (array_key_exists('a', $_GET)) {
	$a = $_GET['a'];
}
$class = strtoupper(substr($c, 0, 1)) . substr($c, 1) . 'App';
$filename = ROOT . '/inc/app/' . $class . '.inc';
if (!file_exists($filename)) {
	$class = 'UserApp';
	$filename = ROOT . '/inc/app/' . $class . '.inc';
}
include_once $filename;
if (class_exists($class)) {
	$obj = new $class;
	$obj->run($a);
} else {
	echo 'ctrl is null!';
}

function debug($str){
    echo "<pre>";
    print_r($str);
    exit;
}
?>

