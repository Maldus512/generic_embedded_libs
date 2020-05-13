#include <string.h>
#include "parameter/parameter.h"
#include "unity.h"

#ifndef LANGUAGES
#define LANGUAGES 2
#endif

#define NUM_PARAMETRI 1

#define LVL_UTENTE 0x1

char *generic_format[LANGUAGES] = {"Valore: %i", "Value:  %i"};

struct parametri_t {
    int intpar;
} parametri;

struct limit_t {
    int min, max;
} limiti;

parameter_data_t p[NUM_PARAMETRI] = {{
    signed_int,
    {.sint = {0, 0, 0, &parametri.intpar, &limiti.min, &limiti.max}},
    {.format = generic_format},
    LVL_UTENTE,
    {0},
    0,
}};

void setUp() {
    memset(&parametri, 0, sizeof(struct parametri_t));
}

void tearDown() {
    init_to_default(p, NUM_PARAMETRI);
}

void test_minmax() {
    int index = first_parameter(p, NUM_PARAMETRI, LVL_UTENTE);

    limiti.min = 0;
    limiti.max = 5;
    init_to_default(p, NUM_PARAMETRI);

    for (int i = 0; i < 6; i++) {
        TEST_ASSERT_EQUAL(i, parametri.intpar);
        parameter_operator(p, NUM_PARAMETRI, index, +1);
    }
    TEST_ASSERT_EQUAL(0, parametri.intpar);

    limiti.min = 10;
    limiti.max = 50;
    TEST_ASSERT(check_for_defaults(p, NUM_PARAMETRI));
    TEST_ASSERT_EQUAL(10, parametri.intpar);

    init_to_default(p, NUM_PARAMETRI);
    TEST_ASSERT_EQUAL(0, parametri.intpar);
    parameter_operator(p, NUM_PARAMETRI, index, -1);
    TEST_ASSERT_EQUAL(50, parametri.intpar);
}