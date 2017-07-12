<?php

/*
 * Error 错误控制类
 * 
 */

class Error {

    /**
     * 错误级别, 此数只能越来越大
     * 0.正常 
     * 1.warning 警告，部分数据可以入库
     * 2.fatal  致命错误，数据不能入库
     * 3.db_error 数据库错误
     * 
     *  @var int 
     */
    public $level = 0;
    public $details = array();
    public $shop_id = 0;

    public function __construct() {
        ;
    }

    public static function init() {
        static $obj = null;
        if (!is_object($obj)) {
            $obj = new Error();
        }
        return $obj;
    }

    public function set_level($level) {
        if ($level > $this->level)
            $this->level = $level;
    }

//	array(1,2,3);
//	array(1=>111,2=>222,3=>333);

    public function set_detail($level, $code, $location, $id = '', $other = '') {
        $this->set_level($level);
        if (isset($id) and ! empty($id)) {
            $this->details[] = array(
                'code' => $code,
                'location' => $location,
                'id' => $id,
                'other' => $other,
            );
        } else {
            $this->details[] = array(
                'code' => $code,
                'location' => $location,
                'id' => '',
                'other' => '',
            );
        }
    }

    public function set_details($level, $code, $location, $ids = array(), $others = array()) {
        $this->set_level($level);
        if (isset($ids) and ! empty($ids)) {
            if (is_array($ids)) {
                foreach ($ids as $id) {
                    $this->details[] = array(
                        'code' => $code,
                        'location' => $location,
                        'id' => $id,
                        'other' => !empty($others) ? $others[$id] : '',
                    );
                }
            }
        } else {
            $this->details[] = array(
                'code' => $code,
                'location' => $location,
                'id' => '',
                'other' => '',
            );
        }
    }

    public function get_detail() {
        return $this->details;
    }

    //0- 正常 1-wanring 2-fatal
    public function get_level() {
        return $this->level;
    }

    public function get_response($data = array()) {
        $res = array('status' => 0);
        $res['status'] = Error::init()->get_level();
        if ($res['status'] > 0) {
            $res['error'] = Error::init()->get_detail();
            
            if (isset($this->shop_id) && $this->shop_id != 6666) {
                foreach ($res['error'] as $k => $tmp) {
                    if (array_key_exists($tmp['code'], Config::$arrResponseMsg)) {
                        $res['error'][$k]['code'] = Config::$arrResponseMsg[$tmp['code']];
                    } elseif (array_key_exists($tmp['code'], Config::$arrTxtResponseMsg)) {
                        $res['error'][$k]['code'] = Config::$arrTxtResponseMsg[$tmp['code']];
                    } elseif (array_key_exists($tmp['code'], Config::$shop_query_tip)) {
                        $res['error'][$k]['code'] = Config::$shop_query_tip[$tmp['code']];
                    }
                }
            }
            if (!empty($data)) {
                foreach ($data as $k => $value) {
                    if (!empty($value)) {
                        $res[$k] = $value;
                    }
                }
            }

            return $res;
        }
        if (!empty($data)) {
            foreach ($data as $k => $value) {
                if (!empty($value)) {
                    $res[$k] = $value;
                }
            }
        }
        
        return $res;
    }

}
