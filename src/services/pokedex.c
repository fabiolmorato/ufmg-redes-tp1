#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../list/list.h"
#include "pokedex.h"

list_t* pokedex = NULL;

void start_pokedex_if_necessary(void) {
  if (pokedex == NULL) {
    pokedex = list_create();
  }
}

int pokemon_index(char* desired) {
  pokemon_t** pokemons = (pokemon_t**) pokedex->values;

  for (unsigned int i = 0; i < pokedex->length; i++) {
    pokemon_t* pokemon = pokemons[i];
    if (strcmp(pokemon->name, desired) == 0) {
      return i;
    }
  }

  return -1;
}

char* pokedex_service_add_pokemon(char** pokemon, unsigned int size) {
  start_pokedex_if_necessary();
  char* response = malloc(1024 * sizeof(char));
  unsigned int response_index = 0;

  for (unsigned int i = 0; i < size; i++) {
    char* current_pokemon = pokemon[i];

    int index = pokemon_index(current_pokemon);

    if (pokedex->length >= 40) {
      unsigned int size = sprintf(response + response_index, "limit exceeded ");
      response_index += size;
    }

    if (index > -1) {
      unsigned int size = sprintf(response + response_index, "%s already exists ", current_pokemon);
      response_index += size;
    } else {
      pokemon_t* new_pokemon = malloc(sizeof(pokemon_t));
      size_t new_pokemon_name_len = strlen(current_pokemon);
      new_pokemon->name = malloc((new_pokemon_name_len + 1) * sizeof(char));
      strncpy(new_pokemon->name, current_pokemon, new_pokemon_name_len + 1);

      list_append(pokedex, new_pokemon, sizeof(pokemon_t*));

      unsigned int size = sprintf(response + response_index, "%s added ", current_pokemon);
      response_index += size;
    }
  }

  response[response_index - 1] = '\0';
  return response;
}

char* pokedex_service_list_pokedex(void) {
  start_pokedex_if_necessary();
  char* response = malloc(1024 * sizeof(char));
  unsigned int response_index = 0;
  pokemon_t** pokemon = (pokemon_t**) pokedex->values;

  for (unsigned int i = 0; i < pokedex->length; i++) {
    unsigned int size = sprintf(response + response_index, "%s ", pokemon[i]->name);
    response_index += size;
  }

  if (response_index == 0) {
    unsigned int size = sprintf(response, "none ");
    response_index += size;
  }

  response[response_index - 1] = '\0';
  return response;
}
