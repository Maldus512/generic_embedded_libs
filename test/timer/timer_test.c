#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "timer/generic_timer.h"
#include "unity.h"

void setUp() {}

void tearDown() {}

void test_start_timer() {
    generic_timer_t timer;
    init_generic_timer(&timer);
    TEST_ASSERT_EQUAL(TIMER_STOPPED, get_timer_state(&timer));

    TEST_ASSERT_EQUAL(0, start_timer(&timer, 0));
    TEST_ASSERT_EQUAL(TIMER_COUNTING, get_timer_state(&timer));

    TEST_ASSERT_EQUAL(-1, start_timer(&timer, 0));
}

void test_count_timer() {
    generic_timer_t timer;
    init_generic_timer(&timer);

    start_timer(&timer, 0);
    TEST_ASSERT_EQUAL(0, get_elapsed_time(&timer, 0));
    TEST_ASSERT_EQUAL(10, get_elapsed_time(&timer, 10));
    TEST_ASSERT_EQUAL(1000, get_elapsed_time(&timer, 1000));
    TEST_ASSERT_EQUAL(200, get_elapsed_time(&timer, 200));

    stop_timer(&timer);
    TEST_ASSERT_EQUAL(TIMER_STOPPED, get_timer_state(&timer));

    TEST_ASSERT_EQUAL(0, get_elapsed_time(&timer, 0));
    TEST_ASSERT_EQUAL(0, get_elapsed_time(&timer, 10));
    TEST_ASSERT_EQUAL(0, get_elapsed_time(&timer, 1000));
}

void test_set_timer() {
    generic_timer_t timer;
    init_generic_timer(&timer);

    TEST_ASSERT_EQUAL(0, set_timer(&timer, 1000));
    TEST_ASSERT_EQUAL(1000, get_total_time(&timer));
    TEST_ASSERT_EQUAL(0, set_timer(&timer, 2000));
    TEST_ASSERT_EQUAL(2000, get_total_time(&timer));

    TEST_ASSERT_EQUAL(TIMER_STOPPED, get_timer_state(&timer));

    // Cannot set a started timer; it has to be stopped first
    start_timer(&timer, 0);
    TEST_ASSERT_EQUAL(-1, set_timer(&timer, 2000));

    TEST_ASSERT_EQUAL(2000, get_remaining_time(&timer, 0));
    TEST_ASSERT_EQUAL(1500, get_remaining_time(&timer, 500));
    TEST_ASSERT(!is_timer_reached(&timer, 500));
    TEST_ASSERT_EQUAL(500, get_remaining_time(&timer, 1500));
    TEST_ASSERT_EQUAL(0, get_remaining_time(&timer, 2000));
    TEST_ASSERT(is_timer_reached(&timer, 2001));
    TEST_ASSERT_EQUAL(0, get_remaining_time(&timer, 2500));

    change_timer(&timer, 42);
    TEST_ASSERT(!is_timer_reached(&timer, 40));
    TEST_ASSERT(is_timer_reached(&timer, 50));
    TEST_ASSERT_EQUAL(42, get_total_time(&timer));
}

void test_pause_timer() {
    generic_timer_t timer;
    unsigned long   time = 0;
    init_generic_timer(&timer);

    set_timer(&timer, 100);
    start_timer(&timer, time);
    time += 20;

    TEST_ASSERT_EQUAL(0, pause_timer(&timer, time));
    TEST_ASSERT_EQUAL(-1, pause_timer(&timer, time));

    time += 100;
    start_timer(&timer, time);

    TEST_ASSERT_EQUAL(80, get_remaining_time(&timer, time));
    time += 70;
    TEST_ASSERT_EQUAL(10, get_remaining_time(&timer, time));
    time += 10;
    TEST_ASSERT_EQUAL(0, get_remaining_time(&timer, time));
}

void test_stress() {
    unsigned long   time = 0, elapsed = 0;
    generic_timer_t timer;
    init_generic_timer(&timer);

    srand(42);

    set_timer(&timer, 100000000);
    start_timer(&timer, time);

    while (!is_timer_reached(&timer, time)) {
        unsigned long delay = rand() % 1000;
        time += delay;

        if (get_timer_state(&timer) == TIMER_COUNTING)
            elapsed += delay;

        switch (rand() % 2) {
            case 0:
                pause_timer(&timer, time);
                break;
            case 1:
                start_timer(&timer, time);
                break;
            default:
                break;
        }

        TEST_ASSERT_EQUAL(elapsed, get_elapsed_time(&timer, time));
    }
}