#include "pid/pid.h"
#include "pid_conf.h"
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

    pid_clear_history(pid);
    pid_add_input(pid, 90);
    TEST_ASSERT_EQUAL(0, pid_compute(pid));

    pid_clear_history(pid);
    pid_add_input(pid, 0);
    TEST_ASSERT_NOT_EQUAL(0, pid_compute(pid));
}