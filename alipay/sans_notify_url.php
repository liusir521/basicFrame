<?php 

/* *
 * 功能：支付宝服务器异步通知页面
 * 版本：3.3
 * 日期：2012-07-23
 * 说明：
 * 以下代码只是为了方便商户测试而提供的样例代码，商户可以根据自己网站的需要，按照技术文档编写,并非一定要使用该代码。
 * 该代码仅供学习和研究支付宝接口使用，只是提供一个参考。


 * ************************页面功能说明*************************
 * 创建该页面文件时，请留心该页面文件中无任何HTML代码及空格。
 * 该页面不能在本机电脑测试，请到服务器上做测试。请确保外部可以访问该页面。
 * 该页面调试工具请使用写文本函数logResult，该函数已被默认关闭，见alipay_notify_class.php中的函数verifyNotify
 * 如果没有收到该页面返回的 success 信息，支付宝会在24小时内按一定的时间策略重发通知
 */

require_once("alipay.config.php");
require_once("lib/alipay_notify.class.php");
require_once("../conf.inc");
//计算得出通知验证结果
$alipayNotify = new AlipayNotify($alipay_config);
$verify_result = $alipayNotify->verifyNotify();
//debug($verify_result);
if ($verify_result) {//验证成功
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //请在这里加上商户的业务逻辑程序代
    //——请根据您的业务逻辑来编写程序（以下代码仅作参考）——
    //获取支付宝的通知返回参数，可参考技术文档中服务器异步通知参数列表
    $z = explode("&", $_POST['body']);

    $data = array();
    foreach ($z as &$v) {
        $vv = explode("=", $v);
        $data[$vv[0]] = $vv[1];
    }



    //商户订单号
     $out_trade_no = $_POST['out_trade_no'];

    //支付宝交易号
    $trade_no = $_POST['trade_no'];

    //交易状态
    $trade_status = $_POST['trade_status'];
    if ($_POST['trade_status'] == 'TRADE_FINISHED') {
        //判断该笔订单是否在商户网站中已经做过处理
        //如果没有做过处理，根据订单号（out_trade_no）在商户网站的订单系统中查到该笔订单的详细，并执行商户的业务程序
        //请务必判断请求时的total_fee、seller_id与通知时获取的total_fee、seller_id为一致的
        //如果有做过处理，不执行商户的业务程序
        //注意：
        //退款日期超过可退款期限后（如三个月可退款），支付宝系统发送该交易状态通知
        //调试用，写文本函数记录程序运行情况是否正常
        //logResult("这里写入想要调试的代码变量值，或其他运行的结果记录");
    } else if ($_POST['trade_status'] == 'TRADE_SUCCESS') {
        $db_array = Conf::$YuYingDB;
        $link = mysql_connect($db_array['server'], $db_array['username'], $db_array['password']);
        mysql_select_db($db_array['database']);
        mysql_query("set names utf8");
        mysql_query("BEGIN"); //开始一个事务
        mysql_query("SET AUTOCOMMIT=0"); //设置事务不自动commit
//更改订单状态
        mysql_query("update `order_wyy_dz` set  `payment`='alipay',`paid_time`='" . time() . "',`status`='paid' where sn=" . $out_trade_no . ";");
        $n1 = mysql_affected_rows();


//写资金日志
        mysql_query("INSERT INTO `order_log_sans` VALUES ('', '" . $data['order_id'] . "', '1', '购买课程', '" . $data['student_id'] . "', '', '0', '-" . $_POST['total_fee'] . "','" . time() . "','dz');");
        $n2 = mysql_affected_rows();


//向student_course_sans表中写数据
        $j_db_array = Conf::$JiaoyanDB;
        mysql_select_db($j_db_array['database']);
        mysql_query("INSERT INTO `student_course_sans` VALUES ('', '" . $data['student_id'] . "', '" . $data['course_id'] . "', 'dz', '" . time() . "');");
        $n3 = mysql_affected_rows();
//
//
//
        if ($n1 > 0 && $n2 > 0 && $n3 > 0) {
            mysql_query("COMMIT");
            echo "success";
        } else {
            mysql_query("ROLLBACK"); //非autocommit模式，执行ROLLBACK使事务操作无效
        }
        mysql_query("END");
        mysql_query("SET AUTOCOMMIT=1"); //恢复autocommit模式
        //判断该笔订单是否在商户网站中已经做过处理
        //如果没有做过处理，根据订单号（out_trade_no）在商户网站的订单系统中查到该笔订单的详细，并执行商户的业务程序
        //请务必判断请求时的total_fee、seller_id与通知时获取的total_fee、seller_id为一致的
        //如果有做过处理，不执行商户的业务程序
        //注意：
        //付款完成后，支付宝系统发送该交易状态通知
        //调试用，写文本函数记录程序运行情况是否正常
    }

    //——请根据您的业务逻辑来编写程序（以上代码仅作参考）——
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
} else {
    //验证失败
    echo "fail";
    //调试用，写文本函数记录程序运行情况是否正常
    //logResult("这里写入想要调试的代码变量值，或其他运行的结果记录");
}

