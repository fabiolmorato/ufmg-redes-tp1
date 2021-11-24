#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "helpers/helpers.h"
#include "command/command.h"
#include "middlewares/command-validator.h"

#include "controllers/pokedex.h"

char ipv4[] = "127.0.0.1";
char ipv6[] = "::1";

char* kill(char** arguments, unsigned int size);

int main(int argc, char** argv) {
  server_validate_args(argc, argv);

  char* ip = strcmp(argv[1], "v6") == 0 ? ipv6 : ipv4;
  unsigned int port = (unsigned int) atoi(argv[2]);

  command_start(ip, port);

  // this middleware invalidates the whole command if message does not follow specified properties, but this shouldn't be the case unfortunately
  // command_add_middleware(command_validator_middleware);
  command_register("add", pokedex_add);
  command_register("list", pokedex_list);
  command_register("remove", pokedex_remove);
  command_register("exchange", pokedex_exchange);
  command_register("kill", kill);

  command_listen();

  return 0;
}

char* kill(char** arguments, unsigned int size) {
  exit(EXIT_SUCCESS);
  return NULL;
}
