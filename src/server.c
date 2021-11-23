#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "helpers/helpers.h"
#include "socket/socket.h"

void validate_args(int argc, char** argv);

char* receive_message(char* message) {
  char* response = malloc(sizeof(char) * 32);
  strncpy(response, "message received", 17);
  printf("%s\n", message);
  return response;
}

int main(int argc, char** argv) {
  validate_args(argc, argv);
  unsigned int port = (unsigned int) atoi(argv[2]);
  socket_t socket = socket_create(argv[1], port);
  socket_listen(socket, receive_message);

  return 0;
}

void usage(char** argv) {
  fprintf(stderr, "Usage: %s <IPv4|IPv6> <PORT>\n", argv[0]);
}

void validate_args(int argc, char** argv) {
  if (argc != 3) {
    usage(argv);
    exit(EXIT_FAILURE);
  }

  if (!is_numeric_string(argv[2])) {
    fprintf(stderr, "Port must be a positive number!\n");
    usage(argv);
    exit(EXIT_FAILURE);
  }

  unsigned int port = (unsigned int) atoi(argv[2]);
  if (port < 1024 || port > 65535) {
    fprintf(stderr, "Port is not in a valid range!\n");
    usage(argv);
    exit(EXIT_FAILURE);
  }
}
