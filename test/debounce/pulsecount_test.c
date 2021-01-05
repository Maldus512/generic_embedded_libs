#include <string.h>
#include "debounce/pulsecounter.h"
#include "unity.h"

pulse_filter_t filter;

void setUp() {}

void tearDown() {}

void test_init() {
    int i = 0;
    pulse_filter_init(&filter, COUNT_HIGH_PULSE, 0);

    for (i = 0; i < 16; i++)
        TEST_ASSERT_EQUAL(0, pulse_count(&filter, i));
}

void do_100_pulses(pulse_type_t type, int debounce) {
    int num = sizeof(unsigned int) * 8;
    int res, i, j, z;
    pulse_filter_init(&filter, type, type == COUNT_HIGH_PULSE ? 0 : 0xFFFFFFFF);

    for (i = 0; i < num; i++) {
        for (j = 0; j < 100; j++) {
            for (z = 0; z < debounce + 1; z++)
                res = pulse_filter(&filter, type == COUNT_HIGH_PULSE ? 1 << i : 0, debounce);

            TEST_ASSERT(!res);
            TEST_ASSERT_EQUAL(j, pulse_count(&filter, i));

            for (z = 0; z < debounce + 1; z++)
                res = pulse_filter(&filter, type == COUNT_HIGH_PULSE ? 0 : 1 << i, debounce);

            TEST_ASSERT(res);
            TEST_ASSERT_EQUAL(j + 1, pulse_count(&filter, i));
        }
    }
}

void test_no_debounce() {
    do_100_pulses(COUNT_HIGH_PULSE, 0);
    do_100_pulses(COUNT_LOW_PULSE, 0);
}

void test_1_debounce() {
    do_100_pulses(COUNT_HIGH_PULSE, 1);
    do_100_pulses(COUNT_LOW_PULSE, 1);
}

void test_100_debounce() {
    do_100_pulses(COUNT_HIGH_PULSE, 1);
    do_100_pulses(COUNT_LOW_PULSE, 1);
}