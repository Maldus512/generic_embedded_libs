#ifndef STRUCT_UPDATE_H_INCLUDED
#define STRUCT_UPDATE_H_INCLUDED

/*
 * struct_update_t holds the encoding of a change to be made to a struct field.
 * The actual encoding involves storing the memory to be written and the offset
 * of the field from the start of the structure.
 */


#include <stdlib.h>
#include <stddef.h>
#include <string.h>

/*
 *  Create a new struct_update_t
 *
 * object: struct instance in question
 * field: name of the field to be modified
 * value: variable or value to be written in the struct field. The actual type to be used is taken from the field.
 */
#define STRUCT_UPDATE(object, field, value)                                                                            \
    ({                                                                                                                 \
        typeof(object.field) _value = value;                                                                           \
        struct_update_from_data(offsetof(typeof(object), field), &_value, sizeof(object.field));                       \
    })


/*
 *  Create a new struct_update_t. The value must be a pointer.
 *
 * object: struct instance in question
 * field: name of the field to be modified
 * array: memory pointer
 * length: number of elements to be used for the modification (follows pointer aritmetic)
 */
#define STRUCT_UPDATE_ARRAY(object, field, array, length)                                                              \
    struct_update_from_data(offsetof(typeof(object), field), array, sizeof(array[0]) * length);


typedef struct {
    size_t offset;
    void * data;
    size_t size;
} struct_update_t;


/*
 *  Create a new struct_update_t. The developer should use either STRUCT_UPDATE or STRUCT_UPDATE_ARRAY instead of this
 * function
 *
 * offset: offset of the selected field from the beginning of the struct. It can be easily found with offsetof
 * data: pointer to the memory to be saved into the struct_update_t encoding
 * size: size of the memory region pointer by data
 *
 * return: struct_update_t
 */
struct_update_t struct_update_from_data(size_t offset, void *data, size_t size);

/*
 *  Applies a struct update to the pointer dest, assumed to point to the correct structure.
 *
 * dest: pointer to the struct to update
 * update: struct_update_t encoding the modification to be carried out
 */
void struct_update_apply(void *dest, struct_update_t update);

/*
 *  Frees the memory allocated by a struct_update_t
 *
 * update: struct_update_t to be deleted; can't be used anymore after this call
 */
void struct_update_free(struct_update_t update);

#endif