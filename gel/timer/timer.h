#ifndef GEL_TIMER_H_INCLUDED
#define GEL_TIMER_H_INCLUDED

#include <stdlib.h>
#include "stopwatch.h"


#define GEL_TIMER_NULL                                                                                                 \
    ((gel_timer_t){.active = 0, .autoreload = 0, .stopwatch = STOPWATCH_NULL, .callback = NULL, .arg = NULL})




typedef struct gel_timer_struct {
    int         active;
    int         autoreload;
    stopwatch_t stopwatch;
    void (*callback)(struct gel_timer_struct *, void *, void *);
    void *arg;
} gel_timer_t;

typedef void (*gel_timer_callback_t)(gel_timer_t *, void *, void *);




gel_timer_t *gel_timer_activate(gel_timer_t *timer, unsigned long period, unsigned long millis, gel_timer_callback_t cb,
                                void *arg);
void         gel_timer_deactivate(gel_timer_t *timer);
void         gel_timer_set_autoreload(gel_timer_t *timer, int autoreload);
int          gel_timer_manage_callbacks(gel_timer_t *timers, size_t num, unsigned long timestamp, void *user_pointer);
int          gel_timer_get_first_available(gel_timer_t *timers, size_t num);
void         gel_timer_deactivate_all(gel_timer_t *timers, size_t num);
void         gel_timer_pause(gel_timer_t *timer, unsigned long timestamp);
void         gel_timer_pause_all(gel_timer_t *timers, size_t num, unsigned long timestamp);
void         gel_timer_resume(gel_timer_t *timer, unsigned long timestamp);
void         gel_timer_resume_all(gel_timer_t *timers, size_t num, unsigned long timestamp);
void         gel_timer_reset(gel_timer_t *timer, unsigned long timestamp);

#endif