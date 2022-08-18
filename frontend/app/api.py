
from app.game_client import GameClient, game_client
from app.web_server import server

#######################
#     USER ROUTES     #
#######################

@server.route('/api/user/register/<username>/<password>')
def api_user_register(username, password):
    game_client.send(f'user register {username} {password}')
    return server.json_response(game_client.recv())
    
@server.route('/api/user/login/<username>/<password>')
def api_user_login(username, password):
    game_client.send(f'user login {username} {password}')
    return server.json_response(game_client.recv())

@server.route('/api/user/logout/<int:userid>')
def api_user_logout(userid):
    game_client.send(f'user logout {userid}')
    return server.json_response(game_client.recv())

@server.route('/api/user/delete/<int:userid>')
def api_user_delete(userid):
    game_client.send(f'user delete {userid}')
    return server.json_response(game_client.recv())

@server.route('/api/user/update/<int:userid>/<password>')
def api_user_update(userid, password):
    game_client.send(f'user update {userid} {password}')
    return server.json_response(game_client.recv())

@server.route('/api/user/get/<int:userid>')
def api_user_get(userid):
    game_client.send(f'user get {userid}')
    return server.json_response(game_client.recv())

#######################
#    GAME ROUTES      #
#######################

@server.route('/api/game/init/2/<int:user1>/<int:user2>')
def api_game_init_2p(user1, user2):
    game_client.send(f'game init 2 {user1} {user2}')
    return server.json_response(game_client.recv())

@server.route('/api/game/init/3/<int:user1>/<int:user2>/<int:user3>')
def api_game_init_3p(user1, user2, user3):
    game_client.send(f'game init 3 {user1} {user2} {user3}')
    return server.json_response(game_client.recv())

@server.route('/api/game/init/4/<int:user1>/<int:user2>/<int:user3>/<int:user4>')
def api_game_init_4p(user1, user2, user3, user4):
    game_client.send(f'game init 4 {user1} {user2} {user3} {user4}')
    return server.json_response(game_client.recv())

@server.route('/api/game/update')
def api_game_update():
    game_client.send(f'game update')
    return server.json_response(game_client.recv())

@server.route('/api/game/pick/table/<int:hand_index>/<int:table_index>')
def api_game_pick_table(hand_index, table_index):
    game_client.send(f'game pick table {hand_index} {table_index}')
    return server.json_response(game_client.recv())

@server.route('/api/game/pick/opponent/<int:hand_index>/<int:opponent_index>')
def api_game_pick_opponent(hand_index, opponent_index):
    game_client.send(f'game pick opponent {hand_index} {opponent_index}')
    return server.json_response(game_client.recv())

@server.route('/api/game/throw/<int:hand_index>')
def api_game_throw(hand_index):
    game_client.send(f'game throw {hand_index}')
    return server.json_response(game_client.recv())

@server.route('/api/game/save')
def api_game_save():
    game_client.send('game save')
    return server.json_response(game_client.recv())

@server.route('/api/game/savelist')
def api_game_savelist():
    game_client.send('game savelist')
    return server.json_response(game_client.recv())

@server.route('/api/game/save/get/<int:index>')
def api_game_save_get(index):
    game_client.send(f'game saveget {index}')
    return server.json_response(game_client.recv())

@server.route('/api/game/load/<int:id>/<int:user1>/<int:user2>/<int:user3>/<int:user4>')
def api_game_load(id, user1, user2, user3, user4):
    game_client.send(f'game load {id} {user1} {user2} {user3} {user4}')
    return server.json_response(game_client.recv())



#######################
#    HISTORY ROUTES   #
#######################

@server.route('/api/history/<int:userid>/<int:page>')
def api_history(userid, page):
    game_client.send(f'history get {userid} {page}')
    return server.json_response(game_client.recv())

#######################
#  STATISTICS ROUTES  #
#######################

@server.route('/api/statistics/<int:userid>')
def api_statistics(userid):
    game_client.send(f'statistics get {userid}')
    return server.json_response(game_client.recv())

@server.route('/api/leaderboard/<int:players_count>')
def api_leaderboard(players_count):
    game_client.send(f'statistics leaderboard {players_count}')
    return server.json_response(game_client.recv())

#######################
#         QUIT        #
#######################
@server.route('/api/quit')
def api_quit():
    game_client.send('quit')
    server.app.window.destroy()
    return server.json_response(game_client.recv())