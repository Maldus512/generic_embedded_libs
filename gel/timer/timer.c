#include "timer.h"
#include "timecheck.h"


gel_timer_t *gel_timer_init(gel_timer_t *timer, unsigned long period, unsigned long timestamp, gel_timer_callback_t cb,
                            void *arg) {
    stopwatch_init(&timer->stopwatch);
    timer->callback   = cb;
    timer->autoreload = 0;
    timer->arg        = arg;
    stopwatch_setngo(&timer->stopwatch, period, timestamp);
    return timer;
}


void gel_timer_reset(gel_timer_t *timer, unsigned long timestamp) {
    switch (stopwatch_get_state(&timer->stopwatch)) {
        case TIMER_COUNTING:
            stopwatch_restart(&timer->stopwatch, timestamp);
            break;

        case TIMER_PAUSED:
            stopwatch_restart(&timer->stopwatch, timestamp);
            stopwatch_pause(&timer->stopwatch, timestamp);
            break;

        case TIMER_STOPPED:
            break;
    }
}


void gel_timer_stop(gel_timer_t *timer) {
    stopwatch_stop(&timer->stopwatch);
}


void gel_timer_resume(gel_timer_t *timer, unsigned long timestamp) {
    stopwatch_start(&timer->stopwatch, timestamp);
}


void gel_timer_set_autoreload(gel_timer_t *timer, uint8_t autoreload) {
    timer->autoreload = autoreload;
}


void gel_timer_pause(gel_timer_t *timer, unsigned long timestamp) {
    stopwatch_pause(&timer->stopwatch, timestamp);
}


void gel_timer_change_period(gel_timer_t *timer, unsigned long period, unsigned long timestamp) {
    switch (stopwatch_get_state(&timer->stopwatch)) {
        case TIMER_COUNTING:
            stopwatch_change(&timer->stopwatch, period, timestamp);
            break;

        case TIMER_PAUSED:
            stopwatch_change(&timer->stopwatch, period, timestamp);
            stopwatch_pause(&timer->stopwatch, timestamp);
            break;

        case TIMER_STOPPED:
            break;
    }
}


uint8_t gel_timer_manage_callback(gel_timer_t *timer, unsigned long timestamp, void *user_pointer) {
    if (stopwatch_is_timer_reached(&timer->stopwatch, timestamp)) {
        timer->callback(timer, user_pointer, timer->arg);
        if (timer->autoreload) {
            stopwatch_restart(&timer->stopwatch, timestamp);
        } else {
            stopwatch_stop(&timer->stopwatch);
        }
        return 1;
    } else {
        return 0;
    }
}


unsigned long gel_timer_get_remaining(gel_timer_t *timer, unsigned long timestamp) {
    return stopwatch_get_remaining(&timer->stopwatch, timestamp);
}