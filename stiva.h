#ifndef STIVA_H
#define STIVA_H
#include "list.h"

typedef struct
{
    list* list;
} stiva;

void stiva_new(stiva* s, int element_size, freeFunction freeFunc);
void stiva_destroy(stiva* s);
void stiva_push(stiva* s, void* element);
void stiva_pop(stiva* s, void* element);
void stiva_peek(stiva* s, void* element);
int stiva_size(stiva* s);

#endif // stiva_H
