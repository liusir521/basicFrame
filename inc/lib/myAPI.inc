<?php
/**
* myApi类//的
 *
 *@param  string $url url地址
 *@param  string/array $param url参数。不必带有“?”
 *@param  string $requst_type 请求方式。GET或POST 默认为GET
 *@param  string $request_mode http请求实现方法。curl或file_get_contents或socket 默认现请求curl，若失败则分别尝试file_get_contents和socket
 * 调用示例：
$html=myAPI::getApi("http://list.dangdang.com", "category=01.01&sort=1");
$html=myAPI::getApi("http://list.dangdang.com", array("category"=>"01.01","sort"=>1),"POST", "soket");
 * 没用对param进行编码 请自行使用rawurlencode对中文参数进行编码
*/
class myAPI {
        private static $access_log=false;

        public static function  getApi($url, $param="",$requst_type="GET", $request_mode="" ,
			$timeout = null,$header = array(), $cookie = '',  $options = array())
        {
                $result="";
                $http_request;
                $requst_type=strtoupper($requst_type);
                switch($request_mode){
                        case "curl":
                                $http_request= new CurlRequest($url, $param, $requst_type,$timeout,$header,$cookie,$options);
                                break;
                        case "file_get_contents":
                                $http_request= new StreamRequest($url, $param, $requst_type,$timeout,$header,$cookie,$options);
                                break;
                        case "socket":
                                $http_request= new SocketRequest($url, $param, $requst_type,$timeout,$header,$cookie,$options);
                                break;
                        default :
                                if (function_exists('curl_init'))
                                        $http_request= new CurlRequest($url, $param, $requst_type,$timeout,$header,$cookie,$options);
                                elseif (function_exists('stream_context_create'))
                                        $http_request= new StreamRequest($url, $param, $requst_type,$timeout,$header,$cookie,$options);
                                elseif (function_exists('socket_connect'))
                                        $http_request= new SocketRequest($url, $param, $requst_type,$timeout,$header,$cookie,$options);
                                else{
                                        $loger = new apiLoger();
                                        $loger->logfile_init();
                                        $loger->logErr(__FUNCTION__." Error!: no request methods can be used.\n");
                                        $loger->logfile_close();
                                        return null;
                                }
                                break;
                }
                $result=$http_request->send();

                if(self::$access_log){
                        $loger = new apiLoger();
                        $loger->logfile_init("access");
                        $loger->logAccess($_SERVER["PHP_SELF"], $url, $param,$requst_type);
                        $loger->logfile_close();
                }
                return $result;
        }


}


class StreamRequest extends HttpRequestMethods{
        public function send(){
                $result =  "";
        try {
                                $url=$this->url;
                                if($this->requst_type=="GET" && !empty($this->param)) {
                                        $parse = parse_url($this->url);
                                        $sep = isset($parse['query']) ? '&' : '?';
                                        $url.=($sep.$this->param);
                                }
                                $opts = array(
                                 'http' => array(
                                  'method'   => $this->requst_type,
                                  'timeout' => $this->timeout,
                                 )
                                );
                                if ($this->requst_type == 'POST'){
                                 $opts['http']['content'] = $this->param;
                                }

                                if(empty($this->header)){
                                        $parse = parse_url($this->url);
                                        $host = $parse['host'];
                                        $this->setHeader("Host: ".$host);
                                        $this->setHeader('Content-type: application/x-www-form-urlencoded');
                                        $this->setHeader('User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.1.2) Gecko/20090729 Firefox/3.5.2');
                                        $this->setHeader('Content-length: '.($this->requst_type=="GET"?'0':strlen($this->param)));
                                        $this->setHeader("Connection: Close");
                                        //'Accept-Language: zh-cn',
                                        //'Cache-Control: no-cache',
                                }
                                if (!empty($this->cookies)){
                                   $this->setHeader("Cookie: ". $this->cookies);
                                }
                                $opts['http']['header'] = '';
                                foreach ($this->header as $h){
                                        $opts['http']['header'] .= $h . "\r\n";
                                }
                                if (!empty($this->options)){
                                         isset($this->options['proxy']) ? $opts['http']['proxy'] = $this->options['proxy'] : '';
                                         isset($this->options['max_redirects']) ? $opts['http']['max_redirects'] = $this->options['max_redirects'] : '';
                                         isset($this->options['request_fulluri']) ? $opts['http']['request_fulluri'] = $this->options['request_fulluri'] : '';
                                }
                                $context = stream_context_create($opts);
                                $result =  file_get_contents($url,false,$context);
                                if ($result === false){
                                   throw new Exception(__CLASS__ .": file_get_contents(". $url .") fail");
                                }
        }
        catch(Exception $e) {
            $loger = new apiLoger();
                        $loger->logfile_init();
                        $loger->logErr(__FUNCTION__." Error!: " . $e->getMessage() . "\n", $e);
                        $loger->logfile_close();
        }
        return $result;
        }
}

