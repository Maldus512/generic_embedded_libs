#include <string.h>
#include "collections/circular_buffer.h"
#include "unity.h"

#define BUFSIZE 64

uint8_t global_buffer[BUFSIZE];

int put_get_string(circular_buf_t *cbuf, uint8_t *writebuf, int len) {
    int      i;
    char     msg[64];
    uint8_t *readbuf = (uint8_t *)malloc(len);

    TEST_ASSERT_EQUAL(len, circular_buf_puts(cbuf, writebuf, len));
    TEST_ASSERT_EQUAL(len, circular_buf_size(cbuf));
    TEST_ASSERT_EQUAL(len, circular_buf_gets(cbuf, readbuf, len));

    for (i = 0; i < len; i++) {
        sprintf(msg, "Expected %i, found %i at index %i", writebuf[i], readbuf[i], i);
        TEST_ASSERT_EQUAL_INT8_MESSAGE(writebuf[i], readbuf[i], msg);
        if (writebuf[i] != readbuf[i])
            return -1;
    }
    free(readbuf);
    return 0;
}

void put_get_byte(circular_buf_t *cbuf, uint8_t value) {
    uint8_t res = value - 1;

    TEST_ASSERT_EQUAL(0, circular_buf_putc(cbuf, value));
    TEST_ASSERT_EQUAL(0, circular_buf_getc(cbuf, &res));
    TEST_ASSERT_EQUAL(value, res);
}

void setUp() {
    memset(global_buffer, 0, BUFSIZE);
}

void tearDown() {}

void test_initialization() {
    circular_buf_t cbuf;
    circular_buf_init(&cbuf, global_buffer, BUFSIZE);
    TEST_ASSERT_EQUAL(0, circular_buf_size(&cbuf));
    TEST_ASSERT_EQUAL(BUFSIZE - 1, circular_buf_capacity(&cbuf));
}

void test_single_put_get() {
    circular_buf_t cbuf;
    circular_buf_init(&cbuf, global_buffer, BUFSIZE);
    put_get_byte(&cbuf, 0xAA);
    TEST_ASSERT(is_circular_buf_empty(&cbuf));
}

void test_string_put_get() {
    uint8_t        buffer[32];
    int            i;
    circular_buf_t cbuf;

    for (i = 0; i < 32; i++) {
        buffer[i] = (uint8_t)(i * 3);
    }
    circular_buf_init(&cbuf, global_buffer, BUFSIZE);
    put_get_string(&cbuf, buffer, 32);
    TEST_ASSERT(is_circular_buf_empty(&cbuf));
}

void test_multiple_string_put_get() {
    uint8_t        buffer[17];
    int            i;
    circular_buf_t cbuf;

    for (i = 0; i < 17; i++) {
        buffer[i] = (uint8_t)(i * 7);
    }
    circular_buf_init(&cbuf, global_buffer, BUFSIZE);

    for (i = 0; i < 1000; i++) {
        put_get_string(&cbuf, buffer, 17);
    }
    TEST_ASSERT(is_circular_buf_empty(&cbuf));
}

void test_put_get() {
    uint8_t        buf1[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    uint8_t        buf2[8] = {0};
    circular_buf_t cbuf;
    circular_buf_init(&cbuf, global_buffer, BUFSIZE);
    TEST_ASSERT_EQUAL(8, circular_buf_puts(&cbuf, buf1, 8));
    TEST_ASSERT_EQUAL(8, circular_buf_gets(&cbuf, buf2, 8));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(buf1, buf2, 8);
    TEST_ASSERT_EQUAL(0, circular_buf_gets(&cbuf, buf2, 8));
}

void test_overflow() {
    uint8_t        buffer[BUFSIZE];
    circular_buf_t cbuf;
    circular_buf_init(&cbuf, global_buffer, BUFSIZE);

    TEST_ASSERT_EQUAL(BUFSIZE / 2, circular_buf_puts(&cbuf, buffer, BUFSIZE / 2));
    TEST_ASSERT(!is_circular_buf_full(&cbuf));
    TEST_ASSERT_EQUAL(BUFSIZE / 2 - 1, circular_buf_puts(&cbuf, buffer, BUFSIZE / 2));
    TEST_ASSERT(is_circular_buf_full(&cbuf));
    TEST_ASSERT_EQUAL(0, circular_buf_puts(&cbuf, buffer, BUFSIZE / 2));
    TEST_ASSERT(is_circular_buf_full(&cbuf));
}

void test_underflow() {
    uint8_t        buffer[BUFSIZE];
    circular_buf_t cbuf;
    circular_buf_init(&cbuf, global_buffer, BUFSIZE);

    TEST_ASSERT_EQUAL(BUFSIZE / 2, circular_buf_puts(&cbuf, buffer, BUFSIZE / 2));
    TEST_ASSERT_EQUAL(BUFSIZE / 2, circular_buf_gets(&cbuf, buffer, BUFSIZE));
    TEST_ASSERT_EQUAL(0, circular_buf_gets(&cbuf, buffer, BUFSIZE));
}

void test_multiple() {
    uint8_t        buffer[300];
    uint8_t        packet[6], read[6];
    circular_buf_t cbuf;
    int            i;
    circular_buf_init(&cbuf, buffer, BUFSIZE);

    packet[0] = 0xAA;
    packet[1] = 0;
    packet[2] = 1;
    packet[3] = 5;
    circular_buf_puts(&cbuf, packet, 6);
    circular_buf_gets(&cbuf, read, 6);
    TEST_ASSERT_EQUAL(5, read[3]);

    packet[3] = 6;
    for (i = 0; i < 300; i++) {
        circular_buf_puts(&cbuf, packet, 6);
        circular_buf_gets(&cbuf, read, 6);
        TEST_ASSERT_EQUAL(6, read[3]);
    }
}