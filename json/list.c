#include "list.h"

/*
 * private: getLastNode
 * Returns the last node of a list
 *
 */
plist getLastNode(plist head) {
    if (head == NULL)
        return NULL;

    plist curr = head;
    while (curr->next != NULL)
        curr = curr->next;

    return curr;
}

/*
 * private: getNode
 * returns the node for the given data
 *
 */
plist getNode(plist head, void* data) {
    plist curr = head;
    while (curr != NULL) {
        if (curr->data == data) {
            return curr;
        }
        curr = curr->next;
    }

    return curr;
}

/*
 * Creates a new list
 *
 */
plist list_create(void* data) {
    plist head = (plist) malloc(sizeof(list));
    head->prev = NULL;
    head->next = NULL;
    head->data = data;

    return head;
}

/*
 * pushes some data onto the list (at the end)
 *
 */
void list_push(plist head, void* data) {
    plist last = getLastNode(head);
    last->next = (plist) malloc(sizeof(list));

    plist newitem = last->next;
    newitem->data = data;
    newitem->next = NULL;
    newitem->prev = last;
}

/*
 * removes the last item from the list
 * and returns the data
 *
 */
void* list_pop(plist* head) {
    plist last = getLastNode(*head);
    if (last == NULL)
        return NULL;

    void* data = last->data;
    if (last->prev != NULL)
        last->prev->next = NULL;

    if (last == *head)
        *head = NULL;

    free(last);
    return data;
}


/*
 * insert an item on the list (at the begining)
 *
 */
plist list_insert(plist head, void* data) {
    plist newhead = list_create(data);
    head->prev = newhead;
    newhead->next = head;

    return newhead;
}

void* list_removeItem(plist* head, void* data) {
    plist curr = getNode(*head, data);
    if (curr == *head) {
        *head = NULL;
        curr->prev = NULL;
    }
    else {
        if (curr != NULL) {
            curr->prev->next = curr->next;
            curr->next->prev = curr->prev;
        }
    }

    return curr->data;
}

/*
 * finds an object in the list
 *
 */
void* list_find(plist head, void* data) {
    plist curr = getNode(head, data);

    if (curr != NULL)
        return curr->data;

    return NULL;
}

/*
 * get an object at the give location
 *
 */
void* list_getAt(plist head, int posistion) {
    return NULL;
}
