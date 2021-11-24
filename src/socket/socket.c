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

    char buf[BUFSZ];
    memset(buf, 0, BUFSZ);

    recv(csock, buf, BUFSZ - 1, 0);

    char* response = handler(buf);
    if (response != NULL) {
      size_t response_len = strlen(response);
      send(csock, response, response_len + 1, 0);
      free(response);
    }

    send(csock, buf, strlen(buf) + 1, 0);
    close(csock);
  }
}

void socket_send_message(char* address, unsigned int port_as_int, char* message, void (*callback)(char*)) {
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

	size_t count = send(s, message, strlen(message) + 1, 0);
  unsigned int total = 0;

  for (;;) {
		count = recv(s, response + total, 1024 - total, 0);
		if (count == 0) {
			break;
		}
    total += count;
	}

	close(s);

  callback(response);
}
