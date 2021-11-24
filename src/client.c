#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "helpers/helpers.h"
#include "socket/socket.h"

void handle_response(char* message) {
  if (strcmp(message, "kill") == 0) {
    exit(EXIT_FAILURE);
  }

  printf("%s\n", message);
}

int main(int argc, char** argv) {
  client_validate_args(argc, argv);

  char* ip = argv[1];
  unsigned int port = (unsigned int) atoi(argv[2]);

  char buf[1024];

  for(;;) {
    fgets(buf, 1024, stdin);
    size_t command_len = strlen(buf);
    buf[command_len - 1] = '\0';

    socket_send_message(ip, port, buf, handle_response);
  }
}
