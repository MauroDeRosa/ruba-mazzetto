let username = $('#username');
let password1 = $('#password1');
let password2 = $('#password2');
let message = $('#message');

function on_register() {
    if (username.val().length < 1) {
        message.html('username can\'t be null');
        return;
    }

    if (password1.val().length < 1) {
        message.html('password can\'t be null');
        return;
    }
    else if (password1.val() != password2.val()) {
        message.html('password are not equal');
        return;
    }

    $.ajax({
        url: `/api/user/register/${username.val()}/${password1.val()}`,
        success: function (obj) {
            if (obj.result == 'success') {
                window.location = '/';
            } else {
                message.html(obj.type == 'json' ? obj.data : obj.message);
            }
        }
    });
}