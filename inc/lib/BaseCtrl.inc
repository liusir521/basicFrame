<?php

class BaseCtrl {

    var $auths = array(
    );
    var $tpldata = array();

    function __construct() {
        $menu = self::menu();
        $this->assign('menu', $menu);
    }

    function __destruct() {
        
    }

    function loadModel($model) {
        $filename = ROOT . '/inc/' . $model . '.inc';
        if (file_exists($filename)) {
            include_once $filename;
        }
    }

    function run($actMethod) {
        if (!method_exists($this, $actMethod)) {
            $this->display('404');
        }
        $ret = @$this->$actMethod();
    }

    function display($tpl) {
        if (strpos($tpl, 'http') !== false) {
            header('location:' . $tpl);
            exit;
        }
        if (is_array($this->tpldata) && count($this->tpldata) > 0) {
            foreach ($this->tpldata as $key => $val) {
                ${$key} = $val;
            }
        }
        include_once ROOT . '/inc/tpl/' . $tpl . '.inc';
    }

    function assign($key, $val) {
        $this->tpldata[$key] = $val;
    }

    function redirect($url) {
        header('location:' . $url);
    }

    function jsredirect($url, $msg = '') {
        $str = '<script>';
        if (trim($msg) != '') {
            $str .= "alert('" . addslashes($msg) . "');";
        }
        $str .= "top.location.href='" . $url . "'";
        $str .= '</script>';
        echo $str;
        exit;
    }

    function debug($var, $flag = false) {
        echo "<pre>";
        print_r($var);
        if (!$flag)
            exit;
    }

    function menu() {
        $use_type = $_SESSION['type'];
        $mymenu = Config::$permition;
        $mymenu = $mymenu[$use_type];
        $all_array = Config::$menu_array;
        $final = array();
        foreach ($all_array as $k => $v) {
            if (in_array($k, $mymenu)) {
                $final[] = $v;
            }
        }
        return $final;
    }

}
