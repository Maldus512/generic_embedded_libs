#include <limits.h>
#include "pagemanager/page_manager.h"
#include "gel_conf.h"
#include "unity.h"

#define PAGE1 1
#define PAGE2 2
#define PAGE3 3

int model;

int create1 = 0;
int create2 = 0;
int create3 = 0;

pman_page_data_t create_page1(pman_model_t model, void *extra) {
    (void)extra;
    (void)model;
    create1++;
    return 0;
}

pman_page_data_t create_page2(pman_model_t model, void *extra) {
    (void)extra;
    (void)model;
    create2++;
    return 0;
}

pman_page_data_t create_page3(pman_model_t model, void *extra) {
    (void)extra;
    (void)model;
    create3++;
    return 0;
}

int open1 = 0;
int open2 = 0;
int open3 = 0;

void open_page1(pman_model_t model, pman_page_data_t data) {
    (void)model;
    (void)data;
    open1++;
}

void open_page2(pman_model_t model, pman_page_data_t data) {
    (void)data;
    (void)model;
    open2++;
}

void open_page3(pman_model_t model, pman_page_data_t data) {
    (void)data;
    (void)model;
    open3++;
}

int close1 = 0;
int close2 = 0;
int close3 = 0;

void close_page1(pman_page_data_t data) {
    (void)data;
    close1++;
}

void close_page2(pman_page_data_t data) {
    (void)data;
    close2++;
}

void close_page3(pman_page_data_t data) {
    (void)data;
    close3++;
}

int destroy1 = 0;
int destroy2 = 0;
int destroy3 = 0;

void destroy_page1(pman_page_data_t data, void *extra) {
    (void)data;
    (void)extra;
    destroy1++;
}

void destroy_page2(pman_page_data_t data, void *extra) {
    (void)data;
    (void)extra;
    destroy2++;
}

void destroy_page3(pman_page_data_t data, void *extra) {
    (void)data;
    (void)extra;
    destroy3++;
}

int event1 = 0;

pman_message_t process_event1(pman_model_t model, pman_page_data_t data, pman_event_t event) {
    (void)model;
    (void)data;
    (void)event;
    event1++;
    return 0;
}

page_manager_t pman;

pman_page_t pages[] = {{.id            = PAGE1,
                        .create        = create_page1,
                        .open          = open_page1,
                        .close         = close_page1,
                        .destroy       = destroy_page1,
                        .process_event = process_event1},
                       {.id            = PAGE2,
                        .create        = create_page2,
                        .open          = open_page2,
                        .close         = close_page2,
                        .destroy       = destroy_page2,
                        .process_event = process_event1},
                       {.id            = PAGE3,
                        .create        = create_page3,
                        .open          = open_page3,
                        .close         = close_page3,
                        .destroy       = destroy_page3,
                        .process_event = process_event1}};

void setUp() {
    create1 = create2 = create3 = 0;
    open1 = open2 = open3 = 0;
    close1 = close2 = close3 = 0;
    destroy1 = destroy2 = destroy3 = 0;
    pman_init(&pman);
    model = 1;
}

void tearDown() {}

void test_change_page() {
    pman_change_page(&pman, model, pages[0]);
    TEST_ASSERT_EQUAL(PAGE1, pman.current_page.id);
    TEST_ASSERT_EQUAL(1, create1);
    TEST_ASSERT_EQUAL(1, open1);

    pman_change_page(&pman, model, pages[1]);
    TEST_ASSERT_EQUAL(PAGE2, pman.current_page.id);
    TEST_ASSERT_EQUAL(1, close1);
    TEST_ASSERT_EQUAL(1, open2);

    pman_change_page(&pman, model, pages[2]);
    TEST_ASSERT_EQUAL(PAGE3, pman.current_page.id);
    TEST_ASSERT_EQUAL(1, close2);
    TEST_ASSERT_EQUAL(1, open3);

    pman_back(&pman, model);
    TEST_ASSERT_EQUAL(PAGE2, pman.current_page.id);
    TEST_ASSERT_EQUAL(1, close3);
    pman_back(&pman, model);
    TEST_ASSERT_EQUAL(2, close2);
    TEST_ASSERT_EQUAL(PAGE1, pman.current_page.id);

    TEST_ASSERT_EQUAL(1, create1);
    TEST_ASSERT_EQUAL(1, create2);
    TEST_ASSERT_EQUAL(1, create3);
    TEST_ASSERT_EQUAL(0, destroy1);
    TEST_ASSERT_EQUAL(1, destroy2);
    TEST_ASSERT_EQUAL(1, destroy3);
}


void test_swap_page(void) {
    pman_change_page(&pman, model, pages[0]);
    TEST_ASSERT_EQUAL(PAGE1, pman.current_page.id);
    TEST_ASSERT_EQUAL(1, create1);
    TEST_ASSERT_EQUAL(1, open1);

    pman_change_page(&pman, model, pages[1]);
    TEST_ASSERT_EQUAL(PAGE2, pman.current_page.id);
    TEST_ASSERT_EQUAL(1, close1);
    TEST_ASSERT_EQUAL(1, open2);

    pman_swap_page(&pman, model, pages[2]);
    TEST_ASSERT_EQUAL(PAGE3, pman.current_page.id);
    TEST_ASSERT_EQUAL(1, close2);
    TEST_ASSERT_EQUAL(1, open3);

    TEST_ASSERT_EQUAL(1, create1);
    TEST_ASSERT_EQUAL(1, create2);
    TEST_ASSERT_EQUAL(1, create3);

    TEST_ASSERT_EQUAL(1, close2);

    TEST_ASSERT_EQUAL(0, destroy1);
    TEST_ASSERT_EQUAL(1, destroy2);
    TEST_ASSERT_EQUAL(0, destroy3);

    pman_back(&pman, model);
    TEST_ASSERT_EQUAL(1, close3);
    TEST_ASSERT_EQUAL(PAGE1, pman.current_page.id);

    TEST_ASSERT_EQUAL(0, destroy1);
    TEST_ASSERT_EQUAL(1, destroy3);
}
