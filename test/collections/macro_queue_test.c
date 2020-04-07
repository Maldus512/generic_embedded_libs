// unit tests
#include "collections/queue.h"
#include "unity.h"
#include <stdint.h>
#include <stdbool.h>

QUEUE_DECLARATION(test_int_queue, uint32_t, 8);
QUEUE_DEFINITION(test_int_queue, uint32_t);

void setUp() {}
void tearDown() {}

void test_queue_works_with_integers(void) {
    struct test_int_queue q;
    test_int_queue_init(&q);

    TEST_ASSERT_EQUAL(true, test_int_queue_is_empty(&q));

    // Test Enqueue
    for (uint32_t i = 0; i < 8; i++) {
        enqueue_result_t r = test_int_queue_enqueue(&q, &i);
        TEST_ASSERT_EQUAL(ENQUEUE_RESULT_SUCCESS, r);
        TEST_ASSERT_EQUAL(false, test_int_queue_is_empty(&q));
    }

    // See that we can't enqueue after the queue is full
    uint32_t         v = 55;
    enqueue_result_t r = test_int_queue_enqueue(&q, &v);
    TEST_ASSERT_EQUAL(ENQUEUE_RESULT_FULL, r);

    // Test Dequeue
    for (uint32_t i = 0; i < 8; i++) {
        uint32_t v2;
        TEST_ASSERT_EQUAL(false, test_int_queue_is_empty(&q));
        dequeue_result_t r = test_int_queue_dequeue(&q, &v2);
        TEST_ASSERT_EQUAL(DEQUEUE_RESULT_SUCCESS, r);
        TEST_ASSERT_EQUAL(i, v2);
    }
    // See that we can't dequeue after the queue is empty
    uint32_t         v3;
    dequeue_result_t dr = test_int_queue_dequeue(&q, &v3);
    TEST_ASSERT_EQUAL(DEQUEUE_RESULT_EMPTY, dr);
    TEST_ASSERT_EQUAL(true, test_int_queue_is_empty(&q));
}

struct test_struct {
    uint8_t foo[13];     // awkward sized buffer
};
QUEUE_DECLARATION(test_struct_queue, struct test_struct, 16);
QUEUE_DEFINITION(test_struct_queue, struct test_struct);

void test_queue_works_for_structs(void) {
    struct test_struct_queue q;
    test_struct_queue_init(&q);

    TEST_ASSERT_EQUAL(true, test_struct_queue_is_empty(&q));
    // Test Enqueue
    for (uint32_t i = 0; i < 16; i++) {
        struct test_struct v = {.foo = {i, i, i, i, i, i, i, i, i, i, i, i, i}};
        enqueue_result_t   r = test_struct_queue_enqueue(&q, &v);
        TEST_ASSERT_EQUAL(ENQUEUE_RESULT_SUCCESS, r);
        TEST_ASSERT_EQUAL(false, test_struct_queue_is_empty(&q));
    }

    // See that we can't enqueue after the queue is full
    struct test_struct v;
    enqueue_result_t   r = test_struct_queue_enqueue(&q, &v);
    TEST_ASSERT_EQUAL(ENQUEUE_RESULT_FULL, r);

    // Test Dequeue
    for (uint32_t i = 0; i < 16; i++) {
        struct test_struct v2;
        TEST_ASSERT_EQUAL(false, test_struct_queue_is_empty(&q));
        dequeue_result_t r = test_struct_queue_dequeue(&q, &v2);
        TEST_ASSERT_EQUAL(DEQUEUE_RESULT_SUCCESS, r);
        for (int j = 0; j < 13; j++) {
            TEST_ASSERT_EQUAL(i, v2.foo[j]);
        }
    }
    // See that we can't dequeue after the queue is empty
    struct test_struct v3;
    dequeue_result_t   dr = test_struct_queue_dequeue(&q, &v3);
    TEST_ASSERT_EQUAL(DEQUEUE_RESULT_EMPTY, dr);
    TEST_ASSERT_EQUAL(true, test_struct_queue_is_empty(&q));
}

void test_queue_works_when_read_and_write_pointers_overflow(void) {
    struct test_int_queue q;
    test_int_queue_init(&q);

    TEST_ASSERT_EQUAL(true, test_int_queue_is_empty(&q));
    for (int cycle = 0; cycle < 40000; cycle++) {
        // Test Enqueue
        for (uint32_t i = 0; i < 8; i++) {
            enqueue_result_t r = test_int_queue_enqueue(&q, &i);
            TEST_ASSERT_EQUAL(ENQUEUE_RESULT_SUCCESS, r);
            TEST_ASSERT_EQUAL(false, test_int_queue_is_empty(&q));
        }

        // See that we can't enqueue after the queue is full
        uint32_t         v = 55;
        enqueue_result_t r = test_int_queue_enqueue(&q, &v);
        TEST_ASSERT_EQUAL(ENQUEUE_RESULT_FULL, r);

        // Test Dequeue
        for (uint32_t i = 0; i < 8; i++) {
            uint32_t v2;
            TEST_ASSERT_EQUAL(false, test_int_queue_is_empty(&q));
            dequeue_result_t r = test_int_queue_dequeue(&q, &v2);
            TEST_ASSERT_EQUAL(DEQUEUE_RESULT_SUCCESS, r);
            TEST_ASSERT_EQUAL(i, v2);
        }
        // See that we can't dequeue after the queue is empty
        uint32_t         v3;
        dequeue_result_t dr = test_int_queue_dequeue(&q, &v3);
        TEST_ASSERT_EQUAL(DEQUEUE_RESULT_EMPTY, dr);
        TEST_ASSERT_EQUAL(true, test_int_queue_is_empty(&q));
    }
}
