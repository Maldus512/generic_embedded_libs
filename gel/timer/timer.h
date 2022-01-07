#ifndef GEL_TIMER_H_INCLUDED
#define GEL_TIMER_H_INCLUDED

#include <stdlib.h>
#include "stopwatch.h"


#define TIMER_NULL ((timer_t){.active = 0, .autoreload = 0, .stopwatch = STOPWATCH_NULL, .callback = NULL, .arg = NULL})




typedef struct timer_struct {
    int         active;
    int         autoreload;
    stopwatch_t stopwatch;
    void (*callback)(struct timer_struct *, void *, void *);
    void *arg;
} timer_t;

typedef void (*timer_callback_t)(timer_t *, void *, void *);




timer_t *timer_activate(timer_t *timer, unsigned long period, unsigned long millis, timer_callback_t cb, void *arg);
void     timer_deactivate(timer_t *timer);
void     timer_set_autoreload(timer_t *timer, int autoreload);
int      timer_manage_callbacks(timer_t *timers, size_t num, unsigned long timestamp, void *user_pointer);
int      timer_get_first_available(timer_t *timers, size_t num);
void     timer_deactivate_all(timer_t *timers, size_t num);
void     timer_pause(timer_t *timer, unsigned long timestamp);
void     timer_pause_all(timer_t *timers, size_t num, unsigned long timestamp);
void     timer_resume(timer_t *timer, unsigned long timestamp);
void     timer_resume_all(timer_t *timers, size_t num, unsigned long timestamp);

#endif