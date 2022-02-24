#include "coada.h"
#include <stdlib.h>

// create new coada
void coada_new(coada* q, int element_size, freeFunction freeFunc)
{
    // allocate memory for list
    q->list = malloc(sizeof (list));
    // create new list
    list_new(q->list, element_size, freeFunc);
}

// delete coada
void coada_destroy(coada* q)
{
    // delete list
    list_destroy(q->list);
    // free memory
    free(q->list);
}

// add element to coada
bool coada_enq(coada* q, void* element)
{
    // all element to list
    return list_append(q->list, element);
}

// remove element from coada and get reference to it
bool coada_deq(coada* q, void* element)
{
    // pop element from list
    return list_head(q->list, element, true);
}

bool coada_peek(coada* q, void* element)
{
    // take element from list
    return list_head(q->list, element, false);
}

int coada_size(coada* q)
{
    // as list size
    return list_size(q->list);
}
