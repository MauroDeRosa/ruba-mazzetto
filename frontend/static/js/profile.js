let p = null;
let username = $('#username');
let created = $('#created');
let modified = $('#modified');
let password1 = $('#password1');
let password2 = $('#password2');
let message = $('#message');

function fetch() {
    $.ajax({
        url: `/api/user/get/${id}`,
        success: function (obj) {
            if(obj.result == 'success' && obj.type == 'json')
            {
                p = obj.data;
                render();
            }
        }
    });
}

function render() {
    username.html(p.username);
    created.html(p.created);
    modified.html(p.modified);
}

function on_change_password() {
    if (password1.val() != password2.val() || password1.val().length < 1) {
        message.html('passwords must be equal and can\'t be empty');
        return;
    }


    $.ajax({
        url: `/api/user/update/${id}/${password1.val()}`,
        success: function (obj) {
            if(obj.result == 'success' && obj.type == 'json')
            {
                message.html('password updated');
                setTimeout(()=>{ message.html(''); }, 1000)
                password1.val('');
                password2.val('');
                fetch();
            }
            else if(obj.result == 'failed' && obj.type == 'message')
            {
                message.html(obj.message);
            }
        }
    });
}

function on_back() {
    $.ajax({
        url: `/api/user/logout/${id}`,
        success: function (response) {
            window.location = '/';
        }
    });

}

$(document).ready(fetch);