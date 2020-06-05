#ifndef WATCHER_H_INCLUDED
#define WATCHER_H_INCLUDED

#include <stdlib.h>

#define WATCHER(ptr, num, cb, data)                                                                                    \
    { NULL, ptr, sizeof(*(ptr)) * num, cb, data }

#define WATCHER_PROCESS_CHANGES(list) watcher_process_changes(list, sizeof(list) / sizeof(*list))

typedef struct {
    void * old;
    void * current;
    size_t size;
    void (*cb)(void *, void *);
    void *data;
} watcher_t;

void watcher_list_init(watcher_t *list, int num, void *(*malloc)(size_t size));
int  watcher_process_changes(watcher_t *list, int num);

#endif