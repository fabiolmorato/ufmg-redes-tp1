#ifndef SOCKET_H_INCLUDED
#define SOCKET_H_INCLUDED

typedef struct {
  int socket;
  struct sockaddr* addr;
} socket_t;

socket_t socket_create(char* address, unsigned int port);
void socket_listen(socket_t socket, char* (*handler)(char* message));
void socket_send_message(char* address, unsigned int port, char* message, void (*callback)(char*));

#endif
