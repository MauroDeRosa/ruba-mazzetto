#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include <io/log.h>
#include <network/server.h>
#include <network/parser.h>
#include <game/user.h>
#include <stdlib.h>

bool server_quit = false;

typedef __socklen_t socklen;

int server_sock, client_sock;
socklen client_size;
struct sockaddr_in server_addr, client_addr;
char server_message[SRV_BUFF_SIZE], client_message[SRV_BUFF_SIZE];

void server_recv()
{
    memset(client_message, 0, SRV_BUFF_SIZE);
    if (recv(client_sock, client_message, SRV_BUFF_SIZE, 0) < 0)
    {
        log_error("Socket couldn't receive message from the connected client");
    }
    log_debug("client says %s", client_message);
}

void server_send(const char *message)
{
    memset(server_message, 0, SRV_BUFF_SIZE);

    if (strlen(message) >= SRV_BUFF_SIZE)
    {
        log_error("message should have a maximum of %zu characters including '\0'");
    }

    strcpy(server_message, message);
    server_message[SRV_BUFF_SIZE - 1] = '\0';

    if (send(client_sock, server_message, strlen(server_message), 0) < 0)
    {
        log_error("Socket can't send message back to the client");
    }
    log_debug("Message sent: %s", server_message);
}

void server_init()
{
    // Create socket:
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    int so_reuseaddr = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &so_reuseaddr, sizeof(so_reuseaddr));

    if (server_sock < 0)
    {
        log_error("Error while creating socket");
    }
    log_info("Socket created successfully");

    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SRV_PORT_NUMBER);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Bind to the set port and IP:
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        log_error("Couldn't bind to the port %zu", SRV_PORT_NUMBER);
    }
    log_info("Socket successfully bound to 127.0.0.1:%zu", SRV_PORT_NUMBER);

    // Listen for clients:
    if (listen(server_sock, 1) < 0)
    {
        log_error("Socket error while listening");
    }
    log_info("Socket listening for incoming connections...");

    // Accept an incoming connection:
    client_size = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_size);

    if (client_sock < 0)
    {
        log_error("Socket can't accept connection.");
    }
    log_info("Socket connected to client at IP: %s and port: %i", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
}

void server_close()
{
    close(client_sock);
    close(server_sock);
}