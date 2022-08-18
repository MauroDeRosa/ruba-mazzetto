let savelist = null;
let players2 = $('#players2');
let players3 = $('#players3');
let players4 = $('#players4');

function fetch() {
    console.log("here");
    $.ajax({
        url: "/api/game/savelist",
        success: function (obj) {
            if(obj.result == 'success' && obj.type == 'json')
            {
                savelist = obj.data;
                render();
            }
        }
    });
}

function render() {
    for (let j = 0; j < 3; j++) {
        let current = j==0 ? players2 : j==1 ? players3 : players4;
        for (let i = 0; i < savelist[j].length; i++) {
            const element = savelist[j][i];
            const date = element.timestamp.split(' ')[0];
            const time = element.timestamp.split(' ')[1];
            
            current.append(`
            <div class="row">
            <div class="col-2 text-center">
            <span style="font-size:0.85em;">${date}</span><br>
            <span style="font-size:0.85em;">${time}</span>
            </div>
            <label class="col-2">${element.users[0].username}</label>
            <label class="col-2">${element.users[1].username}</label>
            <label class="col-2">${j+2 >= 3 ? element.users[2].username : ''}</label>
            <label class="col-2">${j+2 == 4 ? element.users[3].username : ''}</label>
            <a class="button col-2" href="/game/load/${j+2}/${element.id}">load</a>
            </div>
            `);
        }
    }
}

$(document).ready(fetch);