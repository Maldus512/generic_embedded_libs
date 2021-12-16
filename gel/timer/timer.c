/*typedef struct {
    int active;
    int autoreload;
    stopwatch_t stopwatch;
    timer_callback_t callback;
    void *arg;
} timer_t;*/

#include "timer.h"
#include "timecheck.h"



void timer_activate(timer_t *timer, unsigned long period, unsigned long timestamp, timer_callback_t cb, void *arg) {
    stopwatch_init(&timer->stopwatch);
    timer->active=1;
    timer->callback=cb;
    timer->autoreload=0;
    timer->arg=arg;
    stopwatch_setngo(&timer->stopwatch,period,timestamp);
}

void timer_deactivate(timer_t *timer) {
    timer->active=0;
    stopwatch_stop(&timer->stopwatch);
}

void timer_set_autoreload(timer_t *timer, int autoreload) {
    timer->autoreload=autoreload;
}

int timer_manage_callbacks(timer_t *timers, size_t num, unsigned long timestamp) {
    int res = 0;
    for (int i=0;i<num;i++) {
        if (stopwatch_is_timer_reached(&timers[i].stopwatch, timestamp)) {
            timers[i].callback(&timers[i],timers[i].arg);
            res = 1;
            if (!timers[i].autoreload) {
                timers[i].active=0;
            } else if (timers[i].autoreload){
                timers[i].active=1;
                stopwatch_setngo(&timers[i].stopwatch,stopwatch_get_total_time(&timers[i].stopwatch),timestamp);
            }
        }
    }
    return res;
}
int timer_get_first_available(timer_t *timers, size_t num) {
    for (int i=0;i<num;i++) {
        if (timers[i].active==0) {
            return i;
        }
    }
    return -1;
}

void timer_deactivate_all(timer_t *timers,size_t num) {
    for (int i=0;i<num;i++) {
        timer_deactivate(&timers[i]);
    }
}
