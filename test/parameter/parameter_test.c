#include <stdint.h>
#include "parameter/parameter.h"
#include "unity.h"

#define DEFAULT_AL 1


void setUp() {}

void tearDown() {}


void test_basic() {
    uint32_t           value = 5;
    parameter_handle_t par1  = PARAMETER(&value, 0, 10, 2, 0, DEFAULT_AL);
    TEST_ASSERT_EQUAL(10, par1.max.u32);

    parameter_operator(&par1, 1);
    TEST_ASSERT_EQUAL(6, value);
    parameter_operator(&par1, 1);
    parameter_operator(&par1, 1);
    parameter_operator(&par1, 1);
    parameter_operator(&par1, 1);
    TEST_ASSERT_EQUAL(10, value);
    parameter_operator(&par1, 1);
    TEST_ASSERT_EQUAL(0, value);

    uint32_t           max  = 12;
    parameter_handle_t par2 = PARAMETER_DLIMITS(&value, NULL, &max, 1, 0, 0, 0, DEFAULT_AL);
    TEST_ASSERT_EQUAL(0, par2.max.u32);
    TEST_ASSERT_EQUAL(&max, par2.pmax);
}


void test_corrupted_value() {
    int           value1 = 5;
    unsigned char value2 = 10;
    float         value3 = .5;
    unsigned long value4 = 500000;

    parameter_handle_t ps[] = {
        PARAMETER(&value1, 0, 10, 1, 0, DEFAULT_AL),
        PARAMETER(&value2, 0, 100, 2, 0, DEFAULT_AL),
        PARAMETER(&value3, 0, 10, 3, 0, DEFAULT_AL),
        PARAMETER(&value4, 0, 1000000, 4, 0, DEFAULT_AL),
    };

    TEST_ASSERT_EQUAL(0, parameter_check_ranges(ps, 4));
    value4 *= 4;
    TEST_ASSERT_EQUAL(1, parameter_check_ranges(ps, 4));
    TEST_ASSERT_EQUAL(0, parameter_check_ranges(ps, 4));
    value1 = 1000;
    value2 = 101;
    value3 = 10.5;
    TEST_ASSERT_EQUAL(3, parameter_check_ranges(ps, 4));

    TEST_ASSERT_EQUAL(1, value1);
    TEST_ASSERT_EQUAL(2, value2);
    TEST_ASSERT_EQUAL(3, value3);
    TEST_ASSERT_EQUAL(4, value4);
}