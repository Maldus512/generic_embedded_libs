#include <stdint.h>
#include "data_structures/struct_update.h"
#include "unity.h"

#define DEFAULT_AL 1

struct my_data {
    int           value1;
    unsigned long value2;
    double        value3;
    char          value4;
    char          value5[32];
};

void setUp() {}

void tearDown() {}

void test_update() {
    struct my_data data = {1, 2, 3, '4', "My string"};

    struct_update_t update1 = STRUCT_UPDATE(data, value1, 100);
    struct_update_apply(&data, update1);
    TEST_ASSERT_EQUAL(data.value1, 100);

    struct_update_t update2 = STRUCT_UPDATE(data, value2, 200);
    struct_update_apply(&data, update2);
    TEST_ASSERT_EQUAL(data.value2, 200);

    struct_update_t update3 = STRUCT_UPDATE(data, value3, 0.3);
    struct_update_apply(&data, update3);
    TEST_ASSERT_EQUAL(data.value3, .3);

    struct_update_t update4 = STRUCT_UPDATE(data, value4, '4');
    struct_update_apply(&data, update4);
    TEST_ASSERT_EQUAL(data.value4, '4');

    struct_update_free(update1);
    struct_update_free(update2);
    struct_update_free(update3);
    struct_update_free(update4);

    /*char            other_string[] = "second string";
    struct_update_t update5        = STRUCT_UPDATE_ARRAY(data, value5, other_string, strlen(other_string));
    struct_update_apply(&data, update5);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(data.value5, other_string, strlen(other_string));*/
}
