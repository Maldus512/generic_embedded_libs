/*
 *  Helper functions to manage timers. A timer is simply a structure that keeps track
 * of total set period, starting time and elapsed time in between pauses, and
 * timer state (paused, stopped or running).
 */
#include "generic_timer.h"
#include "timecheck.h"

void init_generic_timer(generic_timer_t *timer) {
    timer->starting_time = 0;
    timer->total_time    = 0;
    timer->elapsed_time  = 0;
    timer->state         = TIMER_STOPPED;
}


int start_timer(generic_timer_t *timer, unsigned long timestamp) {
    switch (timer->state) {
        case TIMER_STOPPED:
        case TIMER_PAUSED:
            timer->starting_time = timestamp;
            timer->state         = TIMER_COUNTING;
            return 0;
            break;
        case TIMER_COUNTING:     // Cannot start a counting timer
            return -1;
            break;
    }
    return -1;
}


int stop_timer(generic_timer_t *timer) {
    timer->starting_time = 0;
    timer->elapsed_time  = 0;
    timer->state         = TIMER_STOPPED;
    return 0;
}


int pause_timer(generic_timer_t *timer, unsigned long timestamp) {
    switch (timer->state) {
        case TIMER_COUNTING:
            timer->state = TIMER_PAUSED;
            timer->elapsed_time += time_interval(timer->starting_time, timestamp);
            return 0;
        case TIMER_STOPPED:     // Cannot pause a stopped timer
        case TIMER_PAUSED:      // Cannot pause a paused timer
            return -1;
    }
    return -1;
}


int restart_timer(generic_timer_t *timer, unsigned long timestamp) {
    stop_timer(timer);
    return start_timer(timer, timestamp);
}


void change_timer(generic_timer_t *timer, unsigned long period) {
    timer->total_time = period;
}


int set_timer(generic_timer_t *timer, unsigned long period) {
    if (timer->state != TIMER_STOPPED)     // Can only set a stopped timer
        return -1;

    timer->total_time = period;
    return 0;
}


TIMER_STATE get_timer_state(generic_timer_t *timer) {
    return timer->state;
}


unsigned long get_elapsed_time(generic_timer_t *timer, unsigned long timestamp) {
    if (timer->state == TIMER_COUNTING)
        return timer->elapsed_time + time_interval(timer->starting_time, timestamp);
    else
        return timer->elapsed_time;
}


unsigned long get_remaining_time(generic_timer_t *timer, unsigned long timestamp) {
    unsigned long elapsed = get_elapsed_time(timer, timestamp);
    return elapsed < timer->total_time ? timer->total_time - elapsed : 0;
}


unsigned long get_total_time(generic_timer_t *timer) {
    return timer->total_time;
}


int is_timer_reached(generic_timer_t *t, unsigned long timestamp) {
    unsigned long timeleft = t->total_time - t->elapsed_time;

    return is_expired(t->starting_time, timestamp, timeleft);
}