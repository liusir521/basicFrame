<?php

ini_set('date.timezone', 'Asia/Shanghai');
error_reporting(E_ERROR);
require_once "./lib/WxPay.Api.php";
require_once './lib/WxPay.Notify.php';
require_once 'log.php';
require_once("../conf.inc");
//初始化日志
$logHandler = new CLogFileHandler("./logs/" . date('Y-m-d') . '.log');
$log = Log::Init($logHandler, 15);

class PayNotifyCallBack extends WxPayNotify {

    //查询订单
    public function Queryorder($transaction_id) {
        $input = new WxPayOrderQuery();
        $input->SetTransaction_id($transaction_id);
        $result = WxPayApi::orderQuery($input);
        Log::DEBUG("query1:" . json_encode($result));
        if (array_key_exists("return_code", $result) && array_key_exists("result_code", $result) && $result["return_code"] == "SUCCESS" && $result["result_code"] == "SUCCESS") {
            $z = explode("&", $result['attach']);
            $data = array();
            foreach ($z as &$v) {
                $vv = explode("=", $v);
                $data[$vv[0]] = $vv[1];
            }
            $db_array = Conf::$YuYingDB;
            $link = mysql_connect($db_array['server'], $db_array['username'], $db_array['password']);
            mysql_select_db($db_array['database']);
            mysql_query("set names utf8");
            mysql_query("BEGIN"); //开始一个事务
            mysql_query("SET AUTOCOMMIT=0"); //设置事务不自动commit
            //更改订单状态
            $paid_time = time();
            mysql_query("update `orders` set  `payment`='wxpay',`paid_time`='" . time() . "',`status`='paid' where sn=" . $result['out_trade_no'] . ";");
            $n1 = mysql_affected_rows();
            $total_fee = $result['total_fee'] / 100;
            //写资金日志
            $create_time = time();
            mysql_query("INSERT INTO `order_log` VALUES ('', '" . $data['order_id'] . "', '1', '购买课程', '" . $data['student_id'] . "', '', '0', '-" . $total_fee . "'," . time() . ");");
            $n2 = mysql_affected_rows();
            if ($n1 > 0 && $n2 > 0) {
                mysql_query("COMMIT");
            } else {
                mysql_query("ROLLBACK"); //非autocommit模式，执行ROLLBACK使事务操作无效
                return false;
            }
            mysql_query("END");
            mysql_query("SET AUTOCOMMIT=1"); //恢复autocommit模式
            //操作另外一个数据库
            // 获取该订单下所有课程,向student_course表中写数据
            if ($n1 > 0 && $n2 > 0) {
                $f_data = array();
                $s_sql = "";
                $sql = "SELECT * FROM `order_course` WHERE `sn`=" . $result['out_trade_no'];
                $sn = $result['out_trade_no'];
                $result = mysql_query($sql);
                while ($row = mysql_fetch_assoc($result)) {
                    $f_data[] = $row;
                }
                foreach ($f_data as $v) {
                    $s_sql .= "(" . $data['student_id'] . "," . $v['course_tem_id'] . "," . $v['course_child_id'] . "," . $v['create_time'] . "," . $v['keshi'] . "," . $v['keshi'] . ")" . ",";
                }
                $s_sql = trim($s_sql, ",");
                $j_db_array = Conf::$JiaoyanDB;
                mysql_select_db($j_db_array['database']);
                $final_sql = "INSERT INTO `student_course`(`student_id`,`course_tem_id`,`course_id`,`create_time`,`original_lesson_num`,`existing_lesson_num`) VALUES " . $s_sql . "";
                mysql_query($final_sql);
                //消耗优惠券
                if ($data['coupon_id'] > 0) {
                    $yy_db_array = Conf::$YuYingDB;
                    mysql_select_db($yy_db_array['database']);
                    mysql_query("INSERT INTO `promo_coupon_record` VALUES ('', '" . time() . "', '购买课程', '" . $data['coupon_id'] . "', '" . $data['student_id'] . "','" . $sn . "');");
                }
            }
            return true;
        }
    }

    //重写回调处理函数
    public function NotifyProcess($data, &$msg) {
        Log::DEBUG("call back:" . json_encode($data));
        $notfiyOutput = array();

        if (!array_key_exists("transaction_id", $data)) {
            $msg = "输入参数不正确";
            return false;
        }
        //查询订单，判断订单真实性
        if (!$this->Queryorder($data["transaction_id"])) {
            $msg = "订单查询失败";
            return false;
        }
        return true;
    }

}

Log::DEBUG("begin notify");
$notify = new PayNotifyCallBack();
$notify->Handle(false);
