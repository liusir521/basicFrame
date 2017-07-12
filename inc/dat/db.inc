<?php

class DB {

    public static function JiaoyanDB() {
        return self::getDbInstrance(Conf::$JiaoyanDB);
    }
    
    
    public static function YunYingDB(){
        return self::getDbInstrance(Conf::$YuYingDB);
    }

    public static function getDbInstrance($data) {
        static $instance;
        $key = $data["server"] . "_" . $data["database"] . "_" . $data["username"];
        if (!isset($instance[$key]) || !is_object($instance[$key])) {
            $instance[$key] = new myPDO($data["server"], $data["database"], $data["username"], $data["password"], $data['dbtype']);
        }
        return $instance[$key];
    }

    public static function prepare_insert_element($hash) {
        $field_str = '';
        $value_str = '';
        $prepare_array = array();
        foreach ($hash as $k => $v) {
            $field_str .= ',' . $k;
            $value_str .= ',:' . $k;
            $prepare_array[':' . $k] = $v;
        }
        $field_str = trim($field_str, ', ');
        $value_str = trim($value_str, ', ');
        return array('field' => $field_str, 'value' => $value_str, 'prepare' => $prepare_array);
    }

    public static function prepare_update_element($hash) {
        $field_str = '';
        $prepare_array = array();
        foreach ($hash as $k => $v) {
            $field_str .= ',' . $k . '=:' . $k;
            $prepare_array[':' . $k] = $v;
        }
        $field_str = trim($field_str, ',');
        return array('field' => $field_str, 'prepare' => $prepare_array);
    }

}
?>
