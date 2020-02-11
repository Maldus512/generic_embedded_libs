#ifndef QUEUE_H
#define QUEUE_H
// Freely taken from https://github.com/JSchaenzle/c-message-queue
#include <stdint.h>
#include <string.h>

typedef enum {
    ENQUEUE_RESULT_SUCCESS,
    ENQUEUE_RESULT_FULL,
} enqueue_result_t;

typedef enum {
    DEQUEUE_RESULT_SUCCESS,
    DEQUEUE_RESULT_EMPTY,
} dequeue_result_t;

#define ARRAY_LENGTH(A) (sizeof(A) / sizeof((A)[0]))

#define QUEUE_DECLARATION(NAME, ITEM_TYPE, NUM_ITEMS)                                                                  \
    struct NAME {                                                                                                      \
        uint16_t  read_idx;                                                                                            \
        uint16_t  write_idx;                                                                                           \
        ITEM_TYPE items[NUM_ITEMS];                                                                                    \
    };                                                                                                                 \
    void             NAME##_init(struct NAME *p_queue);                                                                \
    enqueue_result_t NAME##_enqueue(struct NAME *p_queue, ITEM_TYPE *p_new_item);                                      \
    dequeue_result_t NAME##_dequeue(struct NAME *p_queue, ITEM_TYPE *p_item_out);                                      \
    int              NAME##_is_empty(struct NAME *p_queue);

#define QUEUE_DEFINITION(NAME, ITEM_TYPE)                                                                              \
    void NAME##_init(struct NAME *p_queue) {                                                                           \
        p_queue->read_idx  = 0;                                                                                        \
        p_queue->write_idx = 0;                                                                                        \
    }                                                                                                                  \
                                                                                                                       \
    enqueue_result_t NAME##_enqueue(struct NAME *p_queue, ITEM_TYPE *p_new_item) {                                     \
        uint16_t elements_in = p_queue->write_idx - p_queue->read_idx;                                                 \
                                                                                                                       \
        size_t const capacity = ARRAY_LENGTH(p_queue->items);                                                          \
        if (elements_in == capacity) {                                                                                 \
            return ENQUEUE_RESULT_FULL;                                                                                \
        }                                                                                                              \
                                                                                                                       \
        uint16_t i        = (p_queue->write_idx)++ & (capacity - 1);                                                   \
        p_queue->items[i] = *p_new_item;                                                                               \
        return ENQUEUE_RESULT_SUCCESS;                                                                                 \
    }                                                                                                                  \
                                                                                                                       \
    dequeue_result_t NAME##_dequeue(struct NAME *p_queue, ITEM_TYPE *p_item_out) {                                     \
        uint16_t     elements_in = p_queue->write_idx - p_queue->read_idx;                                             \
        size_t const capacity    = ARRAY_LENGTH(p_queue->items);                                                       \
                                                                                                                       \
        if (elements_in == 0) {                                                                                        \
            return DEQUEUE_RESULT_EMPTY;                                                                               \
        }                                                                                                              \
                                                                                                                       \
        uint16_t i  = (p_queue->read_idx)++ & (capacity - 1);                                                          \
        *p_item_out = p_queue->items[i];                                                                               \
                                                                                                                       \
        return DEQUEUE_RESULT_SUCCESS;                                                                                 \
    }                                                                                                                  \
                                                                                                                       \
    int NAME##_is_empty(struct NAME *p_queue) {                                                                        \
        return ((p_queue->write_idx - p_queue->read_idx) == 0);                                                        \
    }

#endif