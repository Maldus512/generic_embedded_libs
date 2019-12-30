#include <limits.h>
#include <string.h>
#include "keyboard/keyboard.h"
#include "unity.h"


enum { P_ONE = 0, P_TWO, P_THREE, P_FOUR, P_FIVE, P_ONEFIVE, P_NUM };

raw_key_t orig_keys[P_NUM] = {{.bitvalue = 0x01, .code = P_ONE},   {.bitvalue = 0x02, .code = P_TWO},
                              {.bitvalue = 0x04, .code = P_THREE}, {.bitvalue = 0x08, .code = P_FOUR},
                              {.bitvalue = 0x10, .code = P_FIVE},  {.bitvalue = 0x11, .code = P_ONEFIVE}};

raw_key_t keys[P_NUM];

void setUp() {
    memcpy(keys, orig_keys, sizeof(raw_key_t) * P_NUM);
}

void tearDown() {}

void click_release(unsigned long bitmap, int code, int period) {
    keycode_event_t event;
    unsigned long   longclick = period * 10;

    event = keyboard_routine(keys, P_NUM, period, longclick, 0, bitmap);
    TEST_ASSERT_EQUAL(KEY_NOTHING, event.event);

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
    TEST_ASSERT_EQUAL(KEY_NOTHING, event.event);

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