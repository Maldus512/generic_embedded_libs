#ifndef GEL_TIMER_H_INCLUDED
#define GEL_TIMER_H_INCLUDED

#include <stdlib.h>
#include <stdint.h>
#include "stopwatch.h"


#define GEL_TIMER_NULL                                                                                                 \
    ((gel_timer_t){.active = 0, .autoreload = 0, .stopwatch = STOPWATCH_NULL, .callback = NULL, .arg = NULL})




typedef struct gel_timer_struct {
    uint8_t     autoreload;
    stopwatch_t stopwatch;
    void (*callback)(struct gel_timer_struct *, void *, void *);
    void *arg;
} gel_timer_t;

typedef void (*gel_timer_callback_t)(gel_timer_t *, void *, void *);




gel_timer_t *gel_timer_init(gel_timer_t *timer, unsigned long period, unsigned long millis, gel_timer_callback_t cb,
                            void *arg);
void         gel_timer_stop(gel_timer_t *timer);
void         gel_timer_set_autoreload(gel_timer_t *timer, uint8_t autoreload);
uint8_t      gel_timer_manage_callback(gel_timer_t *timer, unsigned long timestamp, void *user_pointer);
void         gel_timer_pause(gel_timer_t *timer, unsigned long timestamp);
void         gel_timer_resume(gel_timer_t *timer, unsigned long timestamp);
void         gel_timer_reset(gel_timer_t *timer, unsigned long timestamp);
void         gel_timer_change_period(gel_timer_t *timer, unsigned long period, unsigned long timestamp);

#endif
