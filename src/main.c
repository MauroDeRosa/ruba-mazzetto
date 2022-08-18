#include <io/log.h>
#include <game/user.h>
#include <game/history.h>
#include <game/statistics.h>
#include <network/server.h>
#include <network/parser.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    srand(time(0));
    
    users_init();
    history_init();
    statistics_init();
    game_saves_init();

    system("sh ../frontend/launch.sh &");

    server_init();
    
    while (!server_quit)
    {
        server_recv();
        parse_request(client_message);
    }

    server_close();
    return 0;
}