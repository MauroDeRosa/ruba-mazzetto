#ifndef __PARSER_H__
#define __PARSER_H__

/**
 * @file parser.h
 * @author Mauro De Rosa (it.mauro.derosa@gmail.com)
 * @brief client requests parser for IPC
 * @version 0.1
 * @date 2022-08-23
 * 
 * @copyright Copyright (c) 2022
 * @license licensed under MIT license
 * 
 */

#include <stdbool.h>

/**
 * @brief used by the server on each received message, parses the request (space separated strings) and sends back a json response
 * 
 * @param message the message received from the client
 */
void parse_request(char *message);

#endif /* __PARSER_H__ */
