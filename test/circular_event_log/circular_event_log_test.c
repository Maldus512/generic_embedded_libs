#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "circular_event_log/circular_event_log.h"
#include "unity.h"

#define BUFSIZE    1900
#define EVENT_SIZE sizeof(event_t)
#define MAX_EVENTS (BUFSIZE / EVENT_SIZE)


typedef struct {
    uint64_t code;
} event_t;


static int  save_new_event(uint8_t *event, size_t position);
static int  load_events(uint8_t *events, size_t number, size_t start);
static void random_write_and_read(size_t total, size_t jump, size_t window);


static uint8_t              global_buffer[BUFSIZE];
static circular_event_log_t logger = {
    .next_position  = 0,
    .max_events     = MAX_EVENTS,
    .event_size     = EVENT_SIZE,
    .full_circle    = 0,
    .save_new_event = save_new_event,
    .load_events    = load_events,
};


void setUp(void) {
    memset(global_buffer, 0, BUFSIZE);
    logger.next_position = 0;
    logger.full_circle   = 0;
}

void tearDown(void) {}

void test_event_count(void) {
    event_t event = {0};
    for (size_t i = 0; i < MAX_EVENTS - 1; i++) {
        TEST_ASSERT_EQUAL(0, circular_event_log_new_event(&logger, (uint8_t *)&event));
        TEST_ASSERT_EQUAL(i + 1, circular_event_log_total_events(logger));
        TEST_ASSERT_EQUAL(0, logger.full_circle);
    }

    for (size_t i = 0; i < MAX_EVENTS * 2; i++) {
        TEST_ASSERT_EQUAL(0, circular_event_log_new_event(&logger, (uint8_t *)&event));
        TEST_ASSERT_EQUAL(MAX_EVENTS, circular_event_log_total_events(logger));
        TEST_ASSERT_EQUAL(1, logger.full_circle);
    }
}


void test_single_event(void) {
    const size_t jump = 13;

    for (size_t i = 0; i < MAX_EVENTS * 3; i++) {
        event_t event = {i};
        TEST_ASSERT_EQUAL(0, circular_event_log_new_event(&logger, (uint8_t *)&event));

        event_t recovered = {~i};
        TEST_ASSERT_EQUAL(1, circular_event_log_load_last_events(logger, (uint8_t *)&recovered, 1, 0));
        TEST_ASSERT_EQUAL(event.code, recovered.code);

        recovered.code = ~(i - jump);
        if (i >= jump) {
            TEST_ASSERT_EQUAL(1, circular_event_log_load_last_events(logger, (uint8_t *)&recovered, 1, jump));
            TEST_ASSERT_EQUAL(event.code - jump, recovered.code);
        } else {
            TEST_ASSERT_EQUAL(0, circular_event_log_load_last_events(logger, (uint8_t *)&recovered, 1, jump));
        }
    }
}


void test_overflow(void) {
    event_t events[(MAX_EVENTS * 3) / 2] = {0};

    for (size_t i = 0; i < (MAX_EVENTS * 3) / 2; i++) {
        events[i].code = rand();
        TEST_ASSERT_EQUAL(0, circular_event_log_new_event(&logger, (uint8_t *)&events[i]));
    }

    event_t recovered[MAX_EVENTS] = {0};
    TEST_ASSERT_EQUAL(MAX_EVENTS, circular_event_log_load_last_events(logger, (uint8_t *)&recovered, MAX_EVENTS, 0));
    for (size_t i = 0; i < MAX_EVENTS; i++) {
        TEST_ASSERT_EQUAL(events[MAX_EVENTS / 2 + i].code, recovered[i].code);
    }
}


void test_random_stuff(void) {
    const int max = MAX_EVENTS * 3;

    for (size_t i = 0; i < 100; i++) {
        setUp();

        size_t total    = rand() % max;
        size_t real_max = total > MAX_EVENTS ? MAX_EVENTS : total;

        size_t jump   = rand() % real_max;
        size_t window = rand() % (real_max - jump);
        random_write_and_read(total, jump, window);
    }
}


static void random_write_and_read(size_t total, size_t jump, size_t window) {
    event_t events[total];

    for (size_t i = 0; i < total; i++) {
        events[i].code = rand();
        TEST_ASSERT_EQUAL(0, circular_event_log_new_event(&logger, (uint8_t *)&events[i]));
    }

    event_t recovered[window];
    TEST_ASSERT_EQUAL(window, circular_event_log_load_last_events(logger, (uint8_t *)&recovered, window, jump));

    for (size_t i = 0; i < window; i++) {
        TEST_ASSERT_EQUAL(events[total - jump - window + i].code, recovered[i].code);
    }
}


static int save_new_event(uint8_t *event, size_t position) {
    if (position < MAX_EVENTS) {
        memcpy(&global_buffer[position * EVENT_SIZE], event, EVENT_SIZE);
        return 0;
    } else {
        return -1;
    }
}


static int load_events(uint8_t *events, size_t number, size_t start) {
    if (start + number > MAX_EVENTS) {
        return -1;
    }

    memcpy(events, &global_buffer[start * EVENT_SIZE], number * EVENT_SIZE);
    return number;
}