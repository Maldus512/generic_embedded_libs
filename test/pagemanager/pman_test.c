#include <limits.h>
#include "pagemanager/page_manager.h"
#include "gel_conf.h"
#include "unity.h"

#define PAGE1 1
#define PAGE2 2
#define PAGE3 3

int model;

pman_page_data_t create_page1(pman_model_t model, unsigned long timestamp, void *extra) {
    (void)timestamp;
    (void)extra;
    TEST_ASSERT_EQUAL(1, model);
    return 0;
}

pman_update_t process_event1(pman_model_t model, pman_page_data_t data, pman_event_t event, unsigned long timestamp) {
    (void)model;
    (void)data;
    (void)event;
    (void)timestamp;
    return 0;
}

page_manager_t pman;

pman_page_t pages[] = {{.id            = PAGE1,
                        .create_page   = create_page1,
                        .destroy_page  = null_destroy,
                        .update_page   = null_update,
                        .process_event = process_event1},
                       {.id            = PAGE2,
                        .create_page   = create_page1,
                        .destroy_page  = null_destroy,
                        .update_page   = null_update,
                        .process_event = process_event1},
                       {.id            = PAGE3,
                        .create_page   = create_page1,
                        .destroy_page  = null_destroy,
                        .update_page   = null_update,
                        .process_event = process_event1}};

void setUp() {
    init_page_manager(&pman);
    model = 1;
}

void tearDown() {}

void test_change_page() {
    pman_change_page(&pman, model, pages[0], 0);
    TEST_ASSERT_EQUAL(PAGE1, pman.current_page.id);
    pman_change_page(&pman, model, pages[1], 0);
    TEST_ASSERT_EQUAL(PAGE2, pman.current_page.id);
    pman_change_page(&pman, model, pages[2], 0);
    TEST_ASSERT_EQUAL(PAGE3, pman.current_page.id);

    pman_back(&pman, model, 0);
    TEST_ASSERT_EQUAL(PAGE2, pman.current_page.id);
    pman_back(&pman, model, 0);
    TEST_ASSERT_EQUAL(PAGE1, pman.current_page.id);
}
