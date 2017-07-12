<?php

require_once ROOT . '/inc/dat/UserDB.inc';
require_once ROOT . '/inc/dat/CourseDB.inc';
require_once ROOT . '/inc/lib/tools.inc';
include_once ROOT . '/inc/lib/BaseCtrl.inc';
include_once ROOT . '/inc/biz/Config.inc';

class UserSetting {

    public $data = array();

    function __construct($data = array()) {
        $this->data = $data;
    }
}
