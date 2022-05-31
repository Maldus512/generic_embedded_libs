#include "timer.h"
#include "timecheck.h"


gel_timer_t *gel_timer_activate(gel_timer_t *timer, unsigned long period, unsigned long timestamp,
                                gel_timer_callback_t cb, void *arg) {
    stopwatch_init(&timer->stopwatch);
    timer->active     = 1;
    timer->callback   = cb;
    timer->autoreload = 0;
    timer->arg        = arg;
    stopwatch_setngo(&timer->stopwatch, period, timestamp);
    return timer;
}


void gel_timer_reset(gel_timer_t *timer, unsigned long timestamp) {
    if (timer->active) {
        stopwatch_restart(&timer->stopwatch, timestamp);
    }
}


void gel_timer_deactivate(gel_timer_t *timer) {
    timer->active = 0;
    stopwatch_stop(&timer->stopwatch);
}


void gel_timer_resume(gel_timer_t *timer, unsigned long timestamp) {
    stopwatch_start(&timer->stopwatch, timestamp);
}


void gel_timer_set_autoreload(gel_timer_t *timer, int autoreload) {
    timer->autoreload = autoreload;
}


void gel_timer_pause(gel_timer_t *timer, unsigned long timestamp) {
    stopwatch_pause(&timer->stopwatch, timestamp);
}


int gel_timer_manage_callbacks(gel_timer_t *timers, size_t num, unsigned long timestamp, void *user_pointer) {
    int res = 0;
    for (size_t i = 0; i < num; i++) {
        if (!timers[i].active) {
            continue;
        }

        if (stopwatch_is_timer_reached(&timers[i].stopwatch, timestamp)) {
            timers[i].callback(&timers[i], user_pointer, timers[i].arg);
            res = 1;
            if (timers[i].autoreload) {
                timers[i].active = 1;
                stopwatch_restart(&timers[i].stopwatch, timestamp);
            } else {
                timers[i].active = 0;
            }
        }
    }
    return res;
}


int gel_timer_get_first_available(gel_timer_t *timers, size_t num) {
    for (size_t i = 0; i < num; i++) {
        if (timers[i].active == 0) {
            return i;
        }
    }
    return -1;
}


unsigned long gel_timer_get_remaining(gel_timer_t *timer, unsigned long timestamp) {
    return stopwatch_get_remaining(&timer->stopwatch, timestamp);
}


void gel_timer_deactivate_all(gel_timer_t *timers, size_t num) {
    for (size_t i = 0; i < num; i++) {
        gel_timer_deactivate(&timers[i]);
    }
}


void gel_timer_pause_all(gel_timer_t *timers, size_t num, unsigned long timestamp) {
    for (size_t i = 0; i < num; i++) {
        if (timers[i].active) {
            gel_timer_pause(&timers[i], timestamp);
        }
    }
}


void gel_timer_resume_all(gel_timer_t *timers, size_t num, unsigned long timestamp) {
    for (size_t i = 0; i < num; i++) {
        if (timers[i].active) {
            gel_timer_resume(&timers[i], timestamp);
        }
    }
}
