
from flask import make_response, render_template
from app.game_client import game_client
from app.web_server import server
import json

@server.route('/')
def page_home():
    return render_template('home.html')

@server.route('/game/new/<players_count>')
def page_game_new(players_count):
    return render_template('game_login.html', context={
        "players_count": int(players_count)
    })
    
@server.route('/game/load/<players_count>/<index>/')
def page_game_load(players_count, index):
    return render_template('game_login.html', context={
        "players_count": int(players_count),
        "load_index": int(index)
    })
    
@server.route('/load')
def page_load_game():
    return render_template('load_game.html', context={})

@server.route('/register')
def page_register():
    return render_template('register.html')

@server.route('/login')
def page_login():
    return render_template('profile_login.html')

@server.route('/profile/<int:id>') 
def page_profile(id):
    return render_template('profile.html', context={"id":id})

@server.route('/game')
def page_game():
    return render_template('game.html')
    
@server.route('/history/<int:id>/')
def page_history(id):
    return render_template('history.html', context={"id": id})

@server.route('/statistics/<int:id>/')
def page_statistics(id):
    return render_template('statistics.html', context={"id": id})


@server.route('/leaderboard/<int:category>/')
def page_leaderboard(category):
    return render_template('leaderboard.html', context={"category": category})