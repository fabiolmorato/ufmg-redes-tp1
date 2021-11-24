#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "../services/pokedex.h"

bool command_validator_middleware(char** params, unsigned int size, char** response) {
  for (unsigned int i = 0; i < size; i++) {
    char* word = params[i];

    if (!is_valid_pokemon_name(word)) {
      strncpy(*response, "invalid message", 16);
      return false;
    }
  }

  return true;
}
