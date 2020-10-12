#ifndef WATCHER_H_INCLUDED
#define WATCHER_H_INCLUDED

#include <stdlib.h>
#include <stdint.h>

#include "gel_conf.h"

#if GEL_MALLOC_AVAILABLE
#define WATCHER(ptr, num, cb, data) ((watcher_t){NULL, ptr, sizeof(*(ptr)) * num, (watcher_cb_t)cb, data, 0, 0, 0})
#define WATCHER_DELAYED(ptr, num, cb, data, delay)                                                                     \
    ((watcher_t){NULL, ptr, sizeof(*(ptr)) * num, (watcher_cb_t)cb, data, 0, delay, 0})
#else
#define WATCHER(ptr, num, cb, data) ((watcher_t){{0}, ptr, sizeof(*(ptr)) * num, (watcher_cb_t)cb, data, 0, 0, 0})
#define WATCHER_DELAYED(ptr, num, cb, data, delay)                                                                     \
    ((watcher_t){{0}, ptr, sizeof(*(ptr)) * num, (watcher_cb_t)cb, data, 0, delay, 0})
#endif

#define WATCHER_PROCESS_CHANGES(list, timestamp) watcher_process_changes(list, sizeof(list) / sizeof(*list), timestamp)

typedef void (*watcher_cb_t)(void *mem, void *data);

typedef struct {
#if GEL_MALLOC_AVAILABLE
    void *old;
#else
    uint8_t old[GEL_STATIC_BLOCK_SIZE];
#endif
    void * current;
    size_t size;
    void (*cb)(void *, void *);
    void *        data;
    unsigned long timestamp, delay;
    int           moved;
} watcher_t;

void watcher_list_init(watcher_t *list, int num);
int  watcher_check_for_changes(watcher_t *list, int num);
int  watcher_process_changes(watcher_t *list, int num, unsigned long timestamp);
void watcher_clear_changes(watcher_t *list, int num, unsigned long timestamp);
void watcher_trigger_cb(watcher_t *list, int num, int index);

#endif