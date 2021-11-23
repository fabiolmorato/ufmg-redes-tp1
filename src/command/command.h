#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

#include <stdbool.h>

typedef struct {
  char* command;
  char* (*handler)(char** params, unsigned int);
} command_t;

void command_start(char* address, unsigned int port);
void command_register(char* command, char* (*handler)(char**, unsigned int));
void command_free(void);
void command_listen(void);

#endif
