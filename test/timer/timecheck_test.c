#include <limits.h>
#include "gel/timer/timecheck.h"
#include "unity.h"

void setUp() {}

void tearDown() {}

void test_expired_timers() {
    TEST_ASSERT(!is_expired(0, 0, 1))
    TEST_ASSERT(!is_strictly_expired(0, 1, 1))
    TEST_ASSERT(is_loosely_expired(0, 1, 1))
    TEST_ASSERT(is_expired(0, 2, 1))
}

void test_time_travel() {
    TEST_ASSERT(!is_expired(200, 50, 100));
    TEST_ASSERT(!is_expired(200, 150, 100));
    TEST_ASSERT(!is_expired(200, 200, 100));
    TEST_ASSERT(!is_expired(200, 250, 100));
    TEST_ASSERT(is_expired(200, 350, 100));
}

void test_rollback() {
    unsigned long start, time;
    int           i;
    const int     passing = 500;

    start = time = ULONG_MAX - passing / 3;

    for (i = 0; i < passing; i++) {
        TEST_ASSERT(!is_expired(start, time++, passing));
    }

    time++;
    TEST_ASSERT(is_expired(start, time, passing));
}

void test_interval() {
    TEST_ASSERT_EQUAL(0, time_interval(0, 0));
    TEST_ASSERT_EQUAL(10, time_interval(0, 10));
    TEST_ASSERT_EQUAL(10, time_interval(10, 0));
    TEST_ASSERT_EQUAL(900, time_interval(100, 1000));
    TEST_ASSERT_EQUAL(21, time_interval(ULONG_MAX - 10, 10));
    TEST_ASSERT_EQUAL(20, time_interval(ULONG_MAX - 10, ULONG_MAX + 10));
    TEST_ASSERT_EQUAL(20, time_interval(ULONG_MAX + 10, ULONG_MAX - 10));
}
