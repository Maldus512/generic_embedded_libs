/*
 * Freely inspired by https://github.com/embeddedartistry/embedded-resources/tree/master/examples/c
 */
#include <stdint.h>
#include <string.h>
#include "circular_buffer.h"

/*
 * INTERNAL FUNCTIONS
 */

static size_t advance_pointer(circular_buf_t *cbuf, int step) {
    if (cbuf->full || step == 0) {
        return cbuf->head;
        cbuf->tail = (cbuf->tail + step) % cbuf->max;
    }

    cbuf->head = (cbuf->head + step) % cbuf->max;

    // We mark full because we will advance tail on the next time around
    cbuf->full = (cbuf->head == cbuf->tail);
    return cbuf->head;
}

static size_t retreat_pointer(circular_buf_t *cbuf, int step) {
    cbuf->full = 0;
    cbuf->tail = (cbuf->tail + step) % cbuf->max;
    return cbuf->tail;
}

static int _circular_buf_get(circular_buf_t *cbuf, uint8_t *data, size_t len, int consume) {
    int    read = 0, tail, head;
    size_t size = circular_buf_size(cbuf);
    if (!(cbuf && cbuf->buffer))
        return -1;

    len = len < size ? len : size;

    tail = cbuf->tail;
    head = cbuf->head;
    while (len > 0) {
        size_t chunk;
        if (head > tail)
            chunk = head - tail;
        else
            chunk = cbuf->max - tail;

        chunk = chunk > len ? len : chunk;

        if (data) {
            memcpy(data, &cbuf->buffer[tail], chunk);
            data += chunk;
        }
        read += chunk;
        len -= chunk;

        if (consume)
            tail = retreat_pointer(cbuf, chunk);
        else
            tail = (tail + chunk) % cbuf->max;
    }

    return read;
}

/*
 * NEVER THREAD SAFE
 */

int circular_buf_init(circular_buf_t *cbuf, uint8_t *buffer, size_t size) {
    if (!buffer || size <= 0)
        return 1;

    cbuf->buffer = buffer;
    cbuf->max    = size;
    circular_buf_reset(cbuf);

    return 0;
}

void circular_buf_reset(circular_buf_t *cbuf) {
    if (cbuf) {
        cbuf->head = 0;
        cbuf->tail = 0;
        cbuf->full = 0;
    }
}

/*
 * ALWAYS THREAD SAFE
 */

int is_circular_buf_empty(circular_buf_t *cbuf) {
    if (!cbuf)
        return 1;

    return (!cbuf->full && (cbuf->head == cbuf->tail));
}

int is_circular_buf_full(circular_buf_t *cbuf) {
    if (!cbuf)
        return 0;

    return cbuf->full;
}

size_t circular_buf_size(circular_buf_t *cbuf) {
    if (!cbuf)
        return 0;

    size_t size = cbuf->max;

    if (!cbuf->full) {
        if (cbuf->head >= cbuf->tail) {
            size = (cbuf->head - cbuf->tail);
        } else {
            size = (cbuf->max + cbuf->head - cbuf->tail);
        }
    }

    return size;
}

size_t circular_buf_capacity(circular_buf_t *cbuf) {
    if (!cbuf)
        return 0;

    return cbuf->max;
}

/*
 * PRODUCER THREAD
 * Only reading the tail and writing the head
 */

int circular_buf_putc(circular_buf_t *cbuf, uint8_t data) {
    int r = -1;

    if (!(cbuf && cbuf->buffer))
        return -1;

    if (!is_circular_buf_full(cbuf)) {
        cbuf->buffer[cbuf->head] = data;
        advance_pointer(cbuf, 1);
        r = 0;
    }

    return r;
}

int circular_buf_puts(circular_buf_t *cbuf, uint8_t *data, size_t len) {
    size_t head, tail;
    int    written = 0;
    if (!(cbuf && cbuf->buffer))
        return -1;

    head = cbuf->head;
    tail = cbuf->tail;
    while (!cbuf->full && len > 0) {
        int chunk;
        if (head >= tail)
            chunk = len > cbuf->max - head ? cbuf->max - head : len;
        else
            chunk = len > tail - head ? tail - head : len;

        memcpy(&cbuf->buffer[head], data, chunk);
        head = advance_pointer(cbuf, chunk);
        data += chunk;
        written += chunk;
        len -= chunk;
    }

    return written;
}

/*
 *  CONSUMER THREAD
 *  Only reading the head and writing the tail
 */

int circular_buf_getc(circular_buf_t *cbuf, uint8_t *data) {
    if (!(cbuf && cbuf->buffer))
        return -1;

    if (is_circular_buf_empty(cbuf))
        return -1;

    if (data)
        *data = cbuf->buffer[cbuf->tail];

    retreat_pointer(cbuf, 1);

    return 0;
}

int circular_buf_peek(circular_buf_t *cbuf, uint8_t *data, int len) {
    return _circular_buf_get(cbuf, data, len, 0);
}

int circular_buf_gets(circular_buf_t *cbuf, uint8_t *data, int len) {
    if (data) {
        return _circular_buf_get(cbuf, data, len, 1);
    } else {
        // If no pointer is provided the data is dropped
        retreat_pointer(cbuf, len);
        return len;
    }
}
