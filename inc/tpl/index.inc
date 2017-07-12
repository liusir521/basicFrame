<?php include_once 'header.inc'; ?>
<div class="container containers ">
    <div class="row">
        <div class="col-md-2 ">
            <h2>测试</h2>
        </div>
        <div class="col-md-10 ">
             <button id="tanchu" class="btn btn-info btn-sm">弹出框</button>
            <a  href="#myModal" data-toggle="modal" data-url="index.php?c=User&a=getStudent" class="btn btn-info btn-sm">新建客户</a>
        </div>
    </div>
</div>
<script type="text/javascript">
$("#tanchu").click(function(){
    showMessage("谢谢","alert-danger");
})
</script>
<?php include_once 'footer.inc'; ?>
