#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "timer/stopwatch.h"
#include "unity.h"

void setUp() {}

void tearDown() {}

void test_start_timer() {
    stopwatch_t timer;
    stopwatch_init(&timer);
    TEST_ASSERT_EQUAL(TIMER_STOPPED, stopwatch_get_state(&timer));

    TEST_ASSERT_EQUAL(0, stopwatch_start(&timer, 0));
    TEST_ASSERT_EQUAL(TIMER_COUNTING, stopwatch_get_state(&timer));

    TEST_ASSERT_EQUAL(-1, stopwatch_start(&timer, 0));
}

void test_count_timer() {
    stopwatch_t timer;
    stopwatch_init(&timer);

    stopwatch_start(&timer, 0);
    TEST_ASSERT_EQUAL(0, stopwatch_get_elapsed(&timer, 0));
    TEST_ASSERT_EQUAL(10, stopwatch_get_elapsed(&timer, 10));
    TEST_ASSERT_EQUAL(1000, stopwatch_get_elapsed(&timer, 1000));
    TEST_ASSERT_EQUAL(200, stopwatch_get_elapsed(&timer, 200));

    stopwatch_stop(&timer);
    TEST_ASSERT_EQUAL(TIMER_STOPPED, stopwatch_get_state(&timer));

    TEST_ASSERT_EQUAL(0, stopwatch_get_elapsed(&timer, 0));
    TEST_ASSERT_EQUAL(0, stopwatch_get_elapsed(&timer, 10));
    TEST_ASSERT_EQUAL(0, stopwatch_get_elapsed(&timer, 1000));
}

void test_set_timer() {
    stopwatch_t timer;
    stopwatch_init(&timer);

    TEST_ASSERT_EQUAL(0, stopwatch_set(&timer, 1000));
    TEST_ASSERT_EQUAL(1000, stopwatch_get_total_time(&timer));
    TEST_ASSERT_EQUAL(0, stopwatch_set(&timer, 2000));
    TEST_ASSERT_EQUAL(2000, stopwatch_get_total_time(&timer));

    TEST_ASSERT_EQUAL(TIMER_STOPPED, stopwatch_get_state(&timer));

    // Cannot set a started timer; it has to be stopped first
    stopwatch_start(&timer, 0);
    TEST_ASSERT_EQUAL(-1, stopwatch_set(&timer, 2000));

    TEST_ASSERT_EQUAL(2000, stopwatch_get_remaining(&timer, 0));
    TEST_ASSERT_EQUAL(1500, stopwatch_get_remaining(&timer, 500));
    TEST_ASSERT(!stopwatch_is_timer_reached(&timer, 500));
    TEST_ASSERT_EQUAL(500, stopwatch_get_remaining(&timer, 1500));
    TEST_ASSERT_EQUAL(0, stopwatch_get_remaining(&timer, 2000));
    TEST_ASSERT(stopwatch_is_timer_reached(&timer, 2001));
    TEST_ASSERT_EQUAL(0, stopwatch_get_remaining(&timer, 2500));

    stopwatch_change(&timer, 42, 40);
    TEST_ASSERT(!stopwatch_is_timer_reached(&timer, 40));
    TEST_ASSERT(stopwatch_is_timer_reached(&timer, 82));
    TEST_ASSERT_EQUAL(42, stopwatch_get_total_time(&timer));
}

void test_pause_timer() {
    stopwatch_t timer;
    unsigned long   time = 0;
    stopwatch_init(&timer);

    stopwatch_set(&timer, 100);
    stopwatch_start(&timer, time);
    time += 20;

    TEST_ASSERT_EQUAL(0, stopwatch_pause(&timer, time));
    TEST_ASSERT_EQUAL(-1, stopwatch_pause(&timer, time));

    time += 100;
    stopwatch_start(&timer, time);

    TEST_ASSERT_EQUAL(80, stopwatch_get_remaining(&timer, time));
    time += 70;
    TEST_ASSERT_EQUAL(10, stopwatch_get_remaining(&timer, time));
    time += 10;
    TEST_ASSERT_EQUAL(0, stopwatch_get_remaining(&timer, time));
}

void test_stress() {
    unsigned long   time = 0, elapsed = 0;
    stopwatch_t timer;
    stopwatch_init(&timer);

    srand(42);

    stopwatch_set(&timer, 100000000);
    stopwatch_start(&timer, time);

    while (!stopwatch_is_timer_reached(&timer, time)) {
        unsigned long delay = rand() % 1000;
        time += delay;

        if (stopwatch_get_state(&timer) == TIMER_COUNTING)
            elapsed += delay;

        switch (rand() % 2) {
            case 0:
                stopwatch_pause(&timer, time);
                break;
            case 1:
                stopwatch_start(&timer, time);
                break;
            default:
                break;
        }

        TEST_ASSERT_EQUAL(elapsed, stopwatch_get_elapsed(&timer, time));
    }
}