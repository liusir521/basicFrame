<?php
set_time_limit(0);
if (!defined("ROOT"))
    define("ROOT", dirname(__FILE__));
error_reporting(1);
define('WEBAPI_ONCE', 200);
define('OTHER_IMG_IDC', 3);
ini_set('memory_limit', '1524M');
date_default_timezone_set('Asia/Shanghai');
class Conf {
    public static $appDB = array("database" => "app", "username" => "root", "password" => "root", "server" => "127.0.0.1", "dbtype" => "mysql");
    public static $log = array('log_path' => 'log/');
}
