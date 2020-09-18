#include <string.h>
#include "pid.h"


#define ABS(x) ((x) > 0 ? (x) : -(x))


static pid_control_type_t calc_error(pid_control_type_t in, pid_control_type_t sp) {
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


void pid_add_input(pid_ctrl_t *pid, pid_control_type_t value) {
    pid_output_type_t olderror = pid_current_error(pid);
    pid->input                 = value;
    pid_output_type_t error    = pid_current_error(pid);

    error_sign_t newsign = pid->sign;
    if (error > 0)
        newsign = ERROR_POSITIVE;
    else if (error < 0)
        newsign = ERROR_NEGATIVE;

    if (pid->sign != newsign) {
        pid->sign = newsign;
        if (pid->auto_clear_integral_history)
            pid_clear_integral_history(pid);
    }

    // If the error is over the (respective) threshold null the integral part
    if (error > 0) {
        if (pid->integral_low_thresh_enabled == 0 || ABS(error) < pid->integral_low_threshold)
            pid->history[pid->index] = error;
        else
            pid->history[pid->index] = 0;
    } else if (error < 0) {
        if (pid->integral_high_thresh_enabled == 0 || ABS(error) < pid->integral_high_threshold)
            pid->history[pid->index] = error;
        else
            pid->history[pid->index] = 0;
    }

    pid->index = (pid->index + 1) % GEL_PID_INTEGRAL_HISTORY_SIZE;

    if (pid->first_value) {
        pid->derivative_history[pid->derivative_index] = olderror;
        pid->derivative_index                          = (pid->derivative_index + 1) % GEL_PID_DERIVATIVE_DELTA;
        if (pid->derivative_history_size < GEL_PID_DERIVATIVE_DELTA)
            pid->derivative_history_size++;
    }

    pid->first_value = 1;
}


void pid_set_sp(pid_ctrl_t *pid, pid_control_type_t sp) {
    pid->sp = sp;
}


pid_output_type_t pid_compute_p(pid_ctrl_t *pid) {
    pid_output_type_t error = pid_current_error(pid);
    return (pid_output_type_t)(pid->Kp * error);
}


pid_output_type_t pid_compute_i(pid_ctrl_t *pid) {
    pid_output_type_t iterm = 0;

    for (int i = 0; i < GEL_PID_INTEGRAL_HISTORY_SIZE; i++)
        iterm += pid->history[i];
    iterm *= pid->Ki;

    return iterm;
}


pid_output_type_t pid_compute_d(pid_ctrl_t *pid) {
    pid_output_type_t error = pid_current_error(pid);
    pid_output_type_t dterm = 0;

    if (pid->derivative_history_size == 0)
        return 0;

    size_t last;
    if (pid->derivative_history_size == GEL_PID_DERIVATIVE_DELTA)
        last = pid->derivative_index;
    else
        last = 0;     // The derivative history is not full yet; take the next best thing (oldest reading)

    pid_output_type_t derivative = (error - pid->derivative_history[last]);

    // Null the derivative component if the error is over the respective threshold
    // if (derivative < 0) {
    // Negative derivative (decreasing error) -> Check the low threshold
    if (pid->derivative_threshold && ABS(error) > pid->derivative_low_threshold)
        return 0;
    /*}  else if (derivative > 0) {
         // Positive derivative (increasing error) -> Check the high threshold
         if (pid->derivative_threshold && ABS(error) > pid->derivative_high_threshold)
             return 0;
     }*/

    dterm = pid->Kd * derivative;
    return dterm;
}


pid_output_type_t pid_compute(pid_ctrl_t *pid) {
    pid_output_type_t pterm = pid_compute_p(pid);
    pid_output_type_t iterm = pid_compute_i(pid);
    pid_output_type_t dterm = pid_compute_d(pid);

    return (pid_output_type_t)(pterm + iterm + dterm);
}


pid_output_type_t pid_current_error(pid_ctrl_t *pid) {
    return calc_error(pid->input, pid->sp);
}


void pid_clear_integral_history(pid_ctrl_t *pid) {
    pid->index = 0;
    memset(pid->history, 0, sizeof(pid_control_type_t) * GEL_PID_INTEGRAL_HISTORY_SIZE);
}


void pid_enable_integral_low_threshold(pid_ctrl_t *pid, int enable) {
    pid->integral_low_thresh_enabled = enable;
}


void pid_enable_integral_high_threshold(pid_ctrl_t *pid, int enable) {
    pid->integral_high_thresh_enabled = enable;
}


void pid_enable_derivative_threshold(pid_ctrl_t *pid, int enable) {
    pid->derivative_threshold = enable;
}


void pid_set_integral_low_threshold(pid_ctrl_t *pid, size_t threshold) {
    pid->integral_low_threshold = threshold;
}


void pid_set_integral_high_threshold(pid_ctrl_t *pid, size_t threshold) {
    pid->integral_high_threshold = threshold;
}


void pid_set_derivative_threshold(pid_ctrl_t *pid, pid_control_type_t low_thresh, pid_control_type_t high_thresh) {
    pid->derivative_low_threshold = low_thresh;
    (void)high_thresh;
    // pid->derivative_high_threshold = high_thresh;
}


void pid_set_auto_integral_clear(pid_ctrl_t *pid, int autoclear) {
    pid->auto_clear_integral_history = autoclear;
}