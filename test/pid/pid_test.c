#include "pid/pid.h"
#include "gel_conf.h"
#include "unity.h"

pid_ctrl_t  _pid;
pid_ctrl_t *pid = &_pid;

void setUp() {
    pid_init(pid);
}

void tearDown() {}

void test_set() {
    pid_ctrl_tune(pid, 0.7, 0.8, 0.1);

    TEST_ASSERT_EQUAL_FLOAT(0.7, pid->Kp);
    TEST_ASSERT_EQUAL_FLOAT(0.8, pid->Ki);
    TEST_ASSERT_EQUAL_FLOAT(0.1, pid->Kd);

    pid_set_sp(pid, 100);
    TEST_ASSERT_EQUAL(100, pid->sp);
}

void test_p() {
    pid_ctrl_tune(pid, 1, 0, 0);
    pid_set_sp(pid, 20);
    pid_add_input(pid, 10);
    TEST_ASSERT_EQUAL(10, pid_compute(pid));

    pid_ctrl_tune(pid, 0.5, 0, 0);
    TEST_ASSERT_EQUAL(5, pid_compute(pid));

    pid_add_input(pid, 20);
    TEST_ASSERT_EQUAL(0, pid_compute(pid));

    pid_add_input(pid, 25);
    TEST_ASSERT_EQUAL(-2, pid_compute(pid));
}

void test_pi() {
    pid_ctrl_tune(pid, 0.01, 0.01, 0);
    pid_set_sp(pid, 100);

    for (int i = 0; i < 9; i++) {
        pid_add_input(pid, 90);
        TEST_ASSERT_EQUAL(0, pid_compute(pid));
    }

    for (int i = 0; i < 9; i++) {
        pid_add_input(pid, 90);
        TEST_ASSERT_NOT_EQUAL(0, pid_compute(pid));
    }

    pid_clear_integral_history(pid);
    pid_add_input(pid, 90);
    TEST_ASSERT_EQUAL(0, pid_compute(pid));

    pid_clear_integral_history(pid);
    pid_add_input(pid, 0);
    TEST_ASSERT_NOT_EQUAL(0, pid_compute(pid));
}


void test_pi_threshold() {
    pid_ctrl_tune(pid, 0, 0.01, 0);
    pid_set_integral_low_threshold(pid, 50);
    pid_enable_integral_low_threshold(pid, 1);
    pid_set_integral_high_threshold(pid, 10);
    pid_enable_integral_high_threshold(pid, 1);
    pid_set_sp(pid, 100);

    for (int i = 0; i < 100; i++) {
        pid_add_input(pid, 0);
        TEST_ASSERT_EQUAL(0, pid_compute(pid));
    }

    for (int i = 0; i < 2; i++) {
        pid_add_input(pid, 60);
        TEST_ASSERT_EQUAL(0, pid_compute(pid));
    }

    for (int i = 0; i < 10; i++) {
        pid_add_input(pid, 60);
        TEST_ASSERT_NOT_EQUAL(0, pid_compute(pid));
    }

    pid_ctrl_tune(pid, 0, 1, 0);
    pid_add_input(pid, 120);
    TEST_ASSERT_NOT_EQUAL(0, pid_compute(pid));
    pid_set_auto_integral_clear(pid, 1);
    pid_add_input(pid, 50);
    TEST_ASSERT_EQUAL(0, pid_compute(pid));

    for (int i = 0; i < 100; i++) {
        pid_add_input(pid, 110 + i);
        TEST_ASSERT_EQUAL(0, pid_compute(pid));
    }

    pid_add_input(pid, 109);
    TEST_ASSERT_EQUAL(-9, pid_compute(pid));
    pid_add_input(pid, 109);
    TEST_ASSERT_EQUAL(-9 * 2, pid_compute(pid));
    pid_add_input(pid, 110);
    TEST_ASSERT_EQUAL(-9 * 2, pid_compute(pid));
    pid_add_input(pid, 150);
    TEST_ASSERT_EQUAL(-9 * 2, pid_compute(pid));
}

void test_pd() {
    pid_ctrl_tune(pid, 0, 0, 1);
    pid_set_sp(pid, 100);
    int increase = 0;

    pid_add_input(pid, 100);
    TEST_ASSERT_EQUAL(0, pid_compute(pid));

    for (size_t i = 0; i < GEL_PID_DERIVATIVE_DELTA; i++) {
        increase += 10;
        pid_add_input(pid, 100 + increase);
        int test1 = -increase;
        int test2 = pid_compute(pid);
        TEST_ASSERT_EQUAL(test1, test2);
    }

    for (size_t i = 0; i < 20; i++) {
        increase += 10;
        pid_add_input(pid, 100 + increase);
        TEST_ASSERT_EQUAL(-50, pid_compute(pid));
    }
}


void test_field_data() {
    pid_ctrl_tune(pid, 0.5, 0.1, 0);
    pid_set_sp(pid, 180);

    pid_add_input(pid, 0);
    TEST_ASSERT_EQUAL(108, pid_compute(pid));
    pid_add_input(pid, 0);
    TEST_ASSERT_EQUAL(126, pid_compute(pid));
}