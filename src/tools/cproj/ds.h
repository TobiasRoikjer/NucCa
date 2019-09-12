#ifndef CPROJ_DS_H
#define CPROJ_DS_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct ll ll;

struct ll {
    void *value;
    struct ll *prev;
};

ll * ll_add(ll *list, void *value) {
    ll *next = malloc(sizeof(ll));
    next->value = value;
    next->prev = list;

    return next;
}

void ll_free(ll *list) {
    ll *entry;

    while (list != NULL) {
        entry = list;
        list = list->prev;
        free(entry);
    }
}

void ll_to_array(void **arr, ll *list) {
    for (size_t i = 0; list != NULL; i++) {
        arr[i] = list->value;
        list = list->prev;
    }
}

#endif //CPROJ_DS_H
