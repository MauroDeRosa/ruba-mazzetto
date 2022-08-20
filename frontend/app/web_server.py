import threading
from flask import Flask
from flask import render_template, make_response
import json
from app.game_client import GameClient, game_client

class WebServer(Flask):
    
    def __init__(self, import_name):
        Flask.__init__(self, import_name)
        #self.config['TEMPLATES_AUTO_RELOAD'] = True
    
    def start(self):
        t = threading.Thread(target=lambda: self.run(host='0.0.0.0', port=8080))
        t.daemon = True
        t.start()
        
    def json_response(self, json:str):
        return self.response_class(
            response=json,
            status=200,
            mimetype='application/json'
        )

server = WebServer('__main__')
        