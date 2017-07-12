<?php

include_once ROOT . '/inc/biz/Config.inc';

class Tools {

    static public function upload($dest_dir, $fname = '', $fileElementName = 'fileToUpload', $ext = array('xls', 'rar')) {

        function getFilesStr($dest_dir) {
            $str = "";
            if (!is_dir($dest_dir))
                return "";
            $handle = opendir($dest_dir);
            while ($file = readdir($handle)) {
                $extstr = strtolower(end(explode(".", $file))); //取得后缀名
                if (!empty($extstr))
                    $str .= $file . ",";
            }
            return rtrim($str, ",");
        }

        $max_size = 2000 * 1024;
        $error = "";
        $msg = "";
        $file_path = '';
        $strfiles = '';

        $extstr = strtolower(end(explode(".", $_FILES[$fileElementName]['name']))); //取得后缀名
        if (!empty($_FILES[$fileElementName]['error'])) {
            switch ($_FILES[$fileElementName]['error']) {
                case '1'://The uploaded file exceeds the upload_max_filesize directive in php.ini
                case '2'://The uploaded file exceeds the MAX_FILE_SIZE directive that was specified in the HTML form
                    $error = '您上传的文件太大，请修改!';
                    break;
                case '3'://The uploaded file was only partially uploaded
                case '4'://No file was uploaded.
                case '6'://Missing a temporary folder
                case '7'://Failed to write file to disk
                case '8'://File upload stopped by extension
                    $error = '上传失败!';
                    break;
                case '999':
                default:
                    $error = '上传失败！';
            }
        } elseif (empty($_FILES[$fileElementName]['tmp_name']) || $_FILES[$fileElementName]['tmp_name'] == 'none') {
            $error = '上传失败..找不到上传文件！';
        } elseif (!in_array($extstr, $ext)) {
            $error = '上传失败，文件类型错误！';
        } elseif ($_FILES[$fileElementName]['size'] > $max_size) {
            $error = '您上传的文件太大，请修改！';
        } else {
            try {

                if (empty($fname))
                    $fname = $_FILES[$fileElementName]['name'];
                $tempfile = $_FILES[$fileElementName]['tmp_name'];

                $t_dir = rtrim($dest_dir, "/");
                if (!is_dir($t_dir)) {
                    mkdir($t_dir, 0777);
                }
                $file_name = $dest_dir . $fname;
                $r = move_uploaded_file($tempfile, $file_name);
                chmod($file_name, 0755); //设定上传的文件的属性
                //unlink($tempfile);
                $strfiles = getFilesStr($dest_dir);
            } catch (PDOException $e) {
                print_r($e);
                return "{error:'上传失败！'}";
            }
        }
        $href = str_replace(Conf::$upload_dir_tmp, "upload/", $dest_dir);
        $strhref = $href . $fname . "?t=" . time();
        return "{error:'" . $error . "'," . "file:'" . $strfiles . "',href:'" . $strhref . "'}";
    }

    static public function getValue($key, $defaultValue = false) {
        if (!isset($key) OR empty($key) OR ! is_string($key))
            return false;
        $ret = (isset($_POST[$key]) ? $_POST[$key] : (isset($_GET[$key]) ? $_GET[$key] : $defaultValue));

        if (is_string($ret) === true)
            $ret = urldecode(preg_replace('/((\%5C0+)|(\%00+))/i', '', urlencode($ret)));
        return !is_string($ret) ? $ret : stripslashes($ret);
    }

    static public function toExcle($data) {

        function cleanData(&$str) {
            $str = preg_replace("/\t/", "\\t", $str);
            $str = preg_replace("/\r?\n/", "\\n", $str);
            if (strstr($str, '"'))
                $str = '"' . str_replace('"', '""', $str) . '"';
        }

        # filename for download
        $filename = "website_data_" . date('Ymd') . ".xls";

        header("Content-Disposition: attachment; filename=\"$filename\"");
        header("Content-Type: application/vnd.ms-excel");

        $flag = false;
        foreach ($data as $row) {
            if (!$flag) {
                # display field/column names as first row
                echo implode("\t", array_keys($row)) . "\n";
                $flag = true;
            }
            array_walk($row, 'cleanData');
            echo implode("\t", array_values($row)) . "\n";
        }
        exit;
    }

