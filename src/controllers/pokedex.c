#include <stdio.h>
#include <stdlib.h>

#include "../services/pokedex.h"

char* pokedex_add(char** pokemon, unsigned int size) {
  return pokedex_service_add_pokemon(pokemon, size);
}

char* pokedex_list(char** arguments, unsigned int size) {
  if (size > 0) {
    char* response = malloc(1024 * sizeof(char));
    sprintf(response, "list command takes no positional arguments!");
    return response;
  }

  return pokedex_service_list_pokedex();
}
