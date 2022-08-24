/**
 * @file main.c
 * @author Mauro De Rosa (it.mauro.derosa@gmail.com)
 * @brief program starting point
 * @version 1.0.0 alpha
 * @date 2022-08-24
 * 
 * @copyright Copyright (c) 2022
 * @license licensed under MIT license
 * 
 */

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
    // initialize random seed to current time
    srand(time(0));
    
    // initialize vectors and persistent data
    users_init();
    history_init();
    statistics_init();
    game_saves_init();

    // launch frontend
    system("./gui &");

    // initialize server
    server_init();
    
    // until server mustn't quit receive a message from the client, parse the request message and send appropriate response 
    while (!server_quit)
    {
        server_recv();
        parse_request(client_message);
    }

    // close server on quit
    server_close();
    return 0;
}