    public static function toPicDownload($pid, $id, $image_date) {
        # filename for download
        $filename = $pid . "_" . $id . ".jpg";

        header("Content-Disposition: attachment; filename=\"$filename\"");
        header("Content-Type: image/jpg");
        echo $image_date;
        exit;
    }

    /**
     *
     *  从一个二维数组中取出一列或多列的值组成一个新的数组
     *  @String	$keyname为string型(即一个列名称) 返回一维数组
     *  @array	$keyname为array型(即多个列名称) 返回二维数组
     *  @array	$keyname为默认(即不输入列名称) 返回二维数组
     *
     */
    static public function array_arrange($array, $keynames = array(), $is_unique = false) {
        $result = array();
        if (!is_array($array) || count($array) == 0)
            return $result;

        //若keyname为数组型 则构建result数组结构
        if (is_array($keynames)) {
            //若keynames为空数组 则用原数组的所有列名称填充keynames
            if (count($keynames) == 0) {
                foreach ($array as $key => $value) {
                    $keynames = array_merge($keynames, array_keys($value));
                }
            }
            $keynames = array_unique($keynames);
            //初始化result结构
            foreach ($keynames as $key_kn => $value_kn) {
                $result = array_merge($result, array($value_kn => array()));
            }
        }

        //为result赋值
        foreach ($array as $key => $value) {
            if (is_array($keynames)) {
                foreach ($keynames as $key_kn => $value_kn) {
                    if (array_key_exists($value_kn, $value) && !($is_unique && in_array($value[$value_kn], $result[$value_kn])))
                        $result[$value_kn][] = $value[$value_kn];
                }
            }
            else if (array_key_exists($keynames, $value) && !($is_unique && in_array($value[$keynames], $result)))
                $result[] = $value[$keynames];
        }
        return $result;
    }

    /**
     *
     *  产生对象数组
     *  @array	$rowArray:	从数据库中取出的多条数据组成的数组
     *  @Class	$infoClass	info类
     *  @array	$mapArray		info 类的相关属性做为key值,分别对应$rowArray中数据的相应列,
     *  @String	$keyName		$rowArray中某一列名称
     *  @boolean	$onlyKey		$keyName是否为$rowArray中主键列名称：true是，false不是
     *  @return	array 	$objArray	由info对象组成的数据
     *
     */
    public static function createObjectArray($rowArray, $infoClass, $mapArray, $keyName = 'id', $onlyKey = true) {
        $objArray = array();

        if (!empty($rowArray)) {
            foreach ($rowArray as $valArray) {
                $objInfo = new $infoClass();

                if (array_key_exists($valArray[$keyName], $objArray) && $onlyKey)
                    continue;
                foreach ($mapArray as $propertyName => $arrangeName) {
                    $objInfo->$propertyName = $valArray[$arrangeName];
                }
                if ($onlyKey) {
                    $objArray[$valArray[$keyName]] = $objInfo;
                } else {
                    $objArray[$valArray[$keyName]][] = $objInfo;
                }
            }
        }
        return $objArray;
    }

    static public function redirect($url, $baseUri = __PS_BASE_URI__) {
        if (isset($_SERVER['HTTP_REFERER']) AND ( $url == $_SERVER['HTTP_REFERER']))
            header('Location: ' . $_SERVER['HTTP_REFERER']);
        else
            header('Location: ' . $baseUri . $url);
        exit;
    }

    static public function redirectLink($url) {
        header('Location: ' . $url);
        exit;
    }

    static public function redirectLogin($url) {
        $js = "<script>top.location.href='" . $url . "'</script>";
        echo $js;
    }

    static public function isSubmit($submit) {
        return (
                isset($_POST[$submit]) OR isset($_POST[$submit . '_x']) OR isset($_POST[$submit . '_y'])
                OR isset($_GET[$submit]) OR isset($_GET[$submit . '_x']) OR isset($_GET[$submit . '_y'])
                );
    }

    static public function throwError($errno, $message, $errfile, $errline) {
        $error_page = "/error.php";
        $error_url = $error_page;
        if (Conf::$isDebug) {
            $error_url .= "?REDIRECT_ERROR_NOTES=error: [$errno]$message";
            $error_url .= "<br>file: " . $errfile . "<br>line: " . $errline;
        }
        self::redirectLink($error_url);
        exit;
    }

