#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_
#include <stdint.h>
#include <stdlib.h>

struct circular_buf_t
{
    uint8_t *buffer;
    int     full;
    int   head;
    int   tail;
    int   max;     // of the buffer
};

/// Opaque circular buffer structure
typedef struct circular_buf_t circular_buf_t;

/// Pass in a storage buffer and size, returns a circular buffer handle
/// Requires: buffer is not NULL, size > 0
/// Ensures: cbuf has been created and is returned in an empty state
int circular_buf_init(circular_buf_t *cbuf, uint8_t* buffer, size_t size);

/// Reset the circular buffer to empty, head == tail. Data not cleared
/// Requires: cbuf is valid and created by circular_buf_init
void circular_buf_reset(circular_buf_t* cbuf);

/// Put version 1 continues to add data if the buffer is full
/// Old data is overwritten
/// Requires: cbuf is valid and created by circular_buf_init
int circular_buf_puts(circular_buf_t* cbuf, uint8_t *data, int len);

int circular_buf_putc(circular_buf_t * cbuf, uint8_t data);

/// Retrieve a value from the buffer
/// Requires: cbuf is valid and created by circular_buf_init
/// Returns 0 on success, -1 if the buffer is empty
int circular_buf_get(circular_buf_t* cbuf, uint8_t * data, int len);

int circular_buf_peek(circular_buf_t *cbuf, uint8_t *data, int len);

/// CHecks if the buffer is empty
/// Requires: cbuf is valid and created by circular_buf_init
/// Returns true if the buffer is empty
int circular_buf_empty(circular_buf_t* cbuf);

/// Checks if the buffer is full
/// Requires: cbuf is valid and created by circular_buf_init
/// Returns true if the buffer is full
int circular_buf_full(circular_buf_t* cbuf);

/// Check the capacity of the buffer
/// Requires: cbuf is valid and created by circular_buf_init
/// Returns the maximum capacity of the buffer
size_t circular_buf_capacity(circular_buf_t* cbuf);

/// Check the number of elements stored in the buffer
/// Requires: cbuf is valid and created by circular_buf_init
/// Returns the current number of elements in the buffer
size_t circular_buf_size(circular_buf_t* cbuf);

#endif //CIRCULAR_BUFFER_H_