#include <string.h>
#include "pid.h"

static control_type_t error(control_type_t in, control_type_t sp) {
    return sp - in;
}

void pid_init(pid_ctrl_t *pid) {
    memset(pid, 0, sizeof(pid_ctrl_t));
}

void pid_ctrl_tune(pid_ctrl_t *pid, float kp, float ki, float kd) {
    pid->Kp = kp;
    pid->Kd = kd;
    pid->Ki = ki;
}

void pid_add_input(pid_ctrl_t *pid, control_type_t value) {
    pid->input               = value;
    pid->history[pid->index] = error(pid->input, pid->sp);
    pid->index               = (pid->index + 1) % PID_HISTORY_SIZE;
}

void pid_set_sp(pid_ctrl_t *pid, control_type_t sp) {
    pid->sp = sp;
}

output_type_t pid_compute(pid_ctrl_t *pid) {
    int last = pid->index > 0 ? pid->index - 1 : PID_HISTORY_SIZE - 1;

    control_type_t pterm = pid->Kp * error(pid->input, pid->sp);

    control_type_t iterm = 0;
    for (int i = 0; i < PID_HISTORY_SIZE; i++)
        iterm += pid->history[i];
    iterm *= pid->Ki;

    control_type_t dterm = pid->Kd * (pid->input - pid->history[last]);

    return (output_type_t)(pterm + iterm + dterm);
}

void pid_clear_history(pid_ctrl_t *pid) {
    pid->index = 0;
    memset(pid->history, 0, sizeof(control_type_t) * PID_HISTORY_SIZE);
}