
$(function() {
    $('[data-toggle="popover"]').popover();
    $('[data-toggle="tooltip"]').tooltip();

//    $('input').iCheck({
//        checkboxClass: 'icheckbox_square-blue',
//        radioClass: 'iradio_square-blue',
//        increaseArea: '20%' // optional
//      });

    $('input').iCheck({
        checkboxClass: 'icheckbox_flat-blue',
        radioClass: 'iradio_flat-blue',
        increaseArea: '20%' // optional
    });

    $("a[href=#myModal]").click(function() {
        $("#myModal").load($(this).attr("data-url"));
    });
})

function showMessage(messages, css) {
    $('#messages').html(messages);//淡入
    $('#messages').removeClass();
    $('#messages').addClass(css);
    $('#messages').addClass("alert m_danger");
    $('#messages').fadeIn(500);//淡入
    setTimeout(function() {
        $('#messages').fadeOut(500);
    }, 3000);
    exit;
}

function showMessage_noexit(messages, css) {
    $('#messages').html(messages);//淡入
    $('#messages').removeClass();
    $('#messages').addClass(css);
    $('#messages').addClass("alert m_danger");
    $('#messages').fadeIn(500);//淡入
    setTimeout(function() {
        $('#messages').fadeOut(500);
    }, 3000);
}

function showMessage_reload(messages, css) {
    $('#messages').html(messages);
    $('#messages').removeClass();
    $('#messages').addClass(css);
    $('#messages').fadeIn(500);//淡入
    setTimeout(function () {
        location.reload();
    }, 500);
}

