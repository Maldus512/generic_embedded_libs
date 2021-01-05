#include <limits.h>
#include <string.h>
#include "keypad/keypad.h"
#include "unity.h"

#define ONEFIVEBMP    0x11
#define ONETWOFIVEBMP 0x13

enum { P_ONE = 0, P_TWO, P_THREE, P_FOUR, P_FIVE, P_ONEFIVE, P_ONETWOFIVE, P_NULL, P_NUM };

keypad_key_t keys[P_NUM] = {
    {.bitvalue = ONETWOFIVEBMP, .code = P_ONETWOFIVE},
    {.bitvalue = ONEFIVEBMP, .code = P_ONEFIVE},
    {.bitvalue = 0x01, .code = P_ONE},
    {.bitvalue = 0x02, .code = P_TWO},
    {.bitvalue = 0x04, .code = P_THREE},
    {.bitvalue = 0x08, .code = P_FOUR},
    {.bitvalue = 0x10, .code = P_FIVE},
    KEYPAD_NULL_KEY,
};

void setUp() {
    keypad_reset_keys(keys);
}

void tearDown() {}

void click_release(unsigned long bitmap, int code, int period) {
    keypad_update_t event;
    unsigned long   longclick = period * 10;

    event = keypad_routine(keys, period, longclick, 1, 0, bitmap);
    TEST_ASSERT_EQUAL(KEY_PRESS, event.event);

    event = keypad_routine(keys, period, longclick, 1, period, bitmap);
    TEST_ASSERT_EQUAL(KEY_NOTHING, event.event);

    event = keypad_routine(keys, period, longclick, 1, period + 1, bitmap);
    TEST_ASSERT_EQUAL(KEY_CLICK, event.event);
    TEST_ASSERT_EQUAL(code, event.code);

    event = keypad_routine(keys, period, longclick, 1, period + 2, 0);
    TEST_ASSERT_EQUAL(KEY_NOTHING, event.event);

    event = keypad_routine(keys, period, longclick, 1, period * 2 + 2, 0);
    TEST_ASSERT_EQUAL(KEY_NOTHING, event.event);

    event = keypad_routine(keys, period, longclick, 1, period * 2 + 3, 0);
    TEST_ASSERT_EQUAL(KEY_RELEASE, event.event);
    TEST_ASSERT_EQUAL(code, event.code);
}

void test_click_release() {
    int i, j;

    for (j = 40; j <= 4000; j += 40) {
        for (i = P_ONE; i < P_NUM - 1; i++) {
            click_release(keys[i].bitvalue, keys[i].code, j);
        }
    }
}


void test_event_chain() {
    keypad_update_t event;

    event = keypad_routine(keys, 40, 2000, 100, 0, 0);
    TEST_ASSERT_EQUAL(KEY_NOTHING, event.event);

    event = keypad_routine(keys, 40, 2000, 100, 1, 0x01);
    TEST_ASSERT_EQUAL(KEY_PRESS, event.event);

    event = keypad_routine(keys, 40, 2000, 100, 20, 0x01);
    TEST_ASSERT_EQUAL(KEY_NOTHING, event.event);

    event = keypad_routine(keys, 40, 2000, 100, 42, 0x01);
    TEST_ASSERT_EQUAL(KEY_CLICK, event.event);

    event = keypad_routine(keys, 40, 2000, 100, 42 + 50, 0x01);
    TEST_ASSERT_EQUAL(KEY_NOTHING, event.event);

    event = keypad_routine(keys, 40, 2000, 100, 42 + 101, 0x01);
    TEST_ASSERT_EQUAL(KEY_PRESSING, event.event);

    event = keypad_routine(keys, 40, 2000, 100, 42 + 101 * 2, 0x01);
    TEST_ASSERT_EQUAL(KEY_PRESSING, event.event);

    event = keypad_routine(keys, 40, 2000, 100, 42 + 2001, 0x01);
    TEST_ASSERT_EQUAL(KEY_LONGCLICK, event.event);

    event = keypad_routine(keys, 40, 2000, 100, 42 + 2001 + 101, 0x01);
    TEST_ASSERT_EQUAL(KEY_LONGPRESS, event.event);

    event = keypad_routine(keys, 40, 2000, 100, 42 + 2001 + 102, 0x01);
    TEST_ASSERT_EQUAL(KEY_NOTHING, event.event);

    event = keypad_routine(keys, 40, 2000, 100, 42 + 2001 + 101 * 2, 0x01);
    TEST_ASSERT_EQUAL(KEY_LONGPRESS, event.event);

    event = keypad_routine(keys, 40, 2000, 100, 4000, 0x01);
    TEST_ASSERT_EQUAL(KEY_LONGPRESS, event.event);

    event = keypad_routine(keys, 40, 2000, 100, 4001, 0x0);
    TEST_ASSERT_EQUAL(KEY_NOTHING, event.event);

    event = keypad_routine(keys, 40, 2000, 100, 4042, 0x0);
    TEST_ASSERT_EQUAL(KEY_RELEASE, event.event);
}


