#include <string.h>
#include "circularbuffer/circular_buffer.h"
#include "unity.h"

uint8_t buffer[32];

void setUp() {
    memset(buffer, 0, 32);
}

void tearDown() {

}

void test_initialization() {
    circular_buf_t cbuf;
    circular_buf_init(&cbuf, buffer, 32);
    TEST_ASSERT_EQUAL(0, circular_buf_size(&cbuf));
    TEST_ASSERT_EQUAL(32, circular_buf_capacity(&cbuf));
}