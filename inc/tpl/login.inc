<!DOCTYPE html><head>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
    <meta content="webkit" name="renderer">
    <meta content="width=device-width,initial-scale=1.0,minimum-scale=1.0,maximum-scale=1.0,user-scalable=no" name="viewport">
    <title>学习8-登录</title>
    <link rel="stylesheet" href="css/bootstrap.min.css">
    <link rel="stylesheet" href="css/login.css">
    <script src="js/jquery-1.11.1.min.js"></script>
    <script src="js/bootstrap.min.js"></script>
    <script src="js/modal.js"></script>
    <style>
        .lable-che{
            margin-top: 5px;
            color: red;
            display: none;
        }
    </style>
</head>
<body style=" background: url('images/login_bg.jpg') center top no-repeat; display: block; height: auto; max-width: 100%;  ">        
    <div class="container">
        <div class="logo">
        </div>
        <div class=" user-login row">
            <div class="col-md-6 user-login-left">
                <div class="user-login-left-title">
                    <span>登录</span>
                </div>
                <div class="user-login-form">
                    <div class="form-group">
                        <label for="ajax-username" class="col-md-3 control-label hidden">手机号</label>
                        <div class="controls">
                            <input type="text" placeholder="手机号" name="phone" id="phone" class="form-control" data-widget-cid="widget-2" data-explain="">
                            <label for="ajax-username" id="phone-check" class="control-label lable-che">请输入正确手机号！</label>
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="ajax-password" class="col-md-3 control-label hidden">密码</label>
                        <div class="controls">
                            <input type="password" placeholder="密码" name="password1" id="password1" class="form-control" data-widget-cid="widget-3" data-explain="">
                            <label for="ajax-username" id="password-check" class="control-label lable-che">请输入密码，长度不低于6位！</label>
                        </div>
                    </div>
                    <div class="pemember-password-wrap">
                        <div class="col-md-12 padding-l-r-0">
                            <button class="btn btn-primary user-modal-reg-button" onclick="CheckForm1()" type="button">登录</button>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
    
    
    <script>
        document.onkeydown=function(event){
        e = event ? event :(window.event ? window.event : null);
            if(e.keyCode==13){
                CheckForm1();
            }
        } 
        function CheckForm1() {
            var check = true;
            var phone = $("#phone").val();//获取手机号
            var telReg = !!phone.match(/^(0|86|17951)?(13[0-9]|15[012356789]|17[678]|18[0-9]|14[57])[0-9]{8}$/);
            if (telReg == false) {
                check = false;
                $("#phone-check").show();
                $('#phone').attr("style", "border:1px solid #FF0000");
                return false;
            }

            var password = $("#password1").val();
            if (password.length < 6) {
                check = false;
                $('#password1').attr("style", "border:1px solid #FF0000");
                $("#password-check").show();
            }

            if (check) {
                $.ajax({
                    type: "POST",
                    url: 'index.php?c=User&a=login_put',
                    data: "phone=" + phone + "&password1=" + password,
                    dataType: 'json',
                    success: function (data) {
                        if (data == 0) {
                            var address="<?= $_SERVER['HTTP_REFERER'] ?>";
                            if(address==="http://www.xuexi8.net/index.php?c=user&a=regedit"){
                                location.href = "http://www.xuexi8.net/";
                            }else{
                                location.href = "<?= $_SERVER['HTTP_REFERER'] ?>";
                            }
                            
                        } else {
                            $("#password-check").html(data);
                            $("#password-check").show();
//                            showMessage_noexit(data, "alert-danger");
                        }
                    }
                });
            }

        }
        $('input').blur(function () {
            var inputid = $(this).attr("id");
            if (inputid == "phone") {
                var phone = $("#phone").val();//获取手机号
                var telReg = !!phone.match(/^(0|86|17951)?(13[0-9]|15[012356789]|17[678]|18[0-9]|14[57])[0-9]{8}$/);
                //如果手机号码不能通过验证
                if (telReg == false) {
                    $("#phone-check").show();
                    $('#phone').attr("style", "border:1px solid #FF0000");
                } else{
                    $.ajax({
                        url:'index.php?c=user&a=checkPhone',
                        type:'post',
                        data:'phone='+phone,
                        dataType:'json',
                        success:function(data){
                            //alert(data);
                            if(data==1){
                                $("#phone-check").show();
                                $("#phone-check").html("该手机号未注册");
                                $('#phone').attr("style", "border:1px solid #FF0000");
                            }else{
                                $("#phone-check").hide();
                                $('#phone').attr("style", "1px solid rgb(178, 178, 178)");
                            }
                        }
                    })
                }/*else {
                    $("#phone-check").hide();
                    $('#phone').attr("style", "1px solid rgb(178, 178, 178)");
                }*/
            } else {
                var password = $("#password1").val();
                if (password.length < 6) {
                    $("#password-check").show();
                    $('#password1').attr("style", "border:1px solid #FF0000");
                } else {
                    $("#password-check").hide();
                    $('#password1').attr("style", "1px solid rgb(178, 178, 178)");
                }
            }
        });
    </script>
</body>
</html>