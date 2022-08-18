let usernames_row = $("#usernames_row");
let decks_row = $("#decks_row");
let points_row = $("#points_row");
let table_row = $("#table_row");
let current_username_label = $("#current_username");
let hand_row = $("#hand_row");
let game_div = $("#game_div");
let hand_index = null;
let gd = null;
let interval_id = null;

function get_card(card_string) {
    return `${cards_path}${card_string}.png`;
}

function update() {
    if (gd.state == 'end') {
        $.ajax({
            url: "/api/game/update",
            success: function (obj) {
                if(obj.result == 'success')
                {
                    gd = obj.data;
                    render_end();
                    clearInterval(interval_id);
                }
            }
        });
    }
    else if (gd.state == 'waiting')
    {
    }
    else
    {
        $.ajax({
            url: "/api/game/update",
            success: function (obj) {
                if(obj.result == 'success')
                {
                    gd = obj.data;
                    render();
                }
            }
        });
    }
}

function render() {
        render_decks();
        render_table();
        render_hand();
}

function render_decks() {
    usernames_row.empty();
    decks_row.empty();
    points_row.empty();

    for (let index = 0; index < gd.players_count; index++) {
        usernames_row.append(`<label class="col-2">${gd.players[index].username}</label>`);
        decks_row.append(
            `<div class="col-2">
                <img class="card" src="${get_card(gd.players[index].deck.top)}" ${gd.turn != index && gd.players[index].deck.top != 'ZZ' ? 'onclick="pick_opponent(' + index + ');"' : ''}>
            </div>`);
        points_row.append(`<label class="col-2">points: ${gd.players[index].points}</label>`);
    }

    for (let index = 0; index < 5 - gd.players_count; index++) {
        usernames_row.append('<div class="col-2"></div>');
        decks_row.append('<div class="col-2"></div>');
        points_row.append('<div class="col-2"></div>');
    }
    usernames_row.append('<label class="col-2">Dealer</label>');
    decks_row.append(`<div class="col-2"><img class="card" src="${gd.dealer > 1 ? get_card('AA') : get_card('ZZ')}"></div>`);
    points_row.append('<label class="col-2">' + gd.dealer + '</label>');
}

function render_table() {
    table_row.empty();

    gd.table.forEach((card, index) => {
        table_row.append(
            `<div class="col-2">
            <img class="card" src="${get_card(card)}" ${hand_index != null ? 'onclick="pick_table(' + index + ');' : ''}">
            </div>`)
    });

    for (let index = 0; index < 6 - (gd.table.length % 6); index++) {
        table_row.append(
            `<div class="col-2">
            <img class="card" src="${get_card('ZZ')}" ${hand_index != null ? 'onclick="throw_table();' : ''
            }">
            </div>`)
    }
}

function render_hand() {
    current_username_label.html(`<b style='font-size:1.2em; color:darkorange!important;'>${gd.players[gd.turn].username}</b>'s turn`);
    hand_row.empty();

    gd.players[gd.turn].hand.forEach((card, index) => {
        hand_row.append(
            `<div class="col-4">
                <img class="card ${hand_index == index ? 'card-selected' : ''}" src="${get_card(card)}" ${gd.state == 'waiting' ? 'onclick="hand_select(' + index + ');"' : ''
            } alt="card">
            </div>`)
    });
}

function render_end() {
    game_div.empty();
    game_div.append(`
            <div class="bordered row" id="win_board">
                <div class="col-12 text-center separated"><label>Results</label></div>
                <div class="mb-1 text-center">
                    <label class="col-1">#</label>
                    <div class="col-1"></div>
                    <label class="col-4">Player</label>
                    <label class="col-3">Points</label>
                    <label class="col-3">Steals</label>
                </div>
            </div>
            <div class="row">
                <a class="col-12 button" onclick="on_quit();">Quit</a>
            </div>
        `);
    let win_board = $('#win_board');
    let players_sorted = [...gd.players];
    players_sorted.sort((a, b) => { return b.points - a.points; });

    for (let index = 0; index < gd.players_count; index++) {
        win_board.append(`
            <div class="text-center row">
                <label class="col-1">${index == 0 ?
                '<i class="fa-solid fa-crown" style="color:yellow!important;text-shadow: 0 0 5px #fff;"></i>' :
                '<b style="font-size:1.4em;text-shadow: 0 0 1px #aaa">' + (index + 1) + '</b>'}</label>
                <div class="col-1"></div>
                <label class="col-4">${players_sorted[index].username}</label>
                <label class="col-3">${players_sorted[index].points}</label>
                <label class="col-3">${players_sorted[index].steals}</label>
            </div>
            `);
    }
}

function hand_select(index) {
    $('.card-selected').removeClass('class-selected');
    hand_index = index;
    render();
}

function pick_table(table) {
    $.ajax({
        url: `/api/game/pick/table/${hand_index}/${table}`,
        success: function (obj) {
            hand_index = null;

            if(obj.result == 'success')
            {
                gd = obj.data;
            }
            render();
        }
    });
}

function pick_opponent(opponent) {
    $.ajax({
        url: `/api/game/pick/opponent/${hand_index}/${opponent}`,
        success: function (obj) {
            hand_index = null;

            if(obj.result == 'success')
            {
                gd = obj.data;
            }
            render();
        }
    });
}

function throw_table() {
    $.ajax({
        url: `/api/game/throw/${hand_index}`,
        success: function (obj) {
            hand_index = null;

            if(obj.result == 'success')
            {
                gd = obj.data;
            }
            render();
        }
    });
}

function on_save() {
    $.ajax({
        url: '/api/game/save',
        success: (response) => { on_quit(); }
    })
}

function on_quit() {
    gd.players.forEach((player) => {
        $.ajax({url: `/api/user/logout/${player.id}`,
            success: function (response) {
                window.location = '/';
            }
        });
    });
}

$(document).ready(() => {
    $.ajax({
        url: "/api/game/update",
        success: function (obj) {
            gd = obj.data;
            render();
            interval_id = setInterval(update, 300);
        }
    });
});