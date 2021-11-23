#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

#include <stdbool.h>

typedef struct {
  char* command;
  char* (*handler)(char** params, unsigned int size);
} command_t;

typedef struct {
  bool (*handler)(char** params, unsigned int size, char** response);
} middleware_t;

void command_start(char* address, unsigned int port);
void command_add_middleware(bool (*handler)(char**, unsigned int, char**));
void command_register(char* command, char* (*handler)(char**, unsigned int));
void command_free(void);
void command_listen(void);

#endif
