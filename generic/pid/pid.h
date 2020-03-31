#ifndef PID_H_INCLUDED
#define PID_H_INCLUDED

#include "pid_conf.h"

typedef struct {
    float Kp, Kd, Ki;

    int            index;
    control_type_t history[PID_HISTORY_SIZE];

    control_type_t input;
    control_type_t sp;
} pid_t;

void          init_pid(pid_t *pid);
void          pid_tune(pid_t *pid, float kp, float ki, float kd);
void          pid_add_input(pid_t *pid, control_type_t input);
void          pid_set_sp(pid_t *pid, control_type_t sp);
output_type_t pid_compute(pid_t *pid);
void          pid_clear_history(pid_t *pid);

#endif