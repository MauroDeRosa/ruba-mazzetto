let players = Array(players_count);
let usernames = Array(players_count);
let passwords = Array(players_count);
let messages = Array(players_count);


$(document).ready(function () {
    for (let index = 0; index < players_count; index++) {
        players[index] =   $(`#player${index}`);
        usernames[index] = $(`#username${index}`);
        passwords[index] = $(`#password${index}`);
        messages[index] = $(`#message${index}`);
    }

    if(loading) {
        $.ajax({
            url: `/api/game/save/get/${load_index}`,
            success: function (obj) {
                if(obj.result == 'success' && obj.type == 'json')
                {
                    for (let index = 0; index < players_count; index++) {
                        usernames[index].val(obj.data.users[index].username)
                        usernames[index].attr('readonly', true);
                    }
                }
                else
                {
                    history.back();
                }
            }
        });
    }
});

function on_login(index) {
    let username = usernames[index].val();
    let password = passwords[index].val();

    if (username.length < 1 || password.length < 1) {
        return;
    }

    $.ajax({
        url: `/api/user/login/${username}/${password}`,
        success: function (obj) {
            if(obj.result == 'success')
            {
                players[index].empty();
                players[index].append(`<label class="col-12">Welcome, ${username}!</label>`);
                players[index].append(`<input class="col-12 user_ids" type="hidden" value="${obj.data.id}" />`);
            }
            else
            {
                messages[index].html(obj.type == 'message' ? obj.message : obj.data);
            }
        }
    });
}

function on_play() {
    let user_ids = $('.user_ids');
    
    if(user_ids.length == players_count)
    {
        if(loading)
        {
            let game_load_url = `/api/game/load/${load_index}/`;

            user_ids.each(function (index, value) { 
                game_load_url += value.value;
                game_load_url += (index == user_ids.length - 1) ? '' : '/';
            });

            for (let index = 0; index < (4 - players_count); index++) {
                game_load_url += '/0';
            }

            $.ajax({
                url: game_load_url,
                success: function (response) {
                    window.location = '/game'
                }
            });
        }
        else
        {
            let game_init_url = `/api/game/init/${players_count}/`
            
            user_ids.each(function (index, value) { 
                game_init_url += value.value;
                game_init_url += (index == user_ids.length - 1) ? '' : '/';
            });

            $.ajax({
                url: game_init_url,
                success: function (response) {
                    window.location = '/game'
                }
            });
        }
    }
}

function on_back() {
    $('.user_ids').each(function (index, value) { 
        $.ajax({url: `/api/user/logout/${value.value}`})        
    });
    window.location = '/';
}