<?php

include_once ROOT . '/inc/lib/Error.inc';
include_once ROOT . '/inc/biz/Config.inc';

class EmptyChecker {

    public static function check_empty($params) {
        if (empty($params)) {
            Error::init()->set_detail(1, 2, "all");
            return Error::init()->get_response();
        }
    }

    public static function check_AllParams($params) {
        foreach ($params as $value) {
            if (empty($value)) {
                Error::init()->set_detail(1, 2, "all");
                return Error::init()->get_response();
            }
        }
    }

    public static function check_phone($phone) {
        self::check_empty($phone);
//        if (!preg_match('/1[3|5|7|8|][0-9]{9}/', $phone)) {
//            Error::init()->set_detail(1, 5, "all");
//        }
        
        if (!preg_match('/^[1][358]\d{9}$/', $phone)) {
            Error::init()->set_detail(1, 5, "all");
        }
        return Error::init()->get_response();
    }
   
    public static function check_qq($qq){
        self::check_empty($qq);
        if (!preg_match('/^\d{5,14}$/', $qq)) {
            Error::init()->set_detail(1, 77, "QQ错误");
        }
        return Error::init()->get_response();
    }
    
    public static function check_email($email){
        if (!preg_match('/^(\w)+(\.\w+)*@(\w)+((\.\w{2,3}){1,3})$/', $email)) {
            Error::init()->set_detail(1, 25, "邮箱错误");
        }
        return Error::init()->get_response();
    }
    
    public static function check_code($params, $phonecode) {
        $params['create_time'] = time();
        if ($phonecode[0]['code'] == $params['code'] && (($params['create_time']) - $phonecode[0]['create_time'] < 300)) {
            $user = MobileUserSetting::addUser($params);
            if (!empty($user)) {
                MobileUserSetting::delCodePhone($params['phone']);
                return $user;
            }
        } else {
            Error::init()->set_detail(1, 3, 'db');
            return Error::init()->get_response();
        }
    }

    public static function check_pass($params) {
        self::check_empty($params);
        $params['u_pass'] = MobileUserSetting::md5PassWord($params['u_pass']);
        return MobileUserSetting::getUserByPhoneAndPass($params);
    }

    public static function check_QuestionImg() {
        $input = file_get_contents("php://input");
        if (empty($input)) {
            Error::init()->set_detail(1, 10, 'ALL');
        }
        return Error::init()->get_response();
    }
    
    public static function checkRequestImg() {
        $input = file_get_contents("php://input");
        if (empty($input)) {
            return $input = '';
        }  else {
            return $input;
        }
        return Error::init()->get_response();
    }
    

    public static function check_UpdateQuestion($params) {
        if (self::check_empty($params['questionId'])) {
            Error::init()->set_detail(1, 11, '问题ID为空');
            return Error::init()->get_response();
        }
        $question = QuestionSetting::getQuestionByQuestionIdAndStatus($params['questionId'], 1);
        if (!empty($question)) {
            return $question;
        } else {
            Error::init()->set_detail(1, 12, '问题ID无效');
            return Error::init()->get_response();
        }
    }



    public static function check_QuestionEd($params) {
        EmptyChecker::check_empty($params['questioned_id']);
        EmptyChecker::check_empty($params['user_id']);
        $questioned = QuestionSetting::getQuestionedByUserIdAndQuestionId($params['user_id'], $params['questioned_id']);
        if (empty($questioned)) {
            Error::init()->set_detail(1, 1, 'ALL');
        } else {
            return $questioned;
        }
        return Error::init()->get_response();
    }

}

$n = array(
    'id' => 9,
    'products' => '1,3,7',
);
