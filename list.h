#ifndef LIST_H
#define LIST_H
#include <stdbool.h>

// common function used to free malloc objects
typedef void (*freeFunction) (void*);

typedef bool (*listIterator) (void*);

typedef struct _listNode
{
    void* data;
    struct _listNode* next;
}listNode;

typedef struct
{
    int length;
    int elementSize;
    listNode* head;
    listNode* tail;
    freeFunction freeFunc;
} list;

void list_new(list* list, int element_size, freeFunction freeFunc);
void list_destroy(list* list);
bool list_prepend(list* list, void* element);
bool list_append(list* list, void* element);
bool list_insert(list* list, int pos, void* element);
bool list_remove(list* list, int pos);
void list_for_each(list* list, listIterator iterator);
void* list_at(list* list, int pos);
bool list_head(list* list, void* element, bool removeFromList);
bool list_tail(list* list, void* element);
int list_size(list* list);


#endif // LIST_H
