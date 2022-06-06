#ifndef WATCHER_H_INCLUDED
#define WATCHER_H_INCLUDED

/*
 *  Module handing watcher_t lists, whose elements record the changes made to a memory location.
 * When a location is changed it can be notified with watcher_check_for_changes, or it can
 * fire a callback when watcher_process_changes is called.
 * The callback has access to a void * with user data and the new value of the memory bloc
 */

#include <stdlib.h>
#include <stdint.h>

#include "gel_conf.h"

#if GEL_MALLOC_AVAILABLE
#define WATCHER_ARRAY(ptr, num, cb, data)                                                                              \
    ((watcher_t){NULL, (ptr), sizeof(*(ptr)) * (num), (watcher_cb_t)cb, (void *)(data), 0, 0, 0})
#define WATCHER_DELAYED_ARRAY(ptr, num, cb, data, delay)                                                               \
    ((watcher_t){NULL, (ptr), sizeof(*(ptr)) * (num), (watcher_cb_t)(cb), (void *)(data), 0, (delay), 0})
#else
#define WATCHER_ARRAY(ptr, num, cb, data)                                                                              \
    ((watcher_t){{0}, (ptr), sizeof(*(ptr)) * (num), (watcher_cb_t)(cb), (data), 0, 0, 0})
#define WATCHER_DELAYED_ARRAY(ptr, num, cb, data, delay)                                                               \
    ((watcher_t){{0}, (ptr), sizeof(*(ptr)) * (num), (watcher_cb_t)(cb), (data), 0, (delay), 0})
#endif

#define WATCHER(ptr, cb, data)                WATCHER_ARRAY((ptr), 1, (cb), (data))
#define WATCHER_DELAYED(ptr, cb, data, delay) WATCHER_DELAYED_ARRAY((ptr), 1, (cb), (data), (delay))
#define WATCHER_NULL                          WATCHER((uint8_t *)NULL, NULL, NULL)

typedef void (*watcher_cb_t)(void *mem, void *data);


/*
 *  Watcher struct. Its fields should not be directly accessed
 */
typedef struct {
#if GEL_MALLOC_AVAILABLE
    void *old;
#else
    uint8_t old[GEL_STATIC_BLOCK_SIZE];
#endif
    void  *current;
    size_t size;
    void (*cb)(void *, void *);
    void         *data;
    unsigned long timestamp, delay;
    uint8_t       moved;
} watcher_t;


/*
 *  Initializes a watcher list, mainly copying current values into their old counterpart.
 *  The list should be NULL terminated
 *
 * list: watcher_t list
 */
void watcher_list_init(watcher_t *list);

/*
 *  Checks if one of the elements in the list has changed, returning 1 in that case.
 *  If a change is found the old value is **not** updated, nor is the callback fired.
 *
 * list: watcher_t list
 * return: 1 if one of the elements has changed, 0 otherwise
 */
int watcher_check_for_changes(watcher_t *list);

/*
 *  Checks if one or more elements have changed, updates the old value and fires
 *  the corresponding callback.
 *
 * list: watcher_t list
 * timestamp: a timestamp to check whether the delays (if any) have passed
 *
 * return: 1 if one of the elements has changed, 0 otherwise
 */
int watcher_process_changes(watcher_t *list, unsigned long timestamp);

/*
 *  Clear all changes (i.e. copies the current value into the old slot)
 *  **without** firing the callback.
 *
 * list: watcher_t list
 * timestamp: a timestamp to reset the last change to
 */
void watcher_clear_changes(watcher_t *list, unsigned long timestamp);

/*
 *  Triggers the callback for the index-th element
 *
 * list: watcher_t list
 * index: index of the element to trigger
 */
void watcher_trigger_cb(watcher_t *list, int index);

#endif