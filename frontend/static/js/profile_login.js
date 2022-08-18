let username = $('#username');
let password = $('#password');
let message = $('#message');

function on_login() {
    if(username.val().length < 1 || password.val().length < 1)
    {
        message.html('fields can\'t be empty');
        return;
    }

    $.ajax({
        url: `/api/user/login/${username.val()}/${password.val()}`,
        success: function (obj) {
            if (obj.result == 'success' && obj.type == 'json')
            {
                window.location = `/profile/${obj.data.id}`;
            }
            else if (obj.result == 'failed' && obj.type == 'message')
            {
                message.html(obj.message);   
            }
        }
    });
}