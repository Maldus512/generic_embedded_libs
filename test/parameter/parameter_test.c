#include <string.h>
#include "parameter/parameter.h"
#include "unity.h"

#ifndef LANGUAGES
#define LANGUAGES 2
#endif

#define NUM_PARAMETRI 8

#define LVL_UTENTE 0x1

char *generic_format[LANGUAGES]       = {"Valore: %i", "Value:  %i"};
char *abilitato[LANGUAGES]            = {"Abilitato: %s", "Enabled: %s"};
char *generic_format_float[LANGUAGES] = {"Valore: %.2f", "Value:  %.2f"};
char *sino[2][LANGUAGES]              = {{"no", "no"}, {"si", "yes"}};

struct parametri_t {
    int          intpar1, intpar2;
    unsigned int uintpar1, uintpar2;
    float        fpar1, fpar2;
    char         ab1, ab2;
} parametri;

parameter_data_t p[NUM_PARAMETRI] = {
    {signed_int, {.sint = {1, 10, 5, &parametri.intpar1}}, {.format = generic_format}, LVL_UTENTE, {0}, 0},
    {signed_int, {.sint = {0, 100, 15, &parametri.intpar2}}, {0}, LVL_UTENTE, {0}, 0},
    {unsigned_int, {.uint = {0, 1000, 200, &parametri.uintpar1}}, {.format = generic_format}, LVL_UTENTE, {0}, 0},
    {unsigned_int, {.uint = {0, 3000, 512, &parametri.uintpar2}}, {.format = generic_format}, LVL_UTENTE, {0}, 0},
    {signed_float, {.ft = {0.0, 100.0, 3.14, &parametri.fpar1}}, {.format = generic_format_float}, LVL_UTENTE, {0}, 0},
    {signed_float, {.ft = {0.0, 1.0, .14, &parametri.fpar2}}, {.format = generic_format_float}, LVL_UTENTE, {0}, 0},
    {signed_char,
     {.sch = {0, 1, 0, &parametri.ab1}},
     {.format = abilitato, .string_value = (char ***)sino},
     LVL_UTENTE,
     {0},
     0},
    {signed_char,
     {.sch = {0, 1, 0, &parametri.ab2}},
     {.format = abilitato, .string_value = (char ***)sino},
     LVL_UTENTE,
     {0},
     0},
};

void setUp() {
    memset(&parametri, 0, sizeof(struct parametri_t));
}

void tearDown() {
    init_to_default(p, NUM_PARAMETRI);
}

void assert_default() {
    TEST_ASSERT_EQUAL(5, parametri.intpar1);
    TEST_ASSERT_EQUAL(15, parametri.intpar2);
    TEST_ASSERT_EQUAL(200, parametri.uintpar1);
    TEST_ASSERT_EQUAL(512, parametri.uintpar2);
    TEST_ASSERT_EQUAL(3.14, parametri.fpar1);
    TEST_ASSERT_EQUAL(.14, parametri.fpar2);
    TEST_ASSERT_EQUAL(0, parametri.ab1);
    TEST_ASSERT_EQUAL(0, parametri.ab2);
}

void test_default() {
    init_to_default(p, NUM_PARAMETRI);
    assert_default();
}


