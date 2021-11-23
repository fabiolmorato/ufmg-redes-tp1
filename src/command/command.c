#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../socket/socket.h"
#include "../list/list.h"
#include "command.h"

list_t* command_list = NULL;
list_t* middleware_list = NULL;
socket_t s;

void command_start(char* address, unsigned int port) {
  command_list = list_create();
  middleware_list = list_create();
  s = socket_create(address, port);
}

void command_add_middleware(bool (*handler)(char**, unsigned int, char**)) {
  middleware_t* middleware = malloc(sizeof(middleware_t));
  middleware->handler = handler;
  list_append(middleware_list, middleware, sizeof(middleware_t));
}

void command_register(char* command, char* (*handler)(char**, unsigned int)) {
  size_t command_len = strlen(command);
  char* command_name = calloc((command_len + 1), sizeof(char));
  strncpy(command_name, command, command_len + 1);
  command_t* new_command = malloc(sizeof(command_t));
  new_command->command = command_name;
  new_command->handler = handler;
  list_append(command_list, new_command, sizeof(command_t));
}

bool command_run(char* command, char** params, unsigned int size, char** response) {
  char* middleware_response = malloc(1025 * sizeof(char));
  middleware_t** middlewares = (middleware_t**) middleware_list->values;
  for (unsigned int i = 0; i < middleware_list->length; i++) {
    middleware_t* middleware = middlewares[i];
    if (!middleware->handler(params, size, &middleware_response)) {
      memcpy(*response, middleware_response, strlen(middleware_response) + 1);
      free(middleware_response);
      return true;
    }
  }

  free(middleware_response);

  command_t** commands = (command_t**)command_list->values;
  for (unsigned int i = 0; i < command_list->length; i++) {
    command_t* curr_command = commands[i];
    if (strcmp(command, curr_command->command) == 0) {
      *response = curr_command->handler(params, size);
      return true;
    }
  }

  return false;
}

list_t* str_split(char* str, char separator) {
  list_t* splitted = list_create();
  int index = 0;
  size_t len = strlen(str);
  char buf[1024];

  for (int i = 0; i < len; i++) {
    char c = str[i];
    if (c == separator) {
      buf[index] = '\0';
      list_append(splitted, buf, sizeof(char) * (index + 1));
      index = 0;
    } else {
      buf[index++] = c;
    }
  }

  buf[index] = '\0';
  list_append(splitted, buf, sizeof(char) * (index + 1));

  return splitted;
}

char* message_runner(char* message) {
  char* response = malloc(1024 * sizeof(char));
  list_t* splitted = str_split(message, ' ');
  char** parts = (char**) splitted->values;

  size_t command_len = strlen(parts[0]);
  char* command = malloc((command_len + 1) * sizeof(char));
  strncpy(command, parts[0], command_len + 1);

  list_remove_at_index(splitted, 0);
  char** arguments = (char**) splitted->values;

  if (!command_run(command, arguments, splitted->length, &response)) {
    strncpy(response, "invalid message", 16);
  }

  return response;
}

void command_listen(void) {
  socket_listen(s, message_runner);
}

void command_free(void) {
  command_t** commands = (command_t**)command_list->values;

  for (int i = 0; i < command_list->length; i++) {
    free(commands[i]->command);
  }

  list_free(command_list);
}
