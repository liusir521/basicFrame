<?php

include_once ROOT . '/inc/lib/BaseCtrl.inc';
include_once ROOT . '/inc/lib/tools.inc';
include_once ROOT . '/inc/lib/Request.inc';

class UserApp extends BaseCtrl {

    public $params;
    public $filter;
    public $fina;
    public function __construct() {
        parent::__construct();
        $this->filter = new Request();
        $this->filter->dang_set_params();
        $this->params = $this->filter->get_params();
    }

    function login(){
        $this->display("login");
    }
    function index(){
        $this->display("index");
    }
    
    function getStudent(){
        $this->display("getStudent");
    }
}
    