void test_string_to_display() {
    int  index = 0;
    char string[64];

    init_to_default(p, NUM_PARAMETRI);
    index = first_parameter(p, NUM_PARAMETRI, LVL_UTENTE);
    TEST_ASSERT_EQUAL(0, index);

    string_to_display(p, NUM_PARAMETRI, index, string, 0);
    TEST_ASSERT_EQUAL_STRING("Valore: 5", string);
    string_to_display(p, NUM_PARAMETRI, index, string, 1);
    TEST_ASSERT_EQUAL_STRING("Value:  5", string);

    index = next_parameter(p, NUM_PARAMETRI, &index, LVL_UTENTE);

    string_to_display(p, NUM_PARAMETRI, index, string, 0);
    TEST_ASSERT_EQUAL_STRING("15", string);
    string_to_display(p, NUM_PARAMETRI, index, string, 1);
    TEST_ASSERT_EQUAL_STRING("15", string);

    index = next_parameter(p, NUM_PARAMETRI, &index, LVL_UTENTE);
    index = next_parameter(p, NUM_PARAMETRI, &index, LVL_UTENTE);
    index = next_parameter(p, NUM_PARAMETRI, &index, LVL_UTENTE);

    string_to_display(p, NUM_PARAMETRI, index, string, 0);
    TEST_ASSERT_EQUAL_STRING("Valore: 3.14", string);
    string_to_display(p, NUM_PARAMETRI, index, string, 1);
    TEST_ASSERT_EQUAL_STRING("Value:  3.14", string);

    index = next_parameter(p, NUM_PARAMETRI, &index, LVL_UTENTE);
    index = next_parameter(p, NUM_PARAMETRI, &index, LVL_UTENTE);

    string_to_display(p, NUM_PARAMETRI, index, string, 0);
    TEST_ASSERT_EQUAL_STRING("Abilitato: no", string);
    string_to_display(p, NUM_PARAMETRI, index, string, 1);
    TEST_ASSERT_EQUAL_STRING("Enabled: no", string);
}

void test_check() {
    memset(&parametri, (int)0xFFFFFFFF, sizeof(struct parametri_t));
    TEST_ASSERT(check_for_defaults(p, NUM_PARAMETRI));
    assert_default();
}

void test_num_index() {
    int index = first_parameter(p, NUM_PARAMETRI, LVL_UTENTE);
    TEST_ASSERT_EQUAL(0, index);

    int num = get_parameter_num_from_index(p, NUM_PARAMETRI, index, LVL_UTENTE);
    TEST_ASSERT_EQUAL(0, num);

    index = get_parameter_index_from_num(p, NUM_PARAMETRI, 4, LVL_UTENTE);
    TEST_ASSERT_EQUAL(4, index);

    TEST_ASSERT_EQUAL(-1, get_parameter_num_from_index(p, NUM_PARAMETRI, NUM_PARAMETRI, LVL_UTENTE));
    TEST_ASSERT_EQUAL(-1, get_parameter_index_from_num(p, NUM_PARAMETRI, NUM_PARAMETRI, LVL_UTENTE));
}

void test_movement() {
    int index = first_parameter(p, NUM_PARAMETRI, LVL_UTENTE);

    int num = number_of_parameters(p, NUM_PARAMETRI, LVL_UTENTE);
    TEST_ASSERT_EQUAL(NUM_PARAMETRI, num);

    for (int i = 0; i < num; i++) {
        next_parameter(p, NUM_PARAMETRI, &index, LVL_UTENTE);
    }
    TEST_ASSERT_EQUAL(0, index);

    prev_parameter(p, NUM_PARAMETRI, &index, LVL_UTENTE);
    TEST_ASSERT_EQUAL(num - 1, index);

    for (int i = 0; i < num - 1; i++) {
        prev_parameter(p, NUM_PARAMETRI, &index, LVL_UTENTE);
    }
    TEST_ASSERT_EQUAL(0, index);
}

void test_parameter_operator() {
    int index = first_parameter(p, NUM_PARAMETRI, LVL_UTENTE);

    init_to_default(p, NUM_PARAMETRI);
    parameter_operator(p, NUM_PARAMETRI, index, +1);
    TEST_ASSERT_EQUAL(6, parametri.intpar1);
    parameter_operator(p, NUM_PARAMETRI, index, +1);
    TEST_ASSERT_EQUAL(7, parametri.intpar1);
    parameter_operator(p, NUM_PARAMETRI, index, +1);
    parameter_operator(p, NUM_PARAMETRI, index, +1);
    parameter_operator(p, NUM_PARAMETRI, index, +1);
    TEST_ASSERT_EQUAL(10, parametri.intpar1);

    parameter_operator(p, NUM_PARAMETRI, index, +1);
    TEST_ASSERT_EQUAL(1, parametri.intpar1);
}