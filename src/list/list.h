#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

typedef struct {
  void** values;
  unsigned size;
  unsigned length;
} list_t;

list_t* list_create(void);
void    list_append(list_t* list, void* value, size_t size);
void    list_remove_at_index(list_t* list, unsigned int index);
void    list_free(list_t* list);

#endif
