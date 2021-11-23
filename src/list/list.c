#include <stdlib.h>
#include <string.h>
#include "list.h"

list_t* list_create(void) {
  list_t* list = malloc(sizeof(list_t));

  if (list == NULL) {
    return NULL;
  }

  list->size = 16;
  list->length = 0;

  list->values = malloc(sizeof(void*) * list->size);

  if (list->values == NULL) {
    free(list);
    return NULL;
  }

  return list;
}

void list_append(list_t* list, void* value, size_t size) {
  if (list->length == list->size) {
    list->size *= 2;
    list->values = realloc(list->values, list->size * sizeof(void*));
  }

  char* new_value = malloc(size);
  memcpy(new_value, value, size);
  list->values[list->length++] = new_value;
}

void list_remove_at_index(list_t* list, unsigned int index) {
  if (list->length == 0) return;

  list->length = list->length - 1;

  if (list->length <= list->size / 2 && list->size > 16) {
    list->size /= 2;
  }

  void** new_list = malloc(list->size * sizeof(void*));
  unsigned int curr_index = 0;

  for (unsigned int i = 0; i < list->length + 1; i++) {
    void* value = list->values[i];
    if (i == index) {
      free(value);
      continue;
    }
    new_list[curr_index++] = value;
  }

  free(list->values);
  list->values = new_list;
}

void list_free(list_t* list) {
  for (unsigned i = 0; i < list->length; i++) {
    free(list->values[i]);
  }

  free(list->values);
  free(list);
}
