#include <assert.h>
#include <string.h>

#include "watcher.h"
#include "gel_conf.h"
#include "../timer/timecheck.h"

void watcher_list_init(watcher_t *list) {
    size_t i = 0;

    while (list[i].current) {
#if GEL_MALLOC_AVAILABLE
        list[i].old = malloc(list[i].size);
#else
        assert(GEL_STATIC_BLOCK_SIZE >= list[i].size);
#endif
        memcpy(list[i].old, list[i].current, list[i].size);
        i++;
    }
}


int watcher_check_for_changes(watcher_t *list) {
    int    res = 0;
    size_t i   = 0;

    while (list[i].current) {
        if (memcmp(list[i].old, list[i].current, list[i].size)) {
            res = 1;
            break;
        }
        i++;
    }

    return res;
}


void watcher_clear_changes(watcher_t *list, unsigned long timestamp) {
    size_t i = 0;

    while (list[i].current) {
        if (memcmp((uint8_t *)list[i].old, (uint8_t *)list[i].current, list[i].size)) {
            if (list[i].delay > 0) {
                list[i].timestamp = timestamp;
                list[i].moved     = 0;
            }

            memcpy((uint8_t *)list[i].old, (uint8_t *)list[i].current, list[i].size);
        }
        i++;
    }
}


void watcher_trigger_cb(watcher_t *list, int index) {
    list[index].cb(list[index].current, list[index].data);
}


int watcher_process_changes(watcher_t *list, unsigned long timestamp) {
    int    res = 0;
    size_t i   = 0;

    while (list[i].current) {
        if (memcmp((uint8_t *)list[i].old, (uint8_t *)list[i].current, list[i].size)) {
            if (list[i].delay > 0) {
                list[i].timestamp = timestamp;
                list[i].moved     = 1;
            } else {
                list[i].moved = 0;
                list[i].cb(list[i].current, list[i].data);
            }

            memcpy((uint8_t *)list[i].old, (uint8_t *)list[i].current, list[i].size);
            res = 1;
        } else if (list[i].moved) {
            if (is_expired(list[i].timestamp, timestamp, list[i].delay)) {
                list[i].moved = 0;
                list[i].cb(list[i].current, list[i].data);
            }
        }

        i++;
    }

    return res;
}