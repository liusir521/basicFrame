<?php

/**
 * Description of Config
 * @time 2014/10/11
 * @author songshuhong
 */
require_once ROOT . '/conf.inc';

class Config {

    public static $img_path = "http://xuexi8.img-cn-beijing.aliyuncs.com/";
    public static $url = "http://xuexi8.net";
    public static $password = "dszx";
    public static $arrResponseMsg = array();
    public static $user_type = array();
    public static $pagesize = 10;
    public static $paymoney = 10;
    public static $permition = array(); //不同角色顶部菜单权限
    public static $menu_array = array(); //菜单数组
    public static $season = array(); //菜单数组
    public static $nianji = array(); //年级 科目划分 1
    public static $nianji2 = array(); //年级 科目划分 2
    public static $nianji3 = array(); //年级 科目划分 3
    public static $question_type = array(); //问题类型
    public static $refund_reason = array(); //退款原因
    public static $promo_type = array(); //促销类型
    public static $subject=array();//学科类型

}

Config::$promo_type = array(
    1 => '直减',
    2 => '打折',
);


Config::$refund_reason = array(
    1 => '订单信息有误',
    2 => '没有时间学习',
    3 => '去校区不方便',
    4 => '心情不好，不想买了',
);

Config::$arrResponseMsg = array(
    1 => '参数非法',
    2 => '参数为空',
    3 => '验证码无效',
    4 => '手机号已存在',
    5 => '手机号无效',
    6 => '用户名或密码错误！',
    7 => '账号被冻结，请联系管理员！',
    8 => '注册异常！',
);
Config::$user_type = array(1 => '管理员', 2 => '用户',);

Config::$question_type = array(
    'single_choice' => '单选题',
    'choice' => '多选题',
    'fill' => '填空题',
    'determine' => '判断题',
    'essay' => '问答题',
    'material' => '材料题',
);

Config::$subject=  array("语文","数学","英语","物理","化学",);

Config::$permition = array(
    1 => array(1, 2, 4),
    2 => array(5),
    3 => array(3, 4),
);

Config::$nianji = array(
    1 => '2,4,5,7,8,10,11', //小学
    2 => '13,14,16,17,18,20,21,22,23', //初中
    3 => '30,31', //高中
);

Config::$nianji2 = array(
    1 => array(2), //幼儿
    3 => array(4, 5), //四年级
    6 => array(7, 8), //五年级
    9 => array(10, 11), //六年级
    12 => array(13, 14), //七年级
    15 => array(16, 17, 18), //八年级
    19 => array(20, 21, 22, 23), //九年级
    24 => array(), //高一
    25 => array(), //高二
    26 => array(), //高三
);

Config::$nianji3 = array(1, 3, 6, 9, 12, 15, 19, 24, 25, 26);


Config::$menu_array = array(
    1 =>
    array(
        'id' => '1',
        'pid' => 0,
        'cname' => '年级科目',
        'URL' => 'index.php?c=Jiaoyan&a=kemu',
        'icon' => 'glyphicon-signal',
    ),
    2 =>
    array(
        'id' => '2',
        'pid' => 0,
        'cname' => '教研组',
        'URL' => 'index.php?c=Jiaoyan&a=group',
        'icon' => 'glyphicon-user',
    ),
    3 =>
    array(
        'id' => '3',
        'pid' => 0,
        'cname' => '授课',
        'URL' => 'index.php?c=ShouKe&a=getClasss',
        'icon' => 'glyphicon-book',
    ),
    4 =>
    array(
        'id' => '4',
        'pid' => 0,
        'cname' => '题库',
        'URL' => 'index.php?c=TiKu&a=getCourses',
        'icon' => 'glyphicon glyphicon-pencil',
    ),
);

