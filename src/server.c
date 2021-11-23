#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "helpers/helpers.h"
#include "socket/socket.h"

void validate_args(int argc, char** argv);
char* receive_message(char* message);

int main(int argc, char** argv) {
  validate_args(argc, argv);
  char ip[] = "127.0.0.1";

  if (strcmp(argv[1], "v6") == 0) {
    strncpy(ip, "::1", 4);
  }

  unsigned int port = (unsigned int) atoi(argv[2]);
  socket_t socket = socket_create(ip, port);
  socket_listen(socket, receive_message);

  return 0;
}

char* receive_message(char* message) {
  char* response = malloc(sizeof(char) * 32);
  strncpy(response, "message received", 17);
  printf("%s\n", message);
  return response;
}

void validate_args(int argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <v4|v6> <PORT>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if (strcmp(argv[1], "v4") != 0 && strcmp(argv[1], "v6") != 0) {
    fprintf(stderr, "Protocol must be \"v4\" or \"v6\"!\n");
    fprintf(stderr, "Usage: %s <v4|v6> <PORT>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if (!is_numeric_string(argv[2])) {
    fprintf(stderr, "Port must be a positive number!\n");
    fprintf(stderr, "Usage: %s <v4|v6> <PORT>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  unsigned int port = (unsigned int) atoi(argv[2]);
  if (port < 1024 || port > 65535) {
    fprintf(stderr, "Port is not in a valid range!\n");
    fprintf(stderr, "Usage: %s <v4|v6> <PORT>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
}
