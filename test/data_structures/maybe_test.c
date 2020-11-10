#include "unity.h"
#include "data_structures/maybe.h"

MAYBE_DECLARATION(int);
MAYBE_DEFINITION(int);

typedef struct {
    int    x;
    long   y;
    double z;
    char   c;
} myspecialstruct_t;

MAYBE_DECLARATION(myspecialstruct_t);
MAYBE_DEFINITION(myspecialstruct_t);


void setUp(void) {}

void tearDown(void) {}

void test_maybe(void) {
    MAYBE(int) nothing1 = maybe_int_nothing();
    MAYBE(int) just1    = maybe_int_return(42);

    TEST_ASSERT_EQUAL(0, maybe_int_is_just(&nothing1));
    TEST_ASSERT_EQUAL(1, maybe_int_is_just(&just1));

    TEST_ASSERT_EQUAL(5, maybe_int_with_default(&nothing1, 5));
    TEST_ASSERT_EQUAL(42, maybe_int_with_default(&just1, 5));

    myspecialstruct_t def   = {1, 2, .3, 'd'};
    myspecialstruct_t value = {42, 420, 4.2, 'a'};

    MAYBE(myspecialstruct_t) nothing2 = maybe_myspecialstruct_t_nothing();
    MAYBE(myspecialstruct_t) just2    = maybe_myspecialstruct_t_return(value);

    TEST_ASSERT_EQUAL(0, maybe_myspecialstruct_t_is_just(&nothing2));
    TEST_ASSERT_EQUAL(1, maybe_myspecialstruct_t_is_just(&just2));

    myspecialstruct_t res = maybe_myspecialstruct_t_with_default(&nothing2, def);
    TEST_ASSERT_EQUAL(def.x, res.x);
    TEST_ASSERT_EQUAL(def.y, res.y);
    TEST_ASSERT_EQUAL(def.z, res.z);
    TEST_ASSERT_EQUAL(def.c, res.c);

    res = maybe_myspecialstruct_t_with_default(&just2, def);
    TEST_ASSERT_EQUAL(value.x, res.x);
    TEST_ASSERT_EQUAL(value.y, res.y);
    TEST_ASSERT_EQUAL(value.z, res.z);
    TEST_ASSERT_EQUAL(value.c, res.c);
}