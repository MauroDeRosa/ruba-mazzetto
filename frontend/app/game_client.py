import socket

SERVER_HOSTNAME = 'localhost'
SERVER_PORT = 11666
SERVER_BUFFER_SIZE = 4096

class GameClient:
    
    def __init__(self) -> None:
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        
            
    def connect(self) -> bool:
        try:
            self.socket.connect((SERVER_HOSTNAME, SERVER_PORT))
            print("connected to {:s}".format(repr((SERVER_HOSTNAME, SERVER_PORT))))
            self.socket.send("connected".encode())
            return True
        except:
            return False
        
    def send(self, message:str):
        if(len(message) >= SERVER_BUFFER_SIZE):
            print("message is too long")
        else:
            self.socket.send(message.encode())
        
    def recv(self) -> str:
        return self.socket.recv(SERVER_BUFFER_SIZE).decode()
    
    def close(self):
        self.socket.close()
        
game_client = GameClient()