#include "array.h"

parray array_create() {
    parray a = malloc(sizeof(array));
    memset(a, 0, sizeof(array));

    return a;
}

int array_add(parray _array, char* data) {
    if (_array->size == 0) {
        _array->items = (char**) malloc(sizeof(char*));
    }
    else {
        _array->items = (char**) realloc(_array->items, sizeof(char*)  * (_array->size + 1));
    }

    _array->items[_array->size++] = strdup(data);

    return _array->size;
}

char* array_getAt(parray _array, int pos) {
    if (pos < _array->size)
        return _array->items[pos];

    return NULL;
}

void array_clear(parray _array) {
    int i = 0;
    for (i = 0; i < _array->size; i++) {
        free(_array->items[i]);
    }

    free(_array->items);
    _array->size = 0;
}

int array_size(parray _array) {
    return _array->size;
}
