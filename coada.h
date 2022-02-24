#ifndef COADA_H
#define COADA_H
#include "list.h"

typedef struct
{
    list* list;
} coada;

void coada_new(coada* q, int element_size, freeFunction freeFunc);
void coada_destroy(coada* q);
bool coada_enq(coada* q, void* element);
bool coada_deq(coada* q, void* element);
bool coada_peek(coada* q, void* element);
int coada_size(coada* q);

#endif // coada_H
