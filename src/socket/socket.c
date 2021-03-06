#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#include "socket.h"

#define BUFSZ 1024

socket_t socket_create(char* address, unsigned int port_as_int) {
  struct sockaddr_storage storage;
  int s; // socket
  int enable;
  struct sockaddr* addr = NULL;
  uint16_t port = htons(port_as_int);
  socket_t return_socket;

  struct in_addr inaddr4;
  if (inet_pton(AF_INET, address, &inaddr4)) {
    struct sockaddr_in* addr4 = (struct sockaddr_in*) (&storage);
    addr4->sin_family = AF_INET;
    addr4->sin_port = port;
    addr4->sin_addr = inaddr4;
  }

  struct in6_addr inaddr6;
  if (inet_pton(AF_INET6, address, &inaddr6)) {
    struct sockaddr_in6* addr6 = (struct sockaddr_in6*) (&storage);
    addr6->sin6_family = AF_INET6;
    addr6->sin6_port = port;
    memcpy(&(addr6->sin6_addr), &inaddr6, sizeof(inaddr6));
  }

  s = socket(storage.ss_family, SOCK_STREAM, 0);
  if (s == -1) {
    fprintf(stderr, "Could not create socket!\n");
    exit(EXIT_FAILURE);
  }

  if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) != 0) {
    fprintf(stderr, "Could not configure socket!\n");
    exit(EXIT_FAILURE);
  }

  addr = (struct sockaddr*) (&storage);
  if (bind(s, addr, sizeof(storage)) != 0) {
    fprintf(stderr, "Could not bind to socket!\n");
    exit(EXIT_FAILURE);
  }

  return_socket.socket = s;
  return_socket.addr = addr;
  return return_socket;
}

void addrtostr(const struct sockaddr* addr, char* str, size_t strsize) {
  int version;
  char addrstr[INET6_ADDRSTRLEN + 1] = "";
  uint16_t port = 0;

  if (addr->sa_family == AF_INET) {
    version = 4;
    struct sockaddr_in *addr4 = (struct sockaddr_in *)addr;
    inet_ntop(AF_INET, &(addr4->sin_addr), addrstr, INET6_ADDRSTRLEN + 1);
    port = ntohs(addr4->sin_port); 
  } else if (addr->sa_family == AF_INET6) {
    version = 6;
    struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *)addr;
    inet_ntop(AF_INET6, &(addr6->sin6_addr), addrstr, INET6_ADDRSTRLEN + 1);
    port = ntohs(addr6->sin6_port); 
  }

  if (str && port != 0) {
    snprintf(str, strsize, "IPv%d %s %hu", version, addrstr, port);
  }
}

void socket_listen(socket_t socket, char* (*handler)(char* message)) {
  int s = socket.socket;
  struct sockaddr* addr = socket.addr;
  listen(s, 10);

  char addrstr[BUFSZ];
  addrtostr(addr, addrstr, BUFSZ);
  
  for (;;) {
    struct sockaddr_storage cstorage;
    struct sockaddr* caddr = (struct sockaddr*) (&cstorage);
    socklen_t caddrlen = sizeof(cstorage);

    int csock = accept(s, caddr, &caddrlen);
    if (csock == -1) {
      fprintf(stderr, "Could not accept connection!\n");
    }

    char caddrstr[BUFSZ];
    addrtostr(caddr, caddrstr, BUFSZ);

    char* buf = malloc(BUFSZ * sizeof(char));

    for (;;) {
      size_t len = 0;
      unsigned int total = 0;
      memset(buf, 0, BUFSZ);

      for (;;) {
        len = recv(csock, buf + total, BUFSZ - 1 - total, 0);
        if (len == 0) break;
        total += len;
        if (total > 0 && buf[total - 1] == '\n') break;
      }

      len = strlen(buf);
      if (len == 0) break;
      if (buf[len - 1] == '\n' || buf[len - 1] == '\r') buf[len - 1] = '\0';

      char* response = handler(buf);

      if (response != NULL) {
        size_t response_len = strlen(response);
        response[response_len] = '\n';
        send(csock, response, response_len + 1, 0);
        free(response);
      }
    }

    close(csock);
  }
}

void socket_connect(char* address, unsigned int port_as_int, char* (*get_message)(void), void (*callback)(char*)) {
  struct sockaddr_storage storage;
  int s; // socket
  uint16_t port = htons(port_as_int);

  struct in_addr inaddr4;
  if (inet_pton(AF_INET, address, &inaddr4)) {
    struct sockaddr_in* addr4 = (struct sockaddr_in*) (&storage);
    addr4->sin_family = AF_INET;
    addr4->sin_port = port;
    addr4->sin_addr = inaddr4;
  }

  struct in6_addr inaddr6;
  if (inet_pton(AF_INET6, address, &inaddr6)) {
    struct sockaddr_in6* addr6 = (struct sockaddr_in6*) (&storage);
    addr6->sin6_family = AF_INET6;
    addr6->sin6_port = port;
    memcpy(&(addr6->sin6_addr), &inaddr6, sizeof(inaddr6));
  }

  s = socket(storage.ss_family, SOCK_STREAM, 0);
  if (s == -1) {
    fprintf(stderr, "Could not create socket!\n");
    exit(EXIT_FAILURE);
  }

  struct sockaddr *addr = (struct sockaddr *)(&storage);
	if (connect(s, addr, sizeof(storage)) != 0) {
		fprintf(stderr, "Could not connect to socket!\n");
    exit(EXIT_FAILURE);
	}

  char* response = malloc(1024 * sizeof(char));

  for (;;) {
    char* message = get_message();
    memset(response, 0, 1024);

    send(s, message, strlen(message), 0);
    free(message);

    recv(s, response, 1024, 0);

    callback(response);
  }

  close(s);
}
