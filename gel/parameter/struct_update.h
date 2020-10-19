#ifndef STRUCT_UPDATE_H_INCLUDED
#define STRUCT_UPDATE_H_INCLUDED

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define STRUCT_UPDATE(object, field, value)                                                                            \
    ({                                                                                                                 \
        typeof(object.field) _value = value;                                                                           \
        struct_update_from_data(offsetof(typeof(object), field), &_value, sizeof(object.field));                       \
    })


#define STRUCT_UPDATE_ARRAY(object, field, array, length)                                                              \
    struct_update_from_data(offsetof(typeof(object), field), array, sizeof(array[0]) * length);


typedef struct {
    size_t offset;
    void * data;
    size_t size;
} struct_update_t;

struct_update_t struct_update_from_data(size_t offset, void *data, size_t size);
void            struct_update_apply(void *dest, struct_update_t update);
void            struct_update_free(struct_update_t update);

#endif