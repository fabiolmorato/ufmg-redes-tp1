#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

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

bool is_valid_pokemon_name(char* name) {
  size_t len = strlen(name);

  if (len > 10) return false;

  for (int i = 0; i < len; i++) {
    char c = name[i];

    if (!((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'))) {
      return false;
    }
  }

  return true;
}

char* pokedex_service_add_pokemon(char** pokemon, unsigned int size) {
  start_pokedex_if_necessary();
  char* response = malloc(1024 * sizeof(char));
  unsigned int response_index = 0;

  for (unsigned int i = 0; i < size; i++) {
    char* current_pokemon = pokemon[i];
    size_t new_pokemon_name_len = strlen(current_pokemon);

    int index = pokemon_index(current_pokemon);

    if (pokedex->length >= 40) {
      unsigned int size = sprintf(response + response_index, "limit exceeded ");
      response_index += size;
    } else if (index > -1) {
      unsigned int size = sprintf(response + response_index, "%s already exists ", current_pokemon);
      response_index += size;
    } else {
      pokemon_t* new_pokemon = malloc(sizeof(pokemon_t));
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

char* pokedex_service_remove_pokemon(char* pokemon) {
  start_pokedex_if_necessary();
  char* response = malloc(1024 * sizeof(char));
  
  int index = pokemon_index(pokemon);

  if (index == -1) {
    sprintf(response, "%s does not exist", pokemon);
    return response;
  }

  list_remove_at_index(pokedex, index);
  sprintf(response, "%s removed", pokemon);

  return response;
}

char* pokedex_service_exchange_pokemon(char* pokemon_out, char* pokemon_in) {
  start_pokedex_if_necessary();
  char* response = malloc(1024 * sizeof(char));

  int index_out = pokemon_index(pokemon_out);

  if (index_out == -1) {
    sprintf(response, "%s does not exist", pokemon_out);
    return response;
  }

  int index_in = pokemon_index(pokemon_in);

  if (index_in > -1) {
    sprintf(response, "%s already exists", pokemon_in);
    return response;
  }

  pokemon_t** pokemons = (pokemon_t**) pokedex->values;
  pokemon_t* pokemon = pokemons[index_out];

  free(pokemon->name);
  size_t pokemon_in_len = strlen(pokemon_in);
  pokemon->name = malloc((pokemon_in_len + 1) * sizeof(char));
  strncpy(pokemon->name, pokemon_in, pokemon_in_len + 1);

  sprintf(response, "%s exchanged", pokemon_out);
  return response;
}