    static public function get_user_ip() {
        $user_IP = (isset($_SERVER["HTTP_VIA"])) ? $_SERVER["HTTP_X_FORWARDED_FOR"] : $_SERVER["REMOTE_ADDR"];
        return $user_IP;
    }

    //给用户产生一个独一无二的令牌号
    static public function genToken($len = 32, $md5 = true) {
        # Seed random number generator
        # Only needed for PHP versions prior to 4.2
        mt_srand((double) microtime() * 1000000);
        # Array of characters, adjust as desired
        $chars = array(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);

        # Array indice friendly number of chars;
        $numChars = count($chars) - 1;
        $token = '';

        # Create random token at the specified length
        for ($i = 0; $i < $len; $i++)
            $token .= $chars[mt_rand(0, $numChars)];

        # Should token be run through md5?
        if ($md5) {

            # Number of 32 char chunks
            $chunks = ceil(strlen($token) / 32);
            $md5token = '';

            # Run each chunk through md5
            for ($i = 1; $i <= $chunks; $i++)
                $md5token .= md5(substr($token, $i * 32 - 32, 32));

            # Trim the token
            $token = substr($md5token, 0, $len);
        }
        return $token;
    }

    static public function myencode($html) {


        $len = strlen($html);
        $output = '';

        for ($i = 0; $i < $len; $i++) {
            $ch = $html[$i];
            $ch2 = ord($ch);
            if (($ch2 < 0x20) && ($ch2 != 0x0D) && ($ch2 != 0x0A) && ($ch2 != 0x09))
                continue;
            if ($ch == '%') {
                $output .= '%037';
            } else if (ord($ch) > 126) {
                $output .= '%' . ord($ch);
            } else
                $output .= $ch;
        }
        return $output;
    }

    static public function mydecode($html) {


        $len = strlen($html);
        $output = '';

        for ($i = 0; $i < $len; $i++) {
            $ch = $html[$i];
            if ($ch == '%') {
                $code = substr($html, $i + 1, 3);
                if ($code == '037') {
                    $output .= '%';
                    $i = $i + 3;
                } else {
                    $output .= chr($code);
                    $i = $i + 3;
                }
            } else
                $output .= $ch;
        }
        return $output;
    }

    static public function getCurrentPageUrl() {

        $pageURL = 'http';
        if (isset($_SERVER["HTTPS"]) && $_SERVER["HTTPS"] == "on") {
            $pageURL .= "s";
        }
        $pageURL .= "://";
        $pageURL .= isset($_SERVER['HTTP_X_FORWARDED_HOST']) ? $_SERVER['HTTP_X_FORWARDED_HOST'] : (isset($_SERVER['HTTP_HOST']) ? $_SERVER['HTTP_HOST'] : '');
        $pageURL .= $_SERVER["REQUEST_URI"];

        return $pageURL;
    }

    public static function truncate($instr, $len = 22, $po = 1) {
        $newstr;
        $pot = '';
        if ($po) {
            $pot = "<span class='dot'>...</span>";
        }
        if (strlen($instr) > $len) {
            $newstr = self::msubstr($instr, 0, $len);
            $newstr = str_replace(" ", "&nbsp;", $newstr) . $pot;
            return $newstr;
        } else {
            return str_replace(" ", "&nbsp;", $instr);
        }
    }

    private static function msubstr($str, $start, $len) {

        $tmpstr = "";
        $strlen = $start + $len;
        for ($i = $start; $i < $strlen; $i++) {

            if (ord($str[$i]) > 0x80) {
                if ($i + 1 < $strlen) {
                    $tmpstr .= substr($str, $i, 2);
                }

                $i++;
            } else {
                $tmpstr .= substr($str, $i, 1);
            }
        }
        return $tmpstr;
    }

    public static function alert($value, $JSCode = "") {
        if (!headers_sent())
            header('Content-Type: text/html; charset=gb2312');
        $Js = "<script language=javascript encode='gbk'>";
        $value = $value;
        if (!empty($value))
            $Js.="alert('" . $value . "');\r\n";
        $Js.=$JSCode . "\r\n";
        $Js.="</script>";
        return $Js;
    }

