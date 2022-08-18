let username = null;
let players = null;
let matches = Array(3);
let wins = Array(3);
let losses = Array(3);
let win_rate = Array(3);
let max_win_streak = Array(3);
let min_points = Array(3);
let max_points = Array(3);
let avg_points = Array(3);
let min_steals = Array(3);
let max_steals = Array(3);
let avg_steals = Array(3);
let pie_charts = Array(3);
let charts = Array(3);






function fetch() {
    $.ajax({
        url: `/api/statistics/${id}`,
        success: function (obj) {
            username.html(obj.data.username)
            if (
                obj.result == 'success' &&
                obj.type == 'json') {
                players = obj.data.statistics;
                render();
            }
        }
    });
}

function render() {
    players.forEach((stats, index) => {
        matches[index].html(stats.matches);
        wins[index].html(stats.wins);
        losses[index].html(stats.losses);
        max_win_streak[index].html(stats.max_win_streak);
        win_rate[index].html(stats.win_rate + '%');
        min_points[index].html(stats.min_points);
        max_points[index].html(stats.max_points);
        avg_points[index].html(stats.avg_points);
        min_steals[index].html(stats.min_steals);
        max_steals[index].html(stats.max_steals);
        avg_steals[index].html(stats.avg_steals);
        charts[index].data.datasets[0].data = [stats.wins, stats.losses];
        charts[index].update();
    });
}



$(document).ready(function () {
    username = $('#username');

    for (let index = 0; index < 3; index++) {
        matches[index] = $('#matches' + index);
        wins[index] = $('#wins' + index);
        losses[index] = $('#losses' + index);
        max_win_streak[index] = $('#max_win_streak' + index);
        win_rate[index] = $('#win_rate' + index);
        min_points[index] = $('#min_points' + index);
        max_points[index] = $('#max_points' + index);
        avg_points[index] = $('#avg_points' + index);
        min_steals[index] = $('#min_steals' + index);
        max_steals[index] = $('#max_steals' + index);
        avg_steals[index] = $('#avg_steals' + index);
        pie_charts[index] = $('#pie_chart' + index);
        charts[index] = new Chart(
            pie_charts[index], {
                type: 'doughnut',
                data: {
                    labels: [
                        'Wins',
                        'Losses'
                    ],
                    datasets: [{
                        data: [0,0],
                        backgroundColor: ['rgb(0, 0, 255)', 'rgb(255, 0, 0)'],
                    }]},
                options: { plugins: {legend: {display: false}}}
            });
    }

    fetch();
});