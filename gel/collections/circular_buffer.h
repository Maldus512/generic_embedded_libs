#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_
#include <stdint.h>
#include <stdlib.h>

#ifndef GEL_CBUF_TYPE
#define GEL_CBUF_TYPE uint8_t
#endif

typedef struct circular_buf_t {
    GEL_CBUF_TYPE *buffer;
    size_t         bufsize;     // Size of the buffer
    // The corresponding circular buffer capacity is bufsize - 1
    size_t head;     // Read only on the consumer side
    size_t tail;     // Read only on the producer side
} circular_buf_t;

/* NOT THREAD SAFE */

/*
 * Initializes a circular buffer as empty
 *  cbuf: pointer to the circular buffer struct that holds the metadata
 *  buffer: pointer to an adequately sized memory region for the circular buffer
 *  size: size in bytes for buffer
 *  returns: 0 on success, other on failure
 */
int circular_buf_init(circular_buf_t *cbuf, GEL_CBUF_TYPE *buffer, size_t size);

/*
 * Resets a circular buffer as empty
 *  cbuf: pointer to the circular buffer struct
 */
void circular_buf_reset(circular_buf_t *cbuf);

/* PRODUCER THREAD */

/*
 * Places the content of data into the circular buffer (provided there is enough
 * space left)
 *  cbuf: pointer to the circular buffer struct
 *  data: pointer to the data to add to the buffer
 *  len: bytes of data to add
 *  returns: the number of actual bytes written (can be less if there is
 * not enough room left)
 */
int circular_buf_puts(circular_buf_t *cbuf, GEL_CBUF_TYPE *data, size_t len);

/*
 * Places a single byte into the circular buffer
 *  cbuf: pointer to the circular buffer struct
 *  data: byte to add
 *  returns: 0 on success, other on failure
 */
int circular_buf_putc(circular_buf_t *cbuf, GEL_CBUF_TYPE data);

/* CONSUMER THREAD */

/*
 * Retrieves and consumes len bytes from the circular buffer
 *  cbuf: pointer to the circular buffer struct
 *  data: pointer to where the bytes will be written
 *  len: bytes of data to read
 *  returns: the number of actual bytes read (can be less if there is not
 * enough data in the buffer)
 */
int circular_buf_gets(circular_buf_t *cbuf, GEL_CBUF_TYPE *data, int len);

/*
 *  Drops len bytes from the circular buffer. Same as using circular_buf_gets
 * with a NULL pointer
 *  cbuf: pointer to the circular buffer struct
 *  len: bytes of data to read
 *  returns: the number of actual bytes dropped
 */
int circular_buf_drop(circular_buf_t *cbuf, int len);

/*
 * Retrieves and consumes a single byte from the circular buffer
 *  cbuf: pointer to the circular buffer struct
 *  data: pointer to where the byte will be written
 *  returns: 0 on success, other on failure
 */
int circular_buf_getc(circular_buf_t *cbuf, GEL_CBUF_TYPE *data);

/*
 * Retrieves len bytes from the circular buffer without consuming them
 *  cbuf: pointer to the circular buffer struct
 *  data: pointer to where the bytes will be written
 *  len: bytes of data to read
 *  returns: the number of actual bytes read (can be less if there is not
 * enough data in the buffer)
 */
int circular_buf_peek(circular_buf_t *cbuf, GEL_CBUF_TYPE *data, int len);

/* THREAD SAFE */

/*
 * Checks if the buffer is empty
 *  cbuf: pointer to the circular buffer struct
 *  returns: 1 if the buffer is empty, 0 otherwise
 */
int is_circular_buf_empty(circular_buf_t *cbuf);

/*
 * Checks if the buffer is full
 *  cbuf: pointer to the circular buffer struct
 *  returns: 1 if the buffer is full, 0 otherwise
 */
int is_circular_buf_full(circular_buf_t *cbuf);

/*
 * Returns the maximum capacity of the circular buffer. It is equal to the
 * original buffer size - 1.
 *  cbuf: pointer to the circular buffer struct
 *  returns: capacity of the buffer as it was initialized
 */
size_t circular_buf_capacity(circular_buf_t *cbuf);

/*
 * Returns the number of bytes currently stored in the buffer
 *  cbuf: pointer to the circular buffer struct
 *  returns: number of bytes stored
 */
size_t circular_buf_size(circular_buf_t *cbuf);

#endif     // CIRCULAR_BUFFER_H_