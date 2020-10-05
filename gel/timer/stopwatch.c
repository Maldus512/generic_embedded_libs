/******************************************************************************/
/*                                                                            */
/*  HSW snc - Casalecchio di Reno (BO) ITALY                                  */
/*  ----------------------------------------                                  */
/*                                                                            */
/*  modulo: generic_timer.c                                                   */
/*                                                                            */
/*      gestione timer generici in millis                                     */
/*                                                                            */
/*  Autore: Maldus (Mattia MALDINI) & Massimo ZANNA                           */
/*                                                                            */
/*  Data  : 31/01/2020      REV  : 00.0                                       */
/*                                                                            */
/*  U.mod.: 15/07/2020      REV  : 01.0                                       */
/*                                                                            */
/******************************************************************************/
/*
 *  Helper functions to manage timers. A timer is simply a structure that keeps track
 * of total set period, starting time and elapsed time in between pauses, and
 * timer state (paused, stopped or running).
 */
#include "stopwatch.h"
#include "timecheck.h"



void stopwatch_init(stopwatch_t *timer) {
    timer->starting_time = 0;
    timer->total_time    = 0;
    timer->elapsed_time  = 0;
    timer->state         = TIMER_STOPPED;
}



int stopwatch_start(stopwatch_t *timer, unsigned long timestamp) {
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



int stopwatch_stop(stopwatch_t *timer) {
    timer->starting_time = 0;
    timer->elapsed_time  = 0;
    timer->state         = TIMER_STOPPED;
    return 0;
}



int stopwatch_pause(stopwatch_t *timer, unsigned long timestamp) {
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



int stopwatch_restart(stopwatch_t *timer, unsigned long timestamp) {
    stopwatch_stop(timer);
    return stopwatch_start(timer, timestamp);
}



void stopwatch_change(stopwatch_t *timer, unsigned long period, unsigned long timestamp) {
    timer->total_time = period;
    stopwatch_restart(timer, timestamp);
}



int stopwatch_set(stopwatch_t *timer, unsigned long period) {
    if (timer->state != TIMER_STOPPED)     // Can only set a stopped timer
        return -1;

    timer->total_time = period;
    return 0;
}



STOPWATCH_STATE stopwatch_get_state(stopwatch_t *timer) {
    return timer->state;
}



unsigned long stopwatch_get_elapsed(stopwatch_t *timer, unsigned long timestamp) {
    if (timer->state == TIMER_COUNTING)
        return timer->elapsed_time + time_interval(timer->starting_time, timestamp);
    else
        return timer->elapsed_time;
}


int stopwatch_set_elapsed(stopwatch_t *timer, unsigned long elapsed) {
    if (timer->state == TIMER_STOPPED) {
        timer->elapsed_time = elapsed;
        return 0;
    } else {
        return -1;
    }
}


unsigned long stopwatch_get_remaining(stopwatch_t *timer, unsigned long timestamp) {
    unsigned long elapsed = stopwatch_get_elapsed(timer, timestamp);
    return elapsed < timer->total_time ? timer->total_time - elapsed : 0;
}



unsigned long stopwatch_get_total_time(stopwatch_t *timer) {
    return timer->total_time;
}



int stopwatch_is_timer_reached(stopwatch_t *t, unsigned long timestamp) {
    switch(t->state) {
        case TIMER_STOPPED:
            return 0;
            
        case TIMER_COUNTING: {
            if (t->total_time > t->elapsed_time) {
                unsigned long timeleft = t->total_time - t->elapsed_time;
                return is_expired(t->starting_time, timestamp, timeleft);
            } else {
                return 1;
            }
        }
        
        case TIMER_PAUSED:
            return t->elapsed_time >= t->total_time; 
    }
    return 0;
}
