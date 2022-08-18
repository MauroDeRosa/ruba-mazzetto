#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdbool.h>

bool is_token_empty();
bool if_token_empty_send_failed();
void parse_request(char *message);
void parse_request_user();
void parse_request_game();
void parse_request_history();
void parse_request_statistics();

#endif /* __PARSER_H__ */
