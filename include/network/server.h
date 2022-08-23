#ifndef __SERVER_H__
#define __SERVER_H__

/**
 * @file server.h
 * @author Mauro De Rosa (it.mauro.derosa@gmail.com)
 * @brief local server for IPC between backend and frontend
 * @version 1.0.0 alpha
 * @date 2022-08-23
 * 
 * @copyright Copyright (c) 2022
 * @license licensed under MIT license
 * 
 */

#include <stdbool.h>

#define SRV_PORT_NUMBER 11666 ///< server socket port
#define SRV_BUFF_SIZE 8192 ///< server buffer size (used for client and server messages)

extern bool server_quit; ///< used to check if the server must close or continue polling messages from the connected client
extern char server_message[SRV_BUFF_SIZE], ///< server message buffer
            client_message[SRV_BUFF_SIZE]; ///< client message buffer

/**
 * @brief receive a message from the connected client
 */
void server_recv();

/**
 * @brief send a message to the connected client
 * 
 * @param message the message to send
 */
void server_send(const char *message);

/**
 * @brief initialize server and wait for client connection
 */
void server_init();

/**
 * @brief closes the socket connection
 */
void server_close();

#endif /* __SERVER_H__ */
