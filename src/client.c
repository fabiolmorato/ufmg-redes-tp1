#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "helpers/helpers.h"
#include "socket/socket.h"

void handle_response(char* message);
char* read_input(void);

int main(int argc, char** argv) {
  client_validate_args(argc, argv);

  char* ip = argv[1];
  unsigned int port = (unsigned int) atoi(argv[2]);

  socket_connect(ip, port, read_input, handle_response);
}

void handle_response(char* message) {
  if (strcmp(message, "kill") == 0) {
    exit(EXIT_FAILURE);
  }

  size_t len = strlen(message);
  message[len - 1] = '\0';

  printf("%s\n", message);
}

char* read_input(void) {
  char* buf = malloc(1024 * sizeof(char));
  fgets(buf, 1024, stdin);
  size_t len = strlen(buf);
  buf[len - 1] = '\n';
  return buf;
}
