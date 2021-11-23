#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "helpers/helpers.h"
#include "command/command.h"

void validate_args(int argc, char** argv);
char* receive_message(char** message, unsigned int size);

char ipv4[] = "127.0.0.1";
char ipv6[] = "::1";

int main(int argc, char** argv) {
  server_validate_args(argc, argv);

  char* ip = strcmp(argv[1], "v6") == 0 ? ipv6 : ipv4;
  unsigned int port = (unsigned int) atoi(argv[2]);

  command_start(ip, port);
  command_register("banana", receive_message);
  command_listen();

  return 0;
}

char* receive_message(char** message, unsigned int size) {
  char* response = malloc(sizeof(char) * 32);
  strncpy(response, "message received", 17);
  for (int i = 0; i < size; i++) {
    printf("%s\n", message[i]);
  }
  return response;
}
