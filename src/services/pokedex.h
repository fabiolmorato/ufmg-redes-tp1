#ifndef POKEDEX_SERVICE_H_INCLUDED
#define POKEDEX_SERVICE_H_INCLUDED

typedef struct {
  char* name;
} pokemon_t;

char* pokedex_service_add_pokemon(char** pokemon, unsigned int size);
// char* pokedex_service_remove_pokemon(char* pokemon);
char* pokedex_service_list_pokedex(void);
// char* pokedex_service_exchange_pokemon(char* pokemon_out, char* pokemon_in);

#endif
