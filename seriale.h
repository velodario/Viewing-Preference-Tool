#ifndef SERIALE_H
#define SERIALE_H
#include "coada.h"

typedef struct
{
    char nume[1024];
    char categoria[32];
    float rating;
    coada* sezon;
    int top_position;
} seriale;

#endif // seriale_H
