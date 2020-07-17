#ifndef PID_H_INCLUDED
#define PID_H_INCLUDED

#include "gel_conf.h"

typedef struct {
    float Kp, Kd, Ki;

    int            index;
    pid_control_type_t history[PID_HISTORY_SIZE];

    pid_control_type_t input;
    pid_control_type_t sp;
} pid_ctrl_t;

void          pid_init(pid_ctrl_t *pid);
void          pid_ctrl_tune(pid_ctrl_t *pid, float kp, float ki, float kd);
void          pid_add_input(pid_ctrl_t *pid, pid_control_type_t input);
void          pid_set_sp(pid_ctrl_t *pid, pid_control_type_t sp);
pid_output_type_t pid_compute(pid_ctrl_t *pid);
void          pid_clear_history(pid_ctrl_t *pid);

#endif