void test_longclick() {
    keypad_update_t event;

    event = keypad_routine(keys, 40, 2000, 5, 0, 0x01);
    TEST_ASSERT_EQUAL(KEY_PRESS, event.event);

    event = keypad_routine(keys, 40, 2000, 5, 2001, 0x01);
    TEST_ASSERT_EQUAL(KEY_CLICK, event.event);

    event = keypad_routine(keys, 40, 2000, 5, 2001, 0x01);
    TEST_ASSERT_EQUAL(KEY_LONGCLICK, event.event);

    event = keypad_routine(keys, 40, 2000, 5, 2007, 0x01);
    TEST_ASSERT_EQUAL(KEY_LONGPRESS, event.event);

    event = keypad_routine(keys, 40, 2000, 1, 2004, 0);
    TEST_ASSERT_EQUAL(KEY_NOTHING, event.event);

    event = keypad_routine(keys, 40, 2000, 1, 2043, 0);
    TEST_ASSERT_EQUAL(KEY_NOTHING, event.event);

    event = keypad_routine(keys, 40, 2000, 1, 2045, 0);
    TEST_ASSERT_EQUAL(KEY_RELEASE, event.event);
}


void test_multiclick(void) {
    keypad_update_t event;

    event = keypad_routine(keys, 40, 2000, 1, 0, ONEFIVEBMP);
    TEST_ASSERT_EQUAL(KEY_PRESS, event.event);

    event = keypad_routine(keys, 40, 2000, 1, 50, ONEFIVEBMP);
    TEST_ASSERT_EQUAL(KEY_CLICK, event.event);
    TEST_ASSERT_EQUAL(P_ONEFIVE, event.code);

    event = keypad_routine(keys, 40, 2000, 1, 60, 0x1);
    TEST_ASSERT_EQUAL(KEY_NOTHING, event.event);

    event = keypad_routine(keys, 40, 2000, 1, 120, 0x1);
    TEST_ASSERT_EQUAL(KEY_RELEASE, event.event);
    TEST_ASSERT_EQUAL(P_ONEFIVE, event.code);

    event = keypad_routine(keys, 40, 2000, 1, 120, 0x1);
    TEST_ASSERT_EQUAL(KEY_PRESS, event.event);

    event = keypad_routine(keys, 40, 2000, 1, 180, 0x1);
    TEST_ASSERT_EQUAL(KEY_CLICK, event.event);

    event = keypad_routine(keys, 40, 2000, 1, 180, 0);
    TEST_ASSERT_EQUAL(KEY_NOTHING, event.event);

    event = keypad_routine(keys, 40, 2000, 1, 250, 0);
    TEST_ASSERT_EQUAL(KEY_RELEASE, event.event);

    event = keypad_routine(keys, 40, 2000, 1, 300, 0x1);
    TEST_ASSERT_EQUAL(KEY_PRESS, event.event);

    event = keypad_routine(keys, 40, 2000, 1, 350, 0x1);
    TEST_ASSERT_EQUAL(KEY_CLICK, event.event);
    TEST_ASSERT_EQUAL(P_ONE, event.code);
}


void test_comboclick(void) {
    keypad_update_t event;

    keypad_routine(keys, 40, 2000, 1, 0, ONEFIVEBMP);
    event = keypad_routine(keys, 40, 2000, 1, 41, ONEFIVEBMP);
    TEST_ASSERT_EQUAL(KEY_CLICK, event.event);
    TEST_ASSERT_EQUAL(P_ONEFIVE, event.code);

    event = keypad_routine(keys, 40, 2000, 1, 41, ONETWOFIVEBMP);
    TEST_ASSERT_EQUAL(KEY_NOTHING, event.event);
    event = keypad_routine(keys, 40, 2000, 1, 82, ONETWOFIVEBMP);
    TEST_ASSERT_EQUAL(KEY_RELEASE, event.event);
    TEST_ASSERT_EQUAL(P_ONEFIVE, event.code);

    event = keypad_routine(keys, 40, 2000, 1, 82, ONEFIVEBMP);
    TEST_ASSERT_EQUAL(KEY_PRESS, event.event);
    TEST_ASSERT_EQUAL(P_ONEFIVE, event.code);
    event = keypad_routine(keys, 40, 2000, 1, 123, ONEFIVEBMP);
    TEST_ASSERT_EQUAL(KEY_CLICK, event.event);
    TEST_ASSERT_EQUAL(P_ONEFIVE, event.code);

    event = keypad_routine(keys, 40, 2000, 1, 123, ONETWOFIVEBMP);
    TEST_ASSERT_EQUAL(KEY_NOTHING, event.event);
    event = keypad_routine(keys, 40, 2000, 1, 164, ONETWOFIVEBMP);
    TEST_ASSERT_EQUAL(KEY_RELEASE, event.event);
    TEST_ASSERT_EQUAL(P_ONEFIVE, event.code);
    event = keypad_routine(keys, 40, 2000, 1, 165, ONETWOFIVEBMP);
    TEST_ASSERT_EQUAL(KEY_PRESS, event.event);
    TEST_ASSERT_EQUAL(P_ONETWOFIVE, event.code);
    event = keypad_routine(keys, 40, 2000, 1, 206, ONETWOFIVEBMP);
    TEST_ASSERT_EQUAL(KEY_CLICK, event.event);
    TEST_ASSERT_EQUAL(P_ONETWOFIVE, event.code);
}