let leaderboard = $('#leaderboard');
let lb = null;

function fetch() {
    $.ajax({
        url: `/api/leaderboard/${players_count}`,
        success: function (obj) {
            if (obj.result == 'success' && obj.type == 'json') {
                lb = obj.data;
                render();
            }
            else {
                console.log(obj.message);
            }
        }
    });
}

function render() {
    leaderboard.empty();
    lb.forEach((e, i) => {
        leaderboard.append(`
        <div class="row text-center ${i != lb.length -1 ? 'separated' : ''}">
            <label class="col-2">${e.username}</label>
            <label class="col-2">${e.data.wins}</label>
            <label class="col-2">${e.data.losses}</label>
            <label class="col-2">${e.data.win_rate}%</label>
            <a class="col-2 button" href="/statistics/${e.id}">stats</a>
            <a class="col-2 button" href="/history/${e.id}">history</a>
        </div>
        `);
    });
}

$(document).ready(function () {
    fetch();
});