#ifndef __SERVER_H__
#define __SERVER_H__

#include <stdbool.h>

#define SRV_PORT_NUMBER 11666
#define SRV_BUFF_SIZE 4096

extern bool server_quit;
extern char server_message[SRV_BUFF_SIZE], client_message[SRV_BUFF_SIZE];

void server_recv();
void server_send(const char *message);
void server_init();
void server_close();

#endif /* __SERVER_H__ */
