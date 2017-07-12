<?php

include_once ROOT . '/inc/lib/tools.inc';

class Request {

    public $params;

    public function __construct() {
        
    }

    public function dang_set_params() {
        if (!empty($_POST)) {
            $this->params = $_POST;
        }
    }

    public function shop_set_params() {
        $input = file_get_contents("php://input");
        $input = urldecode($input);
        Request::getLog($input);
        if (!empty($input)) {
            $this->params = $input;
        }
    }

    public function delete_key($key) {
        uunset($this->params[$key]);
    }

    //过滤准备就绪需要的操作
    public function get_params() {
        $this->filter();
        return $this->params;
    }

    //商家过滤准备就绪需要的操作(IOS访问 本地访问)
    public function shop_get_params() {
        $tt = $ip = array();
        if ($_SERVER['HTTP_HOST'] == $_SERVER['REMOTE_ADDR']) {
            $this->params = json_decode($this->params, 1);
            return $this->params;
        } else {
            $appQuestion = $_SERVER['QUERY_STRING'];
            if (strpos($this->params, '&') == false) {
                    list($a, $b) = explode('=', $this->params);
                    $tt[$a] = $b;
                } else {
                    $t = explode('&', $this->params);
                    foreach ($t as $value) {
                        list($a, $b) = explode('=', $value);
                        $tt[$a] = $b;
                    }
                }
                return $tt;
        }
//		$this->params = json_decode($this->params,1);
//		return $this->params;
    }

    public function filter() {
        include_once ROOT . '/inc/lib/tools.inc';
        if (!empty($this->params['order'])) {
            foreach ($this->params['order'] as $type => &$types) {
                if (!empty($types['detail'])) {
                    $types['detail'] = explode(',', $types['detail']);
                } else {
                    unset($this->params['order'][$type]);
                }
            }
        }
//        if ($this->params['shop_id']) {
//            $this->params['product_ids'] = unserialize($this->params['product_ids']);
//        }
        $this->params = $this->params;
//		$this->params = Tools::u82gbk($this->params);
    }

    //编码统接口	
    public function response($params) {
//		$params = Tools::gbk2u8($params);		
        return json_encode($params);
    }

//         public static function getLog($input)
//        {
//            $handel = fopen("log.txt", "a+");
//            fwrite($handel, "\n\r");
//            fwrite($handel, date("Y-m-d H:i:s") . "  ");
//            fwrite($handel, $input);
//            fwrite($handel, "\n\r");
//            fclose($handel);
//        }

    public static function getLog($input) {
        $nian = date("Y");
        $mous = date("m");
        $ri = date("d");
        $dir = $_SERVER['DOCUMENT_ROOT'] . "/wz.houtai/APPlog/" . $nian . "/" . $mous;
        //首先需要检测目录是否存在
        if (!is_dir($dir))
            mkdir($dir, 0777, true); // 如果不存在则创建
// 获取需要创建的文件名称
        $fileName = $_SERVER['DOCUMENT_ROOT'] . "/wz.houtai/APPlog/" . $nian . "/" . $mous . "/" . $ri . ".txt";
        $handel = fopen($fileName, "a+");
        fwrite($handel, "\n\r");
        fwrite($handel, date("Y-m-d H:i:s") . "  ");
        fwrite($handel, $input);
        fwrite($handel, "\n\r");
        fclose($handel);
    }

    public static function uploads($url) {
        $post_data = file_get_contents("php://input");
        $ch = curl_init();
        curl_setopt($ch, CURLOPT_URL, $url);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
        // post数据
        curl_setopt($ch, CURLOPT_POST, 1);
        // post的变量
        curl_setopt($ch, CURLOPT_POSTFIELDS, $post_data);
        $output = curl_exec($ch);
        curl_close($ch);
        //打印获得的数据
        return $output;
//        print_r($output);
    }

    /**
     * 二维数组按参数排序
     * @param type $multi_array
     * @param type $sort_key
     * @param type $sort
     * @return boolean
     */
    public static function multi_array_sort($multi_array, $sort_key, $sort = SORT_ASC) {
        if (is_array($multi_array)) {
            foreach ($multi_array as $row_array) {
                if (is_array($row_array)) {
                    $key_array[] = $row_array[$sort_key];
                } else {
                    return false;
                }
            }
        } else {
            return false;
        }
        array_multisort($key_array, $sort, $multi_array);
        return $multi_array;
    }

    #获取数组中重复数据

    public static function FetchRepeatMemberInArray($array) {
        $len = count($array);
        for ($i = 0; $i < $len; $i ++) {
            for ($j = $i + 1; $j < $len; $j ++) {
                if ($array [$i] == $array [$j]) {
                    $repeat_arr [] = $array [$i];
                    break;
                }
            }
        }
        return $repeat_arr;
    }

    #删除数组中重复数据

    public static function delFetchRepeatMemberInArray($array) {
        $len = count($array);
        for ($i = 0; $i < $len; $i ++) {
            for ($j = $i + 1; $j < $len; $j ++) {
                if ($array [$i] == $array [$j]) {
//                    $repeat_arr [] = $array [$i];
                    unset($array [$i]);
                    break;
                }
            }
        }
        return $array;
    }

    /**
     * 数组分页函数  核心函数  array_slice 
     * 用此函数之前要先将数据库里面的所有数据按一定的顺序查询出来存入数组中 
     * $count   每页多少条数据 
     * $page   当前第几页 
     * $array   查询出来的所有数组 
     * order 0 - 不变     1- 反序 
     */
    public static function page_array($count, $page, $array, $order) {
        global $countpage; #定全局变量  
        $page = (empty($page)) ? '1' : $page; #判断当前页面是否为空 如果为空就表示为第一页面   
        $start = ($page - 1) * $count; #计算每次分页的开始位置  
        if ($order == 1) {
            $array = array_reverse($array);
        }
        $totals = count($array);
        $countpage = ceil($totals / $count); #计算总页面数  
        $pagedata = array();
        $pagedata = array_slice($array, $start, $count);
        return $pagedata;  #返回查询数据  
    }

    #获取时间差

    public static function getTimeDifference($param) {
        $date_1 = date('Y-m-d');
        $date_2 = date('Y-m-d', $param);
        $date1_arr = explode("-", $date_1);
        $date2_arr = explode("-", $date_2);
        $day1 = mktime(0, 0, 0, $date1_arr[1], $date1_arr[2], $date1_arr[0]);
        $day2 = mktime(0, 0, 0, $date2_arr[1], $date2_arr[2], $date2_arr[0]);
        $days = round(($day1 - $day2) / 3600 / 24);
        return $days;
    }

    public static function checkEmpty($param) {
        if (is_array($param)) {
            foreach ($param as $key => $value) {
                if (empty($param[$key]))
                    $param[$key] = "";
            }
        }
        return $param;
    }
}