class CurlRequest extends HttpRequestMethods{
        public function send(){
                $result =  "";
        try {
                        $curl = curl_init();
                        $url=$this->url;
                        if($this->requst_type=="GET" && !empty($this->param)) {
                                $parse = parse_url($url);
                                $sep = isset($parse['query']) ? '&' : '?';
                                $url.=($sep.$this->param);
                        }
                        if($this->requst_type=="POST"){
                                curl_setopt($curl, CURLOPT_POST, 1 );
                                curl_setopt($curl,CURLOPT_POSTFIELDS,$this->param);
                        }
                        curl_setopt($curl, CURLOPT_URL,$url);

                        curl_setopt($curl, CURLOPT_HEADER, 0);
                        curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
                        curl_setopt($curl, CURLOPT_TIMEOUT, $this->timeout);

                        if (empty($this->header)){
                                $this->setHeader(array(
                                          'User-Agent: Mozilla/5.0 (X11; U; Linux i686; zh-CN; rv:1.9.1.2) Gecko/20090729 Firefox/3.5.2',
                                          //'Accept-Language: zh-cn',
                                          //'Cache-Control: no-cache',
                                         )
                                 );
                        }
                        curl_setopt($curl, CURLOPT_HTTPHEADER, $this->header);
                        //curl_setopt($curl, CURLOPT_USERAGENT, 'Mozilla/5.0 (X11; U; Linux i686; zh-CN; rv:1.9.1.2) Gecko/20090729 Firefox/3.5.2');
                        if (!empty($this->cookies)){
                                curl_setopt($curl, CURLOPT_COOKIE, $this->cookies);
                        }
                        if (!empty($this->options)){
                         curl_setopt_array($curl ,  $this->options);
                        }

                        $result = curl_exec($curl);
                        if( ($err = curl_error($curl)) ){
                                throw new Exception(__CLASS__ ." curl error: ". $err);
                        }
                        curl_close($curl);
        }
        catch(Exception $e) {
                        if($curl) curl_close($curl);
            $loger = new apiLoger();
                        $loger->logfile_init();
                        $loger->logErr(__FUNCTION__." Error!: " . $e->getMessage() . "\n", $e);
                        $loger->logfile_close();
        }
        return $result;

        }
}

class SocketRequest extends HttpRequestMethods{
        public function send(){
                $result =  "";
                try {
                        $parse = parse_url($this->url);
                        if(sizeof($parse)==0) return "";
                        $host = @$parse["host"];
                        $port = @$parse["port"];
                        $uri  = @$parse["path"];
                        $query = @$parse["query"];
                        if($port=="") $port=80;
                        if($uri =="") $uri ="/";

                        if(empty($this->header)){
                                $head_array = array();
                                if($this->requst_type=="GET" && !empty($this->param)) {
                                        $parse = parse_url($this->url);
                                        $sep = isset($parse['query']) ? '&' : '?';
                                        $uri.=($sep.$this->param);
                                }
                                $head_array[]=$this->requst_type.' '.$uri.' HTTP/1.0';
                                $head_array[]= 'User-Agent: Mozilla/5.0 (X11; U; Linux i686; zh-CN; rv:1.9.1.2) Gecko/20090729 Firefox/3.5.2';
                                $head_array[]='Host: '.$host;
                                if ($this->requst_type == 'POST' && !empty($this->param)){
                                        $head_array[]='Content-Type: application/x-www-form-urlencoded';
                                        $head_array[]='Content-Length: '. strlen($this->param);
                                }
                                if ($this->cookies != ''){
                                    $head_array[]='Cookie: '. $this->cookies;
                                }
                                $head_array[]='Connection: Close';
                                //'Accept-Language: zh-cn',
                                //'Cache-Control: no-cache',
                                $this->setHeader($head_array);
                        }
                        $send = '';
                        foreach ($this->header as $h) {
                           $send .= $h ."\r\n";
                        }
                        $send .= "\r\n";
                        if ($this->requst_type == 'POST' && !empty($this->param)){
                           $send .= $this->param ."\r\n";
                        }

                        $conn = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
                        socket_set_option($conn, SOL_SOCKET, SO_RCVTIMEO, array("sec"=>$this->timeout,"usec"=>0));
                        if(socket_connect($conn, $host, $port)==false)
                        {
                                throw new Exception(__CLASS__ ." socket_connect error: ");
                                return "";
                        }

                        socket_send($conn,$send,strlen($send),0);

                        $temp="";
                        while( $temp=socket_read($conn, 2048) )
                        {
                                $result.=$temp;
                        }

                        $status = substr($result,0,12);
                        if($status!="HTTP/1.1 200" && $status!="HTTP/1.0 200"){
                                throw new Exception(__CLASS__ ." status error: ");
                                return "";
                        }
                        $result = strstr($result,"\r\n\r\n");
                        if(!$result) return "";
                        $result=substr($result,4, strlen($result)-4);

                        socket_close($conn);
                }
        catch(Exception $e) {
                        if($conn) socket_close($conn);
            $loger = new apiLoger();
                        $loger->logfile_init();
                        $loger->logErr(__FUNCTION__." Error!: " . $e->getMessage() . "\n", $e);
                        $loger->logfile_close();
        }

                return $result;
        }
}

