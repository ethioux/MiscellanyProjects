#ifndef __array_h__
#define __array_h__

#include <string.h>
#include <stdlib.h>
#include <memory.h>

typedef struct _array {
    int size;
    char** items;
} array, *parray;

parray array_create();
int array_add(parray _array, char* data);
char* array_getAt(parray _array, int pos);
void array_clear(parray _array);
int array_size(parray _array);

#endif
