let entries_div = null;
let controls_div = null;
let username = null;
let page = 1;
let entries = null;
let entries_pages = null;

function fetch() {
    $.ajax({
        url: `/api/history/${id}/${page}`,
        success: function (response) {
            entries = response.data.entries;
            entries_pages = response.data.pages;
            username.html(response.data.username)
            
            if(entries.length > 0)
            {
                render();
            }
        }
    });
}

function render() {
    entries_div.empty();
    entries.forEach(entry => {
        let date = entry.timestamp.split(' ')[0];
        let time = entry.timestamp.split(' ')[1];


        entries_div.append(
            `<div class="row">
                <div class="col-3">
                    <label">${date}</label><br>
                    <label">${time}</label>
                </div>
                <label class="col-2">${'<i class="fa fa-user" aria-hidden="true"></i>'.repeat(entry.players)}</label>
                <label class="col-2">${entry.points}</label>
                <label class="col-2">${entry.steals}</label>
                <label class="col-1"></label>
                <label class="col-2"><i class="fa fa-trophy" style="color:${entry.won ? 'yellow' : 'darkgray'}!important;" aria-hidden="true"></i></label>
            </div>`);

        });
        
        controls_div.empty()

        if(page > 1)
        {
            controls_div.append('<a class="button col-4" onclick="page--; fetch();"><i class="fa-solid fa-arrow-left"></i></a>')
        }
        else
        {
            controls_div.append('<div class="col-4"></div>')
        }

        if(page < entries_pages )
        {
            controls_div.append('<a class="button col-4" onclick="page++; fetch();"><i class="fa-solid fa-arrow-left"></i></a>')
        }
        else
        {
            controls_div.append('<div class="col-4"></div>')
        }

        controls_div.append('<a class="button col-4" onclick="history.back()">Back</a>')

}

$(document).ready(function () {
    entries_div = $('#entries_div');
    controls_div = $('#controls_div');
    username = $('#username');
    fetch();
});