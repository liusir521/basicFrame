<?php
require_once("alipay.config.php");
require_once("lib/alipay_notify.class.php");
require_once("../conf.inc");
require_once '../inc/dat/CourseDB.inc';
//计算得出通知验证结果
$alipayNotify = new AlipayNotify($alipay_config);
$verify_result = $alipayNotify->verifyNotify();
if ($verify_result) {//验证成功
    $z = explode("&", $_POST['body']);
    $data = array();
    foreach ($z as &$v) {
        $vv = explode("=", $v);
        $data[$vv[0]] = $vv[1];
    }
    $out_trade_no = $_POST['out_trade_no']; //商户订单号
    $trade_no = $_POST['trade_no']; //支付宝交易号
    $trade_status = $_POST['trade_status']; //交易状态
    if ($_POST['trade_status'] == 'TRADE_FINISHED') {
        echo "success";
    } else if ($_POST['trade_status'] == 'TRADE_SUCCESS') {
        $db_array = Conf::$YuYingDB;
        $link = mysql_connect($db_array['server'], $db_array['username'], $db_array['password']);
        mysql_select_db($db_array['database']);
        mysql_query("set names utf8");
        mysql_query("BEGIN"); //开始一个事务
        mysql_query("SET AUTOCOMMIT=0"); //设置事务不自动commit
        //更改订单状态
        $paid_time = time();
        mysql_query("update `orders` set  `payment`='alipay',`paid_time`='{$paid_time}',`status`='paid' where `sn`='{$out_trade_no}'");
        $n1 = mysql_affected_rows();
        //写资金日志
        $create_time = time();
        mysql_query("INSERT INTO `order_log`(`order_id`,`type`,`message`,`student_id`,`manage_id`,`action`,`create_time`) VALUES ('{$data['order_id']}', '1', '购买课程', '{$data['student_id']}','0', '-{$_POST['total_fee']}','{$create_time}')");
        $n2 = mysql_affected_rows();
        if ($n1 > 0 && $n2 > 0) {
            mysql_query("COMMIT");
        } else {
            mysql_query("ROLLBACK"); //非autocommit模式，执行ROLLBACK使事务操作无效
            echo "fail";
        }
        mysql_query("END");
        mysql_query("SET AUTOCOMMIT=1"); //恢复autocommit模式
        //操作另外一个数据库
       // 获取该订单下所有课程,向student_course表中写数据
        if ($n1 > 0 && $n2 > 0) {
            $f_data = array();
            $s_sql = "";
            $sql = "SELECT * FROM `order_course` WHERE `sn`=" . $out_trade_no;
            $result = mysql_query($sql);
            while ($row = mysql_fetch_assoc($result)){
               $f_data[] = $row;
            }
            foreach($f_data as $v){
                $s_sql .= "(".$data['student_id'].",".$v['course_tem_id'].",".$v['course_child_id'].",".$v['create_time'].",".$v['keshi'].",".$v['keshi'].")".",";
            }
            $s_sql = trim($s_sql,",");
            $j_db_array = Conf::$JiaoyanDB;
            mysql_select_db($j_db_array['database']);
            $final_sql = "INSERT INTO `student_course`(`student_id`,`course_tem_id`,`course_id`,`create_time`,`original_lesson_num`,`existing_lesson_num`) VALUES ".$s_sql."";
            mysql_query($final_sql);
            //消耗优惠券
            if($data['coupon_id'] > 0){
                $yy_db_array = Conf::$YuYingDB;
                mysql_select_db($yy_db_array['database']);
                mysql_query("INSERT INTO `promo_coupon_record` VALUES ('', '".time()."', '购买课程', '".$data['coupon_id']."', '".$data['student_id']."','".$out_trade_no."');");
            }
            
        }
        echo "success";
    }
} else {
    echo "fail";
}
?>