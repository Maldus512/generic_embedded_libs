#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "struct_update.h"


struct_update_t struct_update_from_data(size_t offset, void *data, size_t size) {
    struct_update_t update = {.offset = offset, .size = size};
    update.data            = malloc(size);
    memcpy(update.data, data, size);

    return update;
}


void struct_update_apply(void *dest, struct_update_t update) {
    uint8_t *bytes = dest;
    memcpy(&bytes[update.offset], update.data, update.size);
}


void struct_update_free(struct_update_t update) {
    free(update.data);
}