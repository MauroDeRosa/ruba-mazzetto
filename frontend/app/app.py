import webview
from app.web_server import WebServer
from app.game_client import game_client

import app.api
import app.pages

class App:
    
    def __init__(self, title:str, server:WebServer) -> None:
        self.server:WebServer = server
        self.server.app = self
        self.window = webview.create_window(title=title, url='http://localhost:8080/')
        self.window._js_api = server.js_api
        server.js_api.set_window(self.window)
        
    def start(self):
        while not game_client.connect():
            pass
        game_client.recv() # TODO: fix empty message at connection
        self.server.start()
        webview.start(debug=True, gui='gtk')