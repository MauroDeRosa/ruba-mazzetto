from time import sleep
from webview import Window
from app.game_client import game_client

class JavascriptAPI:
    
    def __init__(self):
        self._window:Window = None
        self.vars:dict = dict()
        self.vars['__current_route'] = '/'

    def set_window(self, window:Window):
        self._window = window
    
    def route(self, path):
        val = None
        
        print(f'---routing request: "{path}"')
        
        if path == 'last':
            val = self.vars.get('__last_route')
        elif path == 'target':
            val = self.vars.get('__target_route')
        else:
            val = path
        
        print(f'---routing to: "{val}"')
        
        if val == None:
            return
        
        self.vars["__last_route"] = self.vars.get("__current_route")
        self.vars["__current_route"] = val
        sleep(0.3)
        self._window.load_url(f'http://localhost:8080{val}')
        
    def varset(self, name, value):
        print(f'vars[\'{name}\'] <= {value}')
        self.vars[name] = value
        return {}
        
    def varget(self, name):
        print(f'vars[\'{name}\'] => {self.vars.get(name)}')
        return self.vars.get(name)
    
    def target(self, path):
        self.vars['__target_route'] = path
        print(f'---target set to: {self.vars["__target_route"]}')
        return {}
        
    def backend(self, message:str) -> str:
        game_client.send(message)
        return game_client.recv()
    
    def destroy(self):
        game_client.send('quit')
        self._window.destroy()