<?php

//的
class Validate {

    static public function isEmail($email) {
        return preg_match('/^[a-z0-9]+[._a-z0-9-]*@[a-z0-9]+[._a-z0-9-]*\.[a-z0-9]+$/ui', $email);
    }

    static public function isMd5($md5) {
        return preg_match('/^[a-z0-9]{32}$/ui', $md5);
    }

    static public function isSha1($sha1) {
        return preg_match('/^[a-z0-9]{40}$/ui', $sha1);
    }

    static public function isFloat($float) {
        return strval(floatval($float)) == strval($float);
    }

    static public function isUnsignedFloat($float) {
        return strval(floatval($float)) == strval($float) AND $float >= 0;
    }

    static public function isOptFloat($float) {
        return empty($float) OR self::isFloat($float);
    }

    static public function isCleanHtml($html) {
        return !preg_match('/<[ \t\n]*script/ui', $html);
    }

    static public function isPasswd($passwd, $size = 5) {
        return preg_match('/^[.a-z_0-9-]{' . $size . ',32}$/ui', $passwd);
    }

    static public function isDate($date) {
        if (!preg_match('/^([0-9]{4})-((0?[1-9])|(1[0-2]))-((0?[1-9])|([1-2][0-9])|(3[01]))( [0-9]{2}:[0-9]{2}:[0-9]{2})?$/ui', $date, $matches))
            return false;
        return checkdate(intval($matches[2]), intval($matches[5]), intval($matches[0]));
    }

    static public function isBirthDate($date) {
        if (empty($date))
            return true;
        if (preg_match('/^([0-9]{4})-((0?[1-9])|(1[0-2]))-((0?[1-9])|([1-2][0-9])|(3[01]))( [0-9]{2}:[0-9]{2}:[0-9]{2})?$/ui', $date, $birthDate)) {
            if ($birthDate[1] >= date('Y') - 9)
                return false;
            return true;
        }
        return false;
    }

    static public function isBool($bool) {
        return is_null($bool) OR is_bool($bool) OR preg_match('/^[0|1]{1}$/ui', $bool);
    }

    static public function isPhoneNumber($phoneNumber) {
        return preg_match('/^[+0-9. ()-]*$/ui', $phoneNumber);
    }

    static public function isOrderWay($orderWay) {
        return ($orderWay === 'ASC' | $orderWay === 'DESC' | $orderWay === 'asc' | $orderWay === 'desc');
    }

    static public function isInt($int) {
        return (int) ((string) $int) OR $int == 0;
    }

    static public function isUnsignedInt($int) {
        return ((int) ((string) $int) OR $int == 0) AND $int < 4294967296 AND $int >= 0;
    }

    static public function isUnsignedId($id) {
        return self::isUnsignedInt($id); /* Because an id could be equal to zero when there is no association */
    }

    static public function isNullOrUnsignedId($id) {
        return is_null($id) OR self::isUnsignedId($id);
    }

    static public function isLoadedObject($object) {
        return is_object($object) AND $object->id;
    }

    static public function isColor($color) {
        return preg_match('/^(#[0-9A-Fa-f]{6}|[[:alnum:]]*)$/ui', $color);
    }

    static public function isUrl($url) {
        return preg_match('/^([[:alnum:]]|[:#%&_=\(\)\.\? \+\-@\/])+$/ui', $url);
    }

    static public function isAbsoluteUrl($url) {
        if (!empty($url))
            return preg_match('/^https?:\/\/([[:alnum:]]|[:#%&_=\(\)\.\? \+\-@\/])+$/ui', $url);
        return true;
    }

    static public function isFileName($name) {
        return preg_match('/^[a-z0-9_.-]*$/ui', $name);
    }

    /**
     * 验证字符串长度
     *
     * @param string $value
     * @param int $len
     *
     * @return boolean
     */
    static function isLenthEqual($value, $len) {
        return strlen($value) == (int) $len;
    }

    /**
     * 最小长度
     *
     * @param mixed $value
     * @param int $len
     *
     * @return boolean
     */
    static function isMinLength($value, $len) {
        return strlen($value) >= $len;
    }

    /**
     * 最大长度
     *
     * @param mixed $value
     * @param int $len
     *
     * @return boolean
     */
    static function isMaxLength($value, $len) {
        return strlen($value) <= $len;
    }

    /**
     * 不为空
     *
     * @param mixed $value
     *
     * @return boolean
     */
    static function isNotEmpty($value = '') {
        return strlen($value);
    }

    /**
     * 是否包含非法字符
     *
     * @param mixed $value
     *
     * @return boolean
     */
    static function isUnlawful($value = '') {
        include_once ROOT . '/inc/lib/badword.inc';
        $bo = false;
        foreach ($badword as $k => $v) {
            $pos = strpos($value, $v);
            if ($pos !== false) {
                $bo = true;
                break;
            }
        }
        return $bo;
    }

}

?>
