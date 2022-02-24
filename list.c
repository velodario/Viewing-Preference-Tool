#include "list.h"
#include <stdlib.h>
#include <string.h>

// create new list
void list_new(list* list, int element_size, freeFunction freeFunc)
{
    if (element_size <= 0) { return; }

    // init properties
    list->length = 0;
    list->elementSize = element_size;
    list->head = list->tail = NULL;
    list->freeFunc = freeFunc;
}

// delete list
void list_destroy(list* list)
{
    listNode* current;
    while (list->head != NULL)
    {
        // going through elements
        current = list->head;
        list->head = current->next;

        // call list free function
        if (list->freeFunc) { list->freeFunc(current->data); }

        // free memory
        free(current->data);
        free(current);
    }
}

// add element to the beginning of the list
bool list_prepend(list* list, void* element)
{
    // allocate memory for new element
    listNode* node = malloc(sizeof (listNode));
    node->data = malloc(list->elementSize);

    // copy data of new element
    memcpy(node->data, element, list->elementSize);

    // insert element at begin
    node->next = list->head;
    list->head = node;

    // check if is first node
    if (!list->tail) { list->tail = list->head; }

    // increase list length
    list->length++;

    return true;
}

// add element to the end of the list
bool list_append(list* list, void* element)
{
    // allocate memory for new element
    listNode* node = malloc(sizeof (listNode));
    node->data = malloc(list->elementSize);
    node->next = NULL;

    // copy data of new element
    memcpy(node->data, element, list->elementSize);

    // if first node then set reference to new element
    if (list->length == 0)
    {
        list->head = list->tail = node;
    }
    else
    {
        list->tail->next = node;    // set next ref of previous tail to new element
        list->tail = node;          // set end ref to new element
    }

    // increase list length
    list->length++;

    return true;
}

// function to insert a element at required position
// NOTE: position starts with 1
bool list_insert(list* list, int pos, void* element)
{
    listNode** current = &list->head;

    // is valid position
    if (pos < 1 || pos > list->length + 1)
    {
        return false;
    }
    else {

        // keep looping until the pos is zero
        while (pos--) {

            if (pos == 0) {

                // allocate memory for new element
                listNode* temp = malloc(sizeof (listNode));
                temp->data = malloc(list->elementSize);

                // copy data of new element
                memcpy(temp->data, element, list->elementSize);

                // making the new Node to point to
                // the old Node at the same position
                temp->next = *current;

                // changing the pointer of the previous node
                // to the old node to point to the new node
                *current = temp;
            }
            else
              // assign double pointer variable to point to the
              // pointer pointing to the address of next node
              current = &(*current)->next;
        }
        list->length++;
    }

    return true;
}

// remove element at index
bool list_remove(list* list, int pos)
{
    listNode** head_ref = &list->head;

    // if linked list is empty
    if (*head_ref == NULL) { return false; }

    // store head node
    listNode* temp = *head_ref;

    // if head needs to be removed
    if (pos == 0)
    {
        // change head
        *head_ref = temp->next;

        // free old head
        free(temp);

        list->length--;

        return true;
    }

    // find previous node of the node to be deleted
    int i;
    for(i = 0; temp != NULL && i < pos - 1; i++)
    {
        temp = temp->next;
    }

    // if position is more than number of nodes
    if (temp == NULL || temp->next == NULL) { return false; }

    // node temp->next is the node to be deleted
    // store pointer to the next of node to be deleted
    listNode *next = temp->next->next;

    // unlink the node from linked list
    free(temp->next); // Free memory

    // unlink the deleted node from list
    temp->next = next;

    list->length--;

    return true;
}

// apply function to every node
void list_for_each(list* list, listIterator iterator)
{
    if (iterator == NULL) { return; }

    listNode* node = list->head;
    bool result = true;

    // going through elements
    while (node != NULL && result)
    {
        result = iterator(node->data);
        node = node->next;
    }
}

// get element at required position
void* list_at(list* list, int pos)
{
    listNode* current = list->head;
    int count = 1;
    while (current != NULL)
    {
        if (count == pos)
        {
            return (current->data);
        }
        count++;
        current = current->next;
    }

    return NULL;
}

// reference to the beginning of the list
bool list_head(list* list, void* element, bool removeFromList)
{
    if (list->head == NULL) { return false; }

    // copy data to outer pointer
    listNode* node = list->head;
    memcpy(element, node->data, list->elementSize);

    // remove element from list
    if (removeFromList)
    {
        list->head = node->next;
        list->length--;

        // delete objects
        free(node->data);
        free(node);
    }

    return true;
}

// reference to the end of the list
bool list_tail(list* list, void* element)
{
    if (list->tail == NULL) { return false; }

    // copy data to outer pointer
    listNode* node = list->tail;
    memcpy(element, node->data, list->elementSize);

    return true;
}

// list length
int list_size(list* list)
{
    return list->length;
}
