#include <string.h>
#include "parameter/parameter.h"
#include "unity.h"

#ifndef LANGUAGES
#define LANGUAGES 2
#endif

#define NUM_PARAMETRI 6

#define LVL_UTENTE      0x1
#define LVL_TECNICO     0x2
#define LVL_COSTRUTTORE 0x4
#define LVL_ESPERTO     (LVL_TECNICO | LVL_COSTRUTTORE)
#define LVL_TUTTI       (LVL_UTENTE | LVL_TECNICO | LVL_COSTRUTTORE)
#define LVL_AMICO       (LVL_UTENTE | LVL_COSTRUTTORE)

char *generic_format[LANGUAGES] = {"Valore: %i", "Value:  %i"};

struct parametri_t {
    int user1, user2;
    int maintainer1, maintainer2;
    int manufacturer1, manufacturer2;
} parametri;

parameter_data_t p[NUM_PARAMETRI] = {
    {signed_int, {.sint = {0, 10, 5, &parametri.user1}}, NULL, generic_format, NULL, LVL_UTENTE, {0}, 0},
    {signed_int, {.sint = {0, 10, 5, &parametri.user2}}, NULL, generic_format, NULL, LVL_UTENTE, {0}, 0},
    {signed_int, {.sint = {0, 10, 5, &parametri.maintainer1}}, NULL, generic_format, NULL, LVL_TECNICO, {0}, 0},
    {signed_int, {.sint = {0, 10, 5, &parametri.maintainer2}}, NULL, generic_format, NULL, LVL_TECNICO, {0}, 0},
    {signed_int, {.sint = {0, 10, 5, &parametri.manufacturer1}}, NULL, generic_format, NULL, LVL_COSTRUTTORE, {0}, 0},
    {signed_int, {.sint = {0, 10, 5, &parametri.manufacturer1}}, NULL, generic_format, NULL, LVL_COSTRUTTORE, {0}, 0},
};

void setUp() {
    memset(&parametri, 0, sizeof(struct parametri_t));
}

void tearDown() {
    init_to_default(p, NUM_PARAMETRI);
}

void test_numero_parametri() {
    TEST_ASSERT_EQUAL(2, number_of_parameters(p, NUM_PARAMETRI, LVL_UTENTE));
    TEST_ASSERT_EQUAL(2, number_of_parameters(p, NUM_PARAMETRI, LVL_TECNICO));
    TEST_ASSERT_EQUAL(2, number_of_parameters(p, NUM_PARAMETRI, LVL_COSTRUTTORE));
}

void test_round() {
    int index = first_parameter(p, NUM_PARAMETRI, LVL_UTENTE);
    TEST_ASSERT_EQUAL(0, index);
    next_parameter(p, NUM_PARAMETRI, &index, LVL_UTENTE);
    TEST_ASSERT_EQUAL(1, index);
    next_parameter(p, NUM_PARAMETRI, &index, LVL_UTENTE);
    TEST_ASSERT_EQUAL(0, index);

    index = first_parameter(p, NUM_PARAMETRI, LVL_TECNICO);
    TEST_ASSERT_EQUAL(2, index);
    next_parameter(p, NUM_PARAMETRI, &index, LVL_TECNICO);
    TEST_ASSERT_EQUAL(3, index);
    next_parameter(p, NUM_PARAMETRI, &index, LVL_TECNICO);
    TEST_ASSERT_EQUAL(2, index);

    index = first_parameter(p, NUM_PARAMETRI, LVL_COSTRUTTORE);
    TEST_ASSERT_EQUAL(4, index);
    next_parameter(p, NUM_PARAMETRI, &index, LVL_COSTRUTTORE);
    TEST_ASSERT_EQUAL(5, index);
    next_parameter(p, NUM_PARAMETRI, &index, LVL_COSTRUTTORE);
    TEST_ASSERT_EQUAL(4, index);
}

void test_num_index() {
    TEST_ASSERT_EQUAL(0, get_parameter_index_from_num(p, NUM_PARAMETRI, 0, LVL_UTENTE));
    TEST_ASSERT_EQUAL(1, get_parameter_index_from_num(p, NUM_PARAMETRI, 1, LVL_UTENTE));
    TEST_ASSERT_EQUAL(-1, get_parameter_index_from_num(p, NUM_PARAMETRI, 2, LVL_UTENTE));

    TEST_ASSERT_EQUAL(2, get_parameter_index_from_num(p, NUM_PARAMETRI, 0, LVL_TECNICO));
    TEST_ASSERT_EQUAL(3, get_parameter_index_from_num(p, NUM_PARAMETRI, 1, LVL_TECNICO));
    TEST_ASSERT_EQUAL(-1, get_parameter_index_from_num(p, NUM_PARAMETRI, 2, LVL_TECNICO));

    TEST_ASSERT_EQUAL(4, get_parameter_index_from_num(p, NUM_PARAMETRI, 0, LVL_COSTRUTTORE));
    TEST_ASSERT_EQUAL(5, get_parameter_index_from_num(p, NUM_PARAMETRI, 1, LVL_COSTRUTTORE));
    TEST_ASSERT_EQUAL(-1, get_parameter_index_from_num(p, NUM_PARAMETRI, 2, LVL_COSTRUTTORE));

    int index = first_parameter(p, NUM_PARAMETRI, LVL_UTENTE);
    TEST_ASSERT_EQUAL(0, get_parameter_num_from_index(p, NUM_PARAMETRI, index, LVL_UTENTE));
    next_parameter(p, NUM_PARAMETRI, &index, LVL_UTENTE);
    TEST_ASSERT_EQUAL(1, get_parameter_num_from_index(p, NUM_PARAMETRI, index, LVL_UTENTE));

    index = first_parameter(p, NUM_PARAMETRI, LVL_TECNICO);
    TEST_ASSERT_EQUAL(0, get_parameter_num_from_index(p, NUM_PARAMETRI, index, LVL_TECNICO));
    next_parameter(p, NUM_PARAMETRI, &index, LVL_TECNICO);
    TEST_ASSERT_EQUAL(1, get_parameter_num_from_index(p, NUM_PARAMETRI, index, LVL_TECNICO));

    index = first_parameter(p, NUM_PARAMETRI, LVL_COSTRUTTORE);
    TEST_ASSERT_EQUAL(0, get_parameter_num_from_index(p, NUM_PARAMETRI, index, LVL_COSTRUTTORE));
    next_parameter(p, NUM_PARAMETRI, &index, LVL_COSTRUTTORE);
    TEST_ASSERT_EQUAL(1, get_parameter_num_from_index(p, NUM_PARAMETRI, index, LVL_COSTRUTTORE));
}

void test_jumping() {
    int index = first_parameter(p, NUM_PARAMETRI, LVL_COSTRUTTORE);

    next_parameter(p, NUM_PARAMETRI, &index, LVL_UTENTE);
    TEST_ASSERT_EQUAL(0, index);

    next_parameter(p, NUM_PARAMETRI, &index, LVL_TECNICO);
    TEST_ASSERT_EQUAL(2, index);
}

void test_overlap() {
    int index = first_parameter(p, NUM_PARAMETRI, LVL_TUTTI);
    TEST_ASSERT_EQUAL(0, index);
    int num = number_of_parameters(p, NUM_PARAMETRI, LVL_TUTTI);
    TEST_ASSERT_EQUAL(NUM_PARAMETRI, num);

    for (int i = 0; i < num; i++) {
        TEST_ASSERT_EQUAL(i, index);
        next_parameter(p, NUM_PARAMETRI, &index, LVL_TUTTI);
    }
    TEST_ASSERT_EQUAL(0, index);
}