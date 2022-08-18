import threading
from app.web_server import server
from app.app import App


app = App("Ruba Mazzetto", server)

if __name__ == '__main__':
    app.start()
    