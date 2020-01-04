#include <string.h>
#include "parameter/parameter.h"
#include "unity.h"

#ifndef LANGUAGES
#define LANGUAGES 2
#endif

#define NUM_PARAMETRI 4

#define LVL_UTENTE 0x1

char *     generic_format[LANGUAGES] = {"Valore: %i", "Value:  %i"};
static int f1 = 0, f2 = 0, unlock1 = 0, unlock2 = 0;

struct parametri_t {
    int intpar1, intpar2, intpar3, intpar4;
} parametri;

void runtime1(parameter_data_t *par, int mod) {
    TEST_ASSERT(par);
    TEST_ASSERT_EQUAL(42, mod);
    f1 = 1;
}

void runtime2(parameter_data_t *par, int mod) {
    TEST_ASSERT(par);
    TEST_ASSERT_EQUAL(42, mod);
    f2               = 1;
    *par->d.sint.var = 420;
}

int toVis(void *ud) {
    if (ud == (void *)1)
        return unlock1;
    else if (ud == (void *)2)
        return unlock2;

    TEST_ABORT();
    return 0;
}

parameter_data_t p[NUM_PARAMETRI] = {
    {signed_int, {.sint = {1, 10, 5, &parametri.intpar1}}, NULL, generic_format, NULL, LVL_UTENTE, {0}, 0},
    {signed_int, {.sint = {0, 100, 15, &parametri.intpar2}}, NULL, generic_format, NULL, LVL_UTENTE, {NULL, toVis, (void *)1}, 0},
    {signed_int, {.sint = {0, 10, 1, &parametri.intpar3}}, NULL, generic_format, NULL, LVL_UTENTE, {runtime1, toVis, (void *)2}, 0},
    {signed_int, {.sint = {0, 10, 1, &parametri.intpar4}}, NULL, generic_format, NULL, LVL_UTENTE, {runtime2, NULL, NULL}, 0},
};

void setUp() {
    f1 = f2 = unlock1 = unlock2 = 0;
    memset(&parametri, 0, sizeof(struct parametri_t));
}

void tearDown() {
    init_to_default(p, NUM_PARAMETRI);
}

void test_runtime_operator() {
    unlock1 = unlock2 = 1;
    init_to_default(p, NUM_PARAMETRI);

    int index = first_parameter(p, NUM_PARAMETRI, LVL_UTENTE);

    parameter_operator(p, NUM_PARAMETRI, index, -1);
    TEST_ASSERT(!f1);
    TEST_ASSERT(!f2);
    TEST_ASSERT_EQUAL(4, parametri.intpar1);

    next_parameter(p, NUM_PARAMETRI, &index, LVL_UTENTE);
    parameter_operator(p, NUM_PARAMETRI, index, 42);
    TEST_ASSERT(!f1);
    TEST_ASSERT(!f2);
    TEST_ASSERT_EQUAL(15 + 42, parametri.intpar2);

    next_parameter(p, NUM_PARAMETRI, &index, LVL_UTENTE);
    parameter_operator(p, NUM_PARAMETRI, index, 42);
    TEST_ASSERT(f1);
    TEST_ASSERT(!f2);
    TEST_ASSERT_EQUAL(1, parametri.intpar3);

    next_parameter(p, NUM_PARAMETRI, &index, LVL_UTENTE);
    parameter_operator(p, NUM_PARAMETRI, index, 42);
    TEST_ASSERT(f2);
    TEST_ASSERT_EQUAL(420, parametri.intpar4);
}

void test_skip() {
    unlock1 = unlock2 = 0;
    int num           = number_of_parameters(p, NUM_PARAMETRI, LVL_UTENTE);
    TEST_ASSERT_EQUAL(2, num);

    unlock1 = 1;
    num     = number_of_parameters(p, NUM_PARAMETRI, LVL_UTENTE);
    TEST_ASSERT_EQUAL(3, num);

    unlock2 = 1;
    num     = number_of_parameters(p, NUM_PARAMETRI, LVL_UTENTE);
    TEST_ASSERT_EQUAL(4, num);
}