abstract class HttpRequestMethods
{

        protected $url = '';
        protected $param = '';
        protected $requst_type = '';
        protected $header = array();
        protected $cookies = '';
        protected $timeout = 0;//单位：秒
        protected $options = array();

        public function __construct($url = '', $param = array(), $requst_type="GET",$timeout = null,$header = array(), $cookie = '',  $options = array()) {
                $this->setUrl($url);
                $this->setParam($param);
                $this->setRequstType($requst_type);
                $this->setHeader($header);
                $this->setCookie($cookie);
				$t=$timeout===null?Conf::$api_timeout:$timeout;
                $this->setTimeout($t);
                $this->setOptions($options);
        }

        protected function setUrl($_url){
                if ($_url != '') {
                        if(strncasecmp(substr($_url,0,7),"http://",7)!=0)
                                $_url="http://".$_url;
                   $this->url = $_url;
                }
        }
        protected function setParam($_param){
                if (empty($_param)) return;
                if(is_array($_param))
                        $this->param =$this->makeQueryArr2Str($_param);//自带urlencode 空格转为+
                elseif (is_string($_param))
                        $this->param = $_param;
        }
        protected function setRequstType($_requst_type){
                if ($_requst_type=="POST" || $_requst_type=="GET") {
                   $this->requst_type = $_requst_type;
                }
        }
        protected function setHeader($_header){
                if (empty($_header)) return;
                if (is_array($_header)){
                   foreach ($_header as $k => $v){
                        $this->header[] = is_numeric($k) ? trim($v) : (trim($k) .": ". trim($v));
                   }
                } elseif (is_string($_header)){
                   $this->header[] = $_header;
                }
        }
        protected function setCookie($_cookie){
                if (empty($_cookie)) {
                   return;
                }
                if (is_array($_cookie)){
                   $this->cookies = $this->makeQueryArr2Str($_cookie, ';');
                }
                elseif (is_string($_cookie)){
                   $this->cookies = $_cookie;
                }
        }
        protected function setTimeout($_timeout){
                if (is_numeric($_timeout)) {
                   $this->timeout = $_timeout;
                }
        }
        protected function setOptions($_options){
                if (empty($_options)) {
                   return;
                }
                if (is_array($_options)){
                   $this->options = $_options;
                }
        }

        protected function send(){}

        private function makeQueryArr2Str($array, $sep = '&'){
                $param = '';
                foreach($array as $k=>$v){
                        $param .= ($param ? $sep : "");
                        $param.=($k."=".$v);
                }
                return $param;
        }
}


/* api日志类 */
class apiLoger {

        private $LOGFILE = "";
        var $FILE_HANDLER;

        static function forceDirectory($dir) { // force directory structure
                return is_dir($dir) or (apiLoger::forceDirectory(dirname($dir)) and mkdir($dir, 0777));
        }
        function logfile_init($type="error") {
                if(empty($this->LOGFILE)) {
                        $filename = Conf::$log['log_path'].'api_'.$type.date('Ymd').'.log';
                        $this->LOGFILE=$filename;
                }else {
                        $filename = $this->LOGFILE;
                }
                apiLoger::forceDirectory(Conf::$log['log_path']);
                $this->FILE_HANDLER = fopen($filename,'a');
        }
        public function logErr($str,$e=null) {
                $file_size=filesize($this->LOGFILE);
                if($file_size>1000000000) return ;
                $err= "\n".date(("Ymd H:i:s"))."-->".$str.":::".print_r($e,true)."<--";
                substr($err, 0,15000);
                if ($this->FILE_HANDLER) {
                        fwrite($this->FILE_HANDLER,$err);//暂时用写入log，空间不够
                }
        }
        public function logAccess($str,$url, $param,$requst_type) {
                $file_size=filesize($this->LOGFILE);
                if($file_size>1000000000) return ;
                $acc= "\n".date(("Ymd H:i:s"))."\n sourceURL：".$str." \n url：".$url."\n param：".$param."\n requst_type：".$requst_type."\n";
                substr($acc, 0,15000);
                if ($this->FILE_HANDLER) {
                        fwrite($this->FILE_HANDLER,$acc);//暂时用写入log，空间不够
                }
        }
        public function logfile_close() {
                if ($this->FILE_HANDLER) {
                        fclose($this->FILE_HANDLER) ;
                }

        }

}
?>
