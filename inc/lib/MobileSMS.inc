<?php

/*
 *  Copyright (c) 2014 The CCP project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a Beijing Speedtong Information Technology Co.,Ltd license
 *  that can be found in the LICENSE file in the root of the web site.
 *
 *   http://www.yuntongxun.com
 *
 *  An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

//说明：需要包含接口声明文件，可将该文件拷贝到自己的程序组织目录下。


class MobileSMS {

    public $accountSid = '8a48b55148bbb4140148ed79bbe80b8f';
//说明：主账号，登陆云通讯网站后，可在“控制台-应用”中看到开发者主账号ACCOUNT SID。

    public $accountToken = '1e7864654fa74d3185fae24bad784db3';
//说明：主账号Token，登陆云通讯网站后，可在控制台-应用中看到开发者主账号AUTH TOKEN。

    public $appId = '8a48b551511a2cec0151243fbc622204';
//说明：应用Id，如果是在沙盒环境开发，请配置“控制台-应用-测试DEMO”中的APPID。如切换到生产环境，请使用自己创建应用的APPID。

    public $serverIP = 'app.cloopen.com';
//说明：请求地址。
//沙盒环境配置成sandboxapp.cloopen.com，
//生产环境配置成app.cloopen.com。

    public $serverPort = '8883';
//说明：请求端口 ，无论生产环境还是沙盒环境都为8883.

    public $softVersion = '2013-12-26';

//说明：REST API版本号保持不变。

    /**
     * 发送模板短信
     * @param to 手机号码集合,用英文逗号分开
     * @param datas 内容数据 格式为数组 例如：array('Marry','Alon')，如不需替换请填 null
     * @param $tempId 模板Id 测试应用和未上线应用使用测试模板请填写1，正式应用上线后填写已申请审核通过的模板ID
     */
    function sendTemplateSMS($to, $datas, $tempId) {
// 初始化REST SDK
        include_once ROOT . '/inc/lib/SMSCCPRestSDK.inc';
        $rest = new REST($this->serverIP, $this->serverPort, $this->softVersion);
        $rest->setAccount($this->accountSid, $this->accountToken);
        $rest->setAppId($this->appId);

        $result = $rest->sendTemplateSMS($to, $datas, $tempId);
        if ($result == NULL) {
            echo "result error!";
            return;
        }

        if ($result->statusCode != 0) {
            //下面可以自己添加错误处理逻辑
            $rest = 0;
        } else {
            //下面可以自己添加成功处理逻辑
            $rest = $result->TemplateSMS->dateCreated;
        }
        return $rest;
    }

}

//Demo调用 
//sendTemplateSMS("手机号码","内容数据","模板Id");
?>
