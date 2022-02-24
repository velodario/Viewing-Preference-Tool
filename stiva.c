#include "stiva.h"
#include <stdlib.h>

// create new stiva
void stiva_new(stiva* s, int element_size, freeFunction freeFunc)
{
    // allocate memory for list
    s->list = malloc(sizeof (list));
    // create new list linked with stiva
    list_new(s->list, element_size, freeFunc);
}

// delete stiva
void stiva_destroy(stiva* s)
{
    // delete list
    list_destroy(s->list);
    // free memory of list object
    free(s->list);
}

void stiva_push(stiva* s, void* element)
{
    // add element at the beginning
    list_prepend(s->list, element);
}

void stiva_pop(stiva* s, void* element)
{
    // don't pop if stiva is empty
    if (stiva_size(s) <= 0) { return; }
    // get element from the beginning with removing
    list_head(s->list, element, true);
}

void stiva_peek(stiva* s, void* element)
{
    // don't peek if stiva is empty
    if (stiva_size(s) <= 0) { return; }
    // get element from the beginning without removing
    list_head(s->list, element, false);
}

int stiva_size(stiva* s)
{
    // as list size
    return list_size(s->list);
}