    public static function fprint($array) {
        echo "<pre>";
        print_r($array);
        echo "</pre>";
        exit;
    }

    ///判断是否是utf8编码 Returns true if $string is valid UTF-8 and false otherwise.
    static function isUtf8($string) {
        // From http://w3.org/International/questions/qa-forms-utf-8.html
        return preg_match('%^(?:
        [\x09\x0A\x0D\x20-\x7E] # ASCII
        | [\xC2-\xDF][\x80-\xBF] # non-overlong 2-byte
        | \xE0[\xA0-\xBF][\x80-\xBF] # excluding overlongs
        | [\xE1-\xEC\xEE\xEF][\x80-\xBF]{2} # straight 3-byte
        | \xED[\x80-\x9F][\x80-\xBF] # excluding surrogates
        | \xF0[\x90-\xBF][\x80-\xBF]{2} # planes 1-3
        | [\xF1-\xF3][\x80-\xBF]{3} # planes 4-15
        | \xF4[\x80-\x8F][\x80-\xBF]{2} # plane 16
        )*$%xs', $string);
    }

    static function getNickName($userInfo) {
        $nick_name = trim($userInfo->nick_name);
        if (!empty($nick_name)) {
            return $nick_name;
        } else {
            $pos = strpos($userInfo->email, '@');
            $nEmail = substr($userInfo->email, 0, $pos);
            if ($nEmail) {
                $nEmail.='@***.***';
            }
            return $nEmail;
        }
    }

//店铺销售评价score为1是一颗星，1.5是一颗半星
    static function getFlowers($score, $imgUrl = '/shop/images/flower.gif', $sum = 5) {
        $str = '';

        $pos = strpos($imgUrl, '.');
        $prefix = substr($imgUrl, 0, $pos);
        $postfix = substr($imgUrl, $pos);

        $imgUrl2 = $prefix . '2' . $postfix;
        $imgUrl3 = $prefix . '3' . $postfix;

        if ($score > 0 && $score < $sum + 1) {
            for ($i = 1; $i <= floor($score); $i++) {
                $str .= "<img src=\"{$imgUrl}\" />";
            }
            if ($score > floor($score)) {
                $str .= "<img src=\"{$imgUrl2}\" />";
            }
            for ($i = ceil($score) + 1; $i <= $sum; $i++) {
                $str .= "<img src=\"{$imgUrl3}\" />";
            }
        }
        if (empty($str)) {
            for ($i = ceil($score) + 1; $i <= $sum; $i++) {
                $str .= "<img src=\"{$imgUrl3}\" />";
            }
        }
        return $str;
    }

    static function displayPrice($price) {
        return sprintf("%01.2f", $price);
    }

    static function check_image_format($filename, $show_type = false) {
        $img_array = array('jpg' => 255216, 'gif' => 7173);
        $file = fopen($filename, "rb");
        $bin = fread($file, 2); //只读2字节
        fclose($file);
        $strInfo = @unpack("C2chars", $bin);
        $typeCode = intval($strInfo['chars1'] . $strInfo['chars2']);
        if (in_array($typeCode, $img_array)) {
            if ($show_type)
                return array_search($typeCode, $img_array);
            return true;
        }
        return false;
    }

    static function check_image_url($pic_url) {
        $ext = array("jpg", "gif", "swf", "png");
        $extstr = strtolower(end(explode(".", $pic_url))); //取得后缀名
        if (in_array($extstr, $ext)) {
            return true;
        } else {
            return false;
        }
    }

    /**
     * 生成HTML SELECT中的下拉项
     * @param Array $array
     * @param String $selected
     */
    static function creatSelectOptions($array, $selected = "selected") {
        if (count($array) == 0) {
            return "";
        }
        $str = "";
        foreach ($array as $key => $value) {
            $str.='<option value="' . $key . '" ';
            if ($key == $selected or $value == $selected) {
                $str.=' selected ';
            }
            if ($value == '')
                $value = '-----请选择-----';
            $str.=' >' . $value . '</option>';
        }
        return $str;
    }

    /**
     * 将数组或者字符串转化为gbk=>utf8
     */
    public static function gbk2u8($array) {
        if (!is_array($array)) {
            return mb_convert_encoding($array, "utf-8", "gbk");
        }
        foreach ($array as &$value) {
            $value = self::gbk2u8($value);
        }
        return $array;
    }

    /**
     * 将数组或者字符串转化为utf8=>gbk
     */
    public static function u82gbk($array) {
        if (!is_array($array)) {
            return mb_convert_encoding($array, "gbk", "utf-8");
        }
        foreach ($array as &$value) {
            $value = self::u82gbk($value);
        }
        return $array;
    }

    /*
     * 批量检查提示
     */

    public static function batchCheckStr($value, $title, $num = 100) {
        $str = "";
        switch ($value) {
            case 1:
                $str = "<font color='red'>" . $title . "项不能为空</font><br />";
                break;
            case 2:
                $str = "<font color='red'>" . $title . "项字数不能大于" . $num . "字节</font><br />";
                break;
            case 3:
                $str = "<font color='red'>" . $title . "项不能为汉字</font><br />";
                break;
            case 4:
                $str = "<font color='red'>" . $title . "项不是时间格式</font><br />";
                break;
            case 999:
                $str = "<font color='red'>" . $title . "项不合法</font>";
        }
        return $str;
    }

    /**
     *  格式化ID
     * @param array $post
     * @return array
     */
    public static function format_ids($ids) {
        $ids = trim($ids);
        if ($ids === '') {
            return '';
        }
        $ids = str_replace(array("，", "，"), array(',', ','), $ids);
        $t = array();
        $t = explode(',', $ids);
        $ids = '';
        for ($i = 0, $len = count($t); $i < $len; $i++) {
            if (empty($t[$i])) {
                continue;
            }
            if (!Validate::isProductId($t[$i])) {
                echo self::alert("输入的信息格式不正确", "history.back(-1);");
            }
            $ids != '' && $ids.=",";
            $ids.=$t[$i];
        }
        return $ids;
    }

    static function formatdate($date, $format = "Y-m-d") {
        $time = strtotime($date);
        if ($time < strtotime("1971-1-1"))
            return "";
        else
            return date($format, $time);
    }

    static function addTag($URL, $discount) {
        $TAG = '';
        if (!empty($URL['action_id'])) {
            $now_time = date('Y-m-d H:i:s');
            if ($now_time >= $URL['action_start_date'] && $now_time <= $URL['action_end_date'])
                $TAG = campaignIcon::showIcon(99, $URL['action_id'], $discount);
        }
        return $TAG;
    }

    /**
     * 封装分页数量
     * @param type $param
     */
    static public function getLimit($page_num) {
        $page_size = 5;
        $limit = ($page_num - 1) * $page_size . "," . $page_size;
        return $limit;
    }
    
    static public function getLimit12($page_num) {
        $page_size = 12;
        $limit = ($page_num - 1) * $page_size . "," . $page_size;
        return $limit;
    }

    /**
     * 发送通知
     * @param type $param
     */
    static public function toMessage($user_id, $notice_str, $notice_type) {
        PushNotice::$tartype = 2;
        $invUser = MobileUserSetting::getUserById($user_id);
        if ($invUser[0]['equipment'] == 1) {
            PushNotice::PushSingleAccountIOS($invUser[0]['phone'], Config::$notice_str[$notice_str], $notice_type);
        } else {
            PushNotice::PushSingleAccountAndroid($invUser[0]['phone'], Config::$notice_str[$notice_str], $notice_type);
        }
    }

    static public function toMessageByPhone($phone, $notice_str, $notice_type) {
        PushNotice::$tartype = 2;
        $invUser = MobileUserSetting::getUserByPhone($phone);
        if ($invUser[0]['equipment'] == 1) {
            PushNotice::PushSingleAccountIOS($invUser[0]['phone'], Config::$notice_str[$notice_str], $notice_type);
        } else {
            PushNotice::PushSingleAccountAndroid($invUser[0]['phone'], Config::$notice_str[$notice_str], $notice_type);
        }
    }

    /**
     * 问题推送
     * @param type $params
     * @param type $question_id
     * @param type $type
     */
    static public function getQuestionToTeacher($params, $question_id, $type) {
        PushNotice::$tartype = 2;
        if ($type == 1) {
            $teacher[0] = MobileUserSetting::getUserById($params['locled_userid']);
            if ($teacher[0]['equipment'] == 1) {
                PushNotice::PushSingleAccountIOS($teacher[0]['phone'], Config::$notice_str[8], 1, $type);
            } else {
                PushNotice::PushSingleAccountAndroid($teacher[0]['phone'], Config::$notice_str[8], 1, $type);
            }
        } else {
            $teachers = QuestionSetting::getTeacherByGood_subject($params['grade_class']);
            foreach ($teachers as $key => $value) {
                $online = TeacherSetting::getOnlineUserById($value['id']);
                if (empty($online)) {
                    unset($teachers[$key]);
                }
                $isonline = TeacherSetting::getOnlineUserById($value['id']);
                if (empty($isonline)) {
                    unset($teachers[$key]);
                } else {
                    $locled = QuestionSetting::getLocledTeacher($value['id']);
                    if (!empty($locled)) {
                        unset($teachers[$key]);
                    }
                }
                $user = MobileUserSetting::getUserById($value['id']);
                if (empty($user[0]['teacher_school_age'])) {
                    unset($teachers[$key]);
                }
            }
            $arr = array_values($teachers);
            if (!empty($arr)) {
                foreach ($arr as $key => $value) {
                    if ($value['equipment'] == 1) {
                        PushNotice::PushSingleAccountIOS_QD($value['phone'], Config::$notice_str[7], $question_id);
                    } else {
                        PushNotice::PushSingleAccountAndroid_QD($value['phone'], Config::$notice_str[7], $question_id);
                    }
                }
            }
        }
    }

    /**
     * 问题转到专职老师
     * @param type $params
     */
    static public function getQuestionToZZTeacher($params) {
        $fullt = Config::$full_teacher[1];
        foreach ($fullt as $key => $value) {
            $teachers = QuestionSetting::getTeacherByGood_subject_Uid($params['grade_class'], $value);
            if (!empty($teachers[0])) {
                $teacher[] = $value;
            }
        }
        if (count($teacher) >= 2) {
            $ne['locled_userid'] = $teacher[array_rand($teacher)];
        } else {
            $ne['locled_userid'] = $teacher[0];
        }
        $ne['locked_time'] = time();
        $ne['status'] = 2;
        $ques = QuestionSetting::updateQuestionByQid($ne, $params['question_id']);
        self::toMessage($ne['locled_userid'], 8, "t1");
        return $ques;
    }

    static public function toAllTeacher($param) {
        $teachers = QuestionSetting::getTeacherByGood_subject($params['grade_class']);
        foreach ($teachers as $key => $value) {
            $online = TeacherSetting::getOnlineUserById($value['id']);
            if (empty($online)) {
                unset($teachers[$key]);
            }
            $isonline = TeacherSetting::getOnlineUserById($value['id']);
            if (empty($isonline)) {
                unset($teachers[$key]);
            } else {
                $locled = QuestionSetting::getLocledTeacher($value['id']);
                if (!empty($locled)) {
                    unset($teachers[$key]);
                }
            }
            $user = MobileUserSetting::getUserById($value['id']);
            if (empty($user[0]['teacher_school_age'])) {
                unset($teachers[$key]);
            }
        }
        $arr = array_values($teachers);
        if (!empty($arr)) {
            foreach ($arr as $key => $value) {
                if ($value['equipment'] == 1) {
                    PushNotice::PushSingleAccountIOS_QD($value['phone'], Config::$notice_str[7], $question_id);
                } else {
                    PushNotice::PushSingleAccountAndroid_QD($value['phone'], Config::$notice_str[7], $question_id);
                }
            }
        }
    }

    public static function printArray($input, $name = "yyl") {
        $handel = fopen($name . '.txt', 'a+b');
        fwrite($handel, "\n\r");
        fwrite($handel, date("Y-m-d H:i:s") . "  ");
        fwrite($handel, print_r($input, true));
        fwrite($handel, "\n\r");
        fclose($handel);
    }

    public static function to_encode($params) {
        $params = urlencode($params);
        for ($i = 1; $i <= 9; $i++) {
            $params = base64_encode($params);
        }
        return urlencode($params);
    }

    public static function to_decode($params) {
        $params = urldecode($params);
        for ($i = 1; $i <= 9; $i++) {
            $params = base64_decode($params);
        }
        return urldecode($params);
    }

}

?>
