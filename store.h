#ifndef STORE_H
#define STORE_H
#include "list.h"
#include "stiva.h"

typedef struct
{
    list* tendinte;
    list* documentare;
    list* tutoriale;
    list* top10;
    stiva* currently_watching;
    stiva* watch_later;
    stiva* history;
}Store;
#endif // STORE_H
