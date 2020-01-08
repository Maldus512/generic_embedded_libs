#include <string.h>
#include "parameter/parameter.h"
#include "unity.h"

#ifndef LANGUAGES
#define LANGUAGES 2
#endif

#define NUM_PARAMETRI 4

#define LVL_UTENTE 0x1

#define GENERIC_FORMAT                                                                                                 \
    { .format = generic_format }

char *generic_format[LANGUAGES]       = {"Valore: %i", "Value:  %i"};
char *generic_format_float[LANGUAGES] = {"Valore: %.2f", "Value:  %.2f"};

struct parametri_t {
    int          intpar1, intpar2;
    unsigned int uintpar1;
    float        fpar1;
} parametri;

parameter_data_t p[NUM_PARAMETRI] = {
    {signed_int, {.sint = {1, 15, 5, &parametri.intpar1}}, GENERIC_FORMAT, LVL_UTENTE, {0}, 4},
    {signed_int, {.sint = {0, 100, 15, &parametri.intpar2}}, GENERIC_FORMAT, LVL_UTENTE, {0}, 0.1},
    {unsigned_int, {.uint = {0, 1000, 200, &parametri.uintpar1}}, GENERIC_FORMAT, LVL_UTENTE, {0}, 50},
    {signed_float, {.ft = {0.0, 100.0, 3.14, &parametri.fpar1}}, GENERIC_FORMAT, LVL_UTENTE, {0}, 0.01},
};

void setUp() {
    memset(&parametri, 0, sizeof(struct parametri_t));
}

void tearDown() {
    init_to_default(p, NUM_PARAMETRI);
}

void test_multiplied_modif() {
    init_to_default(p, NUM_PARAMETRI);
    int index = first_parameter(p, NUM_PARAMETRI, LVL_UTENTE);

    parameter_operator(p, NUM_PARAMETRI, index, 1);
    TEST_ASSERT_EQUAL(9, parametri.intpar1);
    parameter_operator(p, NUM_PARAMETRI, index, 2);     // Comes full circle and is initialized to min
    TEST_ASSERT_EQUAL(1, parametri.intpar1);

    next_parameter(p, NUM_PARAMETRI, &index, LVL_UTENTE);
    parameter_operator(p, NUM_PARAMETRI, index, 9);
    TEST_ASSERT_EQUAL(15, parametri.intpar2);
    parameter_operator(p, NUM_PARAMETRI, index, 10);
    TEST_ASSERT_EQUAL(16, parametri.intpar2);

    next_parameter(p, NUM_PARAMETRI, &index, LVL_UTENTE);
    int start = parametri.uintpar1;

    for (int i = 0; i < 10; i++) {
        TEST_ASSERT_EQUAL(start + i * 50, parametri.uintpar1);
        parameter_operator(p, NUM_PARAMETRI, index, 1);
    }

    next_parameter(p, NUM_PARAMETRI, &index, LVL_UTENTE);
    parameter_operator(p, NUM_PARAMETRI, index, 1);
    TEST_ASSERT_EQUAL_FLOAT(3.15, parametri.fpar1);
    parameter_operator(p, NUM_PARAMETRI, index, -3);
    TEST_ASSERT_EQUAL_FLOAT(3.12, parametri.fpar1);
    parameter_operator(p, NUM_PARAMETRI, index, -13);
    TEST_ASSERT_EQUAL_FLOAT(2.99, parametri.fpar1);
}