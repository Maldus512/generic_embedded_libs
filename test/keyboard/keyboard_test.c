#include <limits.h>
#include <string.h>
#include "keyboard/keyboard.h"
#include "unity.h"

#define ONEFIVEBMP 0x11

enum { P_ONE = 0, P_TWO, P_THREE, P_FOUR, P_FIVE, P_ONEFIVE, P_NUM };

raw_key_t keys[P_NUM] = {
    {.bitvalue = ONEFIVEBMP, .code = P_ONEFIVE}, {.bitvalue = 0x01, .code = P_ONE},  {.bitvalue = 0x02, .code = P_TWO},
    {.bitvalue = 0x04, .code = P_THREE},         {.bitvalue = 0x08, .code = P_FOUR}, {.bitvalue = 0x10, .code = P_FIVE},
};

void setUp() {
    reset_keys(keys, P_NUM);
}

void tearDown() {}

void click_release(unsigned long bitmap, int code, int period) {
    keycode_event_t event;
    unsigned long   longclick = period * 10;

    event = keyboard_routine(keys, P_NUM, period, longclick, 0, bitmap);
    TEST_ASSERT_EQUAL(KEY_PRESS, event.event);

    event = keyboard_routine(keys, P_NUM, period, longclick, period, bitmap);
    TEST_ASSERT_EQUAL(KEY_NOTHING, event.event);

    event = keyboard_routine(keys, P_NUM, period, longclick, period + 1, bitmap);
    TEST_ASSERT_EQUAL(KEY_CLICK, event.event);
    TEST_ASSERT_EQUAL(code, event.code);

    event = keyboard_routine(keys, P_NUM, period, longclick, period + 2, 0);
    TEST_ASSERT_EQUAL(KEY_NOTHING, event.event);

    event = keyboard_routine(keys, P_NUM, period, longclick, period * 2 + 2, 0);
    TEST_ASSERT_EQUAL(KEY_NOTHING, event.event);

    event = keyboard_routine(keys, P_NUM, period, longclick, period * 2 + 3, 0);
    TEST_ASSERT_EQUAL(KEY_RELEASE, event.event);
    TEST_ASSERT_EQUAL(code, event.code);
}

void test_click_release() {
    int i, j;

    for (j = 40; j <= 4000; j += 40) {
        for (i = P_ONE; i < P_NUM; i++) {
            click_release(keys[i].bitvalue, keys[i].code, j);
        }
    }
}

void test_longclick() {
    keycode_event_t event;

    event = keyboard_routine(keys, P_NUM, 40, 2000, 0, 0x01);
    TEST_ASSERT_EQUAL(KEY_PRESS, event.event);

    event = keyboard_routine(keys, P_NUM, 40, 2000, 2001, 0x01);
    TEST_ASSERT_EQUAL(KEY_LONGCLICK, event.event);

    event = keyboard_routine(keys, P_NUM, 40, 2000, 2002, 0x01);
    TEST_ASSERT_EQUAL(KEY_LONGPRESS, event.event);

    event = keyboard_routine(keys, P_NUM, 40, 2000, 2003, 0);
    TEST_ASSERT_EQUAL(KEY_NOTHING, event.event);

    event = keyboard_routine(keys, P_NUM, 40, 2000, 2043, 0);
    TEST_ASSERT_EQUAL(KEY_NOTHING, event.event);

    event = keyboard_routine(keys, P_NUM, 40, 2000, 2044, 0);
    TEST_ASSERT_EQUAL(KEY_RELEASE, event.event);
}

void test_click_time() {
    keyboard_routine(keys, P_NUM, 40, 2000, 0, 0x01);
    // As long as the key was not clicked the click time is always 0
    TEST_ASSERT_EQUAL(0, get_click_time(keys, P_NUM, P_ONE, 0));
    TEST_ASSERT_EQUAL(0, get_click_time(keys, P_NUM, P_ONE, 30));
    TEST_ASSERT_EQUAL(0, get_click_time(keys, P_NUM, P_ONE, 100));

    keyboard_routine(keys, P_NUM, 40, 2000, 50, 0x01);
    TEST_ASSERT_EQUAL(0, get_click_time(keys, P_NUM, P_ONE, 0));
    TEST_ASSERT_EQUAL(30, get_click_time(keys, P_NUM, P_ONE, 30));
    TEST_ASSERT_EQUAL(100, get_click_time(keys, P_NUM, P_ONE, 100));

    keyboard_routine(keys, P_NUM, 40, 2000, 4000, 0x01);
    TEST_ASSERT_EQUAL(0, get_click_time(keys, P_NUM, P_ONE, 0));
    TEST_ASSERT_EQUAL(3000, get_click_time(keys, P_NUM, P_ONE, 3000));
    TEST_ASSERT_EQUAL(10000, get_click_time(keys, P_NUM, P_ONE, 10000));
}

void test_multiclick() {
    keycode_event_t event;

    event = keyboard_routine(keys, P_NUM, 40, 2000, 0, ONEFIVEBMP);
    TEST_ASSERT_EQUAL(KEY_PRESS, event.event);

    event = keyboard_routine(keys, P_NUM, 40, 2000, 50, ONEFIVEBMP);
    TEST_ASSERT_EQUAL(KEY_CLICK, event.event);
    TEST_ASSERT_EQUAL(P_ONEFIVE, event.code);

    event = keyboard_routine(keys, P_NUM, 40, 2000, 60, 0x1);
    TEST_ASSERT_EQUAL(KEY_NOTHING, event.event);

    event = keyboard_routine(keys, P_NUM, 40, 2000, 120, 0x1);
    TEST_ASSERT_EQUAL(KEY_RELEASE, event.event);
    TEST_ASSERT_EQUAL(P_ONEFIVE, event.code);

    event = keyboard_routine(keys, P_NUM, 40, 2000, 120, 0x1);
    TEST_ASSERT_EQUAL(KEY_NOTHING, event.event);

    event = keyboard_routine(keys, P_NUM, 40, 2000, 180, 0x1);
    TEST_ASSERT_EQUAL(KEY_NOTHING, event.event);

    event = keyboard_routine(keys, P_NUM, 40, 2000, 180, 0);
    TEST_ASSERT_EQUAL(KEY_NOTHING, event.event);

    event = keyboard_routine(keys, P_NUM, 40, 2000, 250, 0);
    TEST_ASSERT_EQUAL(KEY_NOTHING, event.event);

    event = keyboard_routine(keys, P_NUM, 40, 2000, 300, 0x1);
    TEST_ASSERT_EQUAL(KEY_PRESS, event.event);

    event = keyboard_routine(keys, P_NUM, 40, 2000, 350, 0x1);
    TEST_ASSERT_EQUAL(KEY_CLICK, event.event);
    TEST_ASSERT_EQUAL(P_ONE, event.code);
}