#include <unistd.h>
#include <string.h>
#include "watcher.h"

void watcher_list_init(watcher_t *list, int num, void *(*malloc)(size_t size)) {
    for (int i = 0; i < num; i++) {
        if (malloc)
            list[i].old = malloc(list[i].size);
        memcpy(list[i].old, list[i].current, list[i].size);
    }
}


int watcher_check_for_changes(watcher_t *list, int num) {
    int res = 0;

    for (int i = 0; i < num; i++) {
        if (memcmp(list[i].old, list[i].current, list[i].size)) {
            res = 1;
            break;
        }
    }

    return res;
}


int watcher_process_changes(watcher_t *list, int num) {
    int res = 0;

    for (int i = 0; i < num; i++) {
        if (memcmp(list[i].old, list[i].current, list[i].size)) {
            list[i].cb(list[i].current, list[i].data);
            memcpy(list[i].old, list[i].current, list[i].size);
            res = 1;
        }
    }

    return res;
}