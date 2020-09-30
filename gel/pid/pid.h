#ifndef PID_H_INCLUDED
#define PID_H_INCLUDED
#include <unistd.h>

#include "gel_conf.h"


typedef enum {
    ERROR_NEGATIVE = -1,
    ERROR_POSITIVE = +1,
} error_sign_t;


typedef struct {
    float Kp, Kd, Ki;

    size_t             index, derivative_index;
    size_t             derivative_history_size;
    pid_control_type_t history[GEL_PID_INTEGRAL_HISTORY_SIZE];
    pid_control_type_t derivative_history[GEL_PID_DERIVATIVE_DELTA];
    pid_control_type_t input;
    pid_control_type_t sp;

    pid_output_type_t integral_low_threshold, integral_high_threshold;
    pid_output_type_t derivative_low_threshold;     //, derivative_high_threshold; TODO: Check if actually useful

    int          first_value, auto_clear_integral_history;
    int          derivative_threshold;
    int          integral_high_thresh_enabled, integral_low_thresh_enabled;
    error_sign_t sign;
} pid_ctrl_t;

void              pid_init(pid_ctrl_t *pid);
void              pid_ctrl_tune(pid_ctrl_t *pid, float kp, float ki, float kd);
void              pid_add_input(pid_ctrl_t *pid, pid_control_type_t input);
void              pid_set_sp(pid_ctrl_t *pid, pid_control_type_t sp);
pid_output_type_t pid_compute(pid_ctrl_t *pid);
pid_output_type_t pid_compute_p(pid_ctrl_t *pid);
pid_output_type_t pid_compute_i(pid_ctrl_t *pid);
pid_output_type_t pid_compute_d(pid_ctrl_t *pid);
pid_output_type_t pid_current_error(pid_ctrl_t *pid);
void              pid_clear_integral_history(pid_ctrl_t *pid);

void pid_set_integral_high_threshold(pid_ctrl_t *pid, size_t threshold);
void pid_set_integral_low_threshold(pid_ctrl_t *pid, size_t threshold);
void pid_set_derivative_threshold(pid_ctrl_t *pid, pid_control_type_t low_thresh, pid_control_type_t high_thresh);
void pid_enable_derivative_threshold(pid_ctrl_t *pid, int enable);
void pid_enable_integral_low_threshold(pid_ctrl_t *pid, int enable);
void pid_enable_integral_high_threshold(pid_ctrl_t *pid, int enable);
void pid_set_auto_integral_clear(pid_ctrl_t *pid, int autoclear);

#endif