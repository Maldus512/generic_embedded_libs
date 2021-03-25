#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED
#include <stdint.h>
#include <string.h>

typedef enum {
    PUSH_RESULT_SUCCESS = 0,
    PUSH_RESULT_FULL,
} push_result_t;

typedef enum {
    POP_RESULT_SUCCESS = 0,
    POP_RESULT_EMPTY,
} pop_result_t;

#define ARRAY_LENGTH(A) (sizeof(A) / sizeof((A)[0]))

#define STACK_DECLARATION(NAME, ITEM_TYPE, NUM_ITEMS)                                                                  \
    struct NAME {                                                                                                      \
        uint16_t  idx, num;                                                                                            \
        ITEM_TYPE items[NUM_ITEMS];                                                                                    \
    };                                                                                                                 \
    void          NAME##_init(struct NAME *p_stack);                                                                   \
    push_result_t NAME##_push(struct NAME *p_stack, ITEM_TYPE *p_new_item);                                            \
    pop_result_t  NAME##_pop(struct NAME *p_stack, ITEM_TYPE *p_item_out);                                             \
    pop_result_t  NAME##_peek(struct NAME *p_stack, ITEM_TYPE *p_item_out);                                            \
    void          NAME##_dequeue(struct NAME *p_stack);                                                                \
    int           NAME##_is_empty(struct NAME *p_stack);                                                               \
    int           NAME##_is_full(struct NAME *p_stack);

#define STACK_DEFINITION(NAME, ITEM_TYPE)                                                                              \
    void NAME##_init(struct NAME *p_stack) {                                                                           \
        p_stack->idx = 0;                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    push_result_t NAME##_push(struct NAME *p_stack, ITEM_TYPE *p_new_item) {                                           \
        size_t const capacity = ARRAY_LENGTH(p_stack->items);                                                          \
        if (p_stack->idx == capacity) {                                                                                \
            return PUSH_RESULT_FULL;                                                                                   \
        }                                                                                                              \
                                                                                                                       \
        p_stack->items[p_stack->idx++] = *p_new_item;                                                                  \
        return PUSH_RESULT_SUCCESS;                                                                                    \
    }                                                                                                                  \
                                                                                                                       \
    pop_result_t NAME##_pop(struct NAME *p_stack, ITEM_TYPE *p_item_out) {                                             \
        if (p_stack->idx == 0) {                                                                                       \
            return POP_RESULT_EMPTY;                                                                                   \
        }                                                                                                              \
                                                                                                                       \
        if (p_item_out)                                                                                                \
            *p_item_out = p_stack->items[p_stack->idx - 1];                                                            \
        p_stack->idx--;                                                                                                \
                                                                                                                       \
        return POP_RESULT_SUCCESS;                                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    pop_result_t NAME##_peek(struct NAME *p_stack, ITEM_TYPE *p_item_out) {                                            \
        if (p_stack->idx == 0) {                                                                                       \
            return POP_RESULT_EMPTY;                                                                                   \
        }                                                                                                              \
                                                                                                                       \
        if (p_item_out)                                                                                                \
            *p_item_out = p_stack->items[p_stack->idx];                                                                \
                                                                                                                       \
        return POP_RESULT_SUCCESS;                                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    void NAME##_dequeue(struct NAME *p_stack) {                                                                        \
        if (p_stack->idx > 0) {                                                                                        \
            int i;                                                                                                       \
            for (i = 0; i < p_stack->idx - 1; i++)                                                                 \
                p_stack->items[i] = p_stack->items[i + 1];                                                             \
                                                                                                                       \
            p_stack->idx--;                                                                                            \
        }                                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    int NAME##_is_empty(struct NAME *p_stack) {                                                                        \
        return p_stack->idx == 0;                                                                                      \
    }                                                                                                                  \
                                                                                                                       \
    int NAME##_is_full(struct NAME *p_stack) {                                                                         \
        size_t const capacity = ARRAY_LENGTH(p_stack->items);                                                          \
        return p_stack->idx == capacity;                                                                               \
    }

#endif