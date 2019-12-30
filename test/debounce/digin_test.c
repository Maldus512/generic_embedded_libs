#include <string.h>
#include "debounce/debounce.h"
#include "unity.h"

debounce_filter_t filter;

void setUp() {
}

void tearDown() {
}

void test_init() {
    int i;
    init_debounce_filter(&filter);
    set_debounce_filter(&filter, 0);

    for (i = 0; i < 16; i++)
        TEST_ASSERT_EQUAL(0, digital_read(i, &filter));
}

void do_100_changes(int debounce) {
    int i, j, z;
    int res;
    init_debounce_filter(&filter);
    set_debounce_filter(&filter, 0);

    for (i = 0; i < 16; i++) {
        for (j = 0; j < 100; j++) {
            int value = (!(j % 2)) > 0;

            for (z = 0; z < debounce + 1; z++)
                res = debounce_filter(&filter, value << i, debounce);

            TEST_ASSERT(res);
            TEST_ASSERT_EQUAL(value, digital_read(i, &filter));
        }
    }
}

void test_no_debounce() {
    do_100_changes(0);
}

void test_1_debounce() {
    do_100_changes(1);
}

void test_100_debounce() {
    do_100_changes(100);
}


void test_interrupt_change() {
    init_debounce_filter(&filter);
    set_debounce_filter(&filter, 0);

    TEST_ASSERT(!debounce_filter(&filter, 1, 4));
    TEST_ASSERT(!debounce_filter(&filter, 1, 4));
    TEST_ASSERT(!debounce_filter(&filter, 1, 4));

    TEST_ASSERT(!debounce_filter(&filter, 0, 4));

    TEST_ASSERT(!debounce_filter(&filter, 1, 4));
    TEST_ASSERT(!debounce_filter(&filter, 1, 4));
    TEST_ASSERT(!debounce_filter(&filter, 1, 4));
    TEST_ASSERT(!debounce_filter(&filter, 1, 4));
    TEST_ASSERT(debounce_filter(&filter, 1, 4));
}