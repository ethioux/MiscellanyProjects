#ifndef __list_h__
#define __list_h__

#include <stdlib.h>

struct _list {
    struct _list* prev;
    struct _list* next;
    void* data;
};

typedef struct _list list;
typedef struct _list* plist;

plist list_create(void* data);
void list_push(plist head, void* data);
plist list_insert(plist head, void* data);
void* list_pop(plist* head);
void* list_removeItem(plist* head, void* data);
void* list_find(plist head, void* data);
void* list_getAt(plist head, int posistion);

#define getnext(item) item->next
#define getprev(item) item->prev

#endif
