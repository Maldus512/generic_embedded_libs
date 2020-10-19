/******************************************************************************/
/*                                                                            */
/*  HSW snc - Casalecchio di Reno (BO) ITALY                                  */
/*  ----------------------------------------                                  */
/*                                                                            */
/*  modulo: generic_timer.h                                                   */
/*                                                                            */
/*      gestione timer generici in millis                                     */
/*                                                                            */
/*  Autore: Maldus (Mattia MALDINI) & Massimo ZANNA                           */
/*                                                                            */
/*  Data  : 31/01/2020      REV  : 00.0                                       */
/*                                                                            */
/*  U.mod.: 10/08/2020      REV  : 01.0                                       */
/*                                                                            */
/******************************************************************************/

#ifndef STOPWATCH_H_INCLUDED
#define STOPWATCH_H_INCLUDED

#define STOPWATCH_NULL  {.starting_time = 0, .elapsed_time = 0, .total_time = 0, .state = TIMER_STOPPED}

#define stopwatch_setngo(timer, duration, timestamp) stopwatch_set(timer, duration); stopwatch_start(timer, timestamp)

// Three possible states of a timer
typedef enum {
    TIMER_STOPPED = 0,
    TIMER_COUNTING,
    TIMER_PAUSED,
} STOPWATCH_STATE;

// No data hiding to avoid depending on dynamic memory
typedef struct {
    unsigned long starting_time;
    unsigned long elapsed_time;
    unsigned long total_time;
    STOPWATCH_STATE   state;
} stopwatch_t;

/*
 *  Initializes the timer struct
 * timer: pointer to the timer struct to initialize
 */
void stopwatch_init(stopwatch_t *timer);

/*
 *  Starts the timer.
 * timer: pointer to the timer struct to initialize
 * timestamp: current time
 * return: -1 if the timer is already running, 0 otherwise
 */
int stopwatch_start(stopwatch_t *timer, unsigned long timestamp);

/*
 *  Stops the timer. This resets the elapsed time to 0.
 * timer: pointer to the timer struct to initialize
 * timestamp: current time
 * return: -1 if the timer is already stopped, 0 otherwise
 */
int stopwatch_stop(stopwatch_t *timer);

/*
 *  Pauses the timer. This does not change the elapsed time.
 * timer: pointer to the timer struct to initialize
 * timestamp: current time
 * return: -1 if the timer is not running, 0 otherwise
 */
int stopwatch_pause(stopwatch_t *timer, unsigned long timestamp);

/*
 *  Restarts the timer (setting the elapsed time to 0).
 * timer: pointer to the timer struct to initialize
 * timestamp: current time
 * return: 0
 */
int stopwatch_restart(stopwatch_t *timer, unsigned long timestamp);

/*
 *  Sets the target period to be reached. It only works on stopped timers.
 * timer: pointer to the timer struct to initialize
 * period: period of time the timer should run for
 * return: -1 if the timer is not stopped, 0 otherwise
 */
int stopwatch_set(stopwatch_t *timer, unsigned long period);

/*
 *  Similar to `stopwatch_set` but with no restrictions on the timer's state.
 * timer: pointer to the timer struct to initialize
 * period: period of time the timer should run for
 * timestamp: current time
 */
void stopwatch_change(stopwatch_t *timer, unsigned long period, unsigned long timestamp);

/*
 *  Returns the state of the timer.
 * timer: pointer to the timer struct to initialize
 * return: current state of the timer
 */
STOPWATCH_STATE stopwatch_get_state(stopwatch_t *timer);

/*
 *  Returns the set period for the timer
 * timer: pointer to the timer struct to initialize
 * return: total set period
 */
unsigned long stopwatch_get_total_time(stopwatch_t *timer);

/*
 *  Returns the elapsed time. Note that the elapsed time keeps counting regardless
 * of whether the timer has reached its set period or not.
 * timer: pointer to the timer struct to initialize
 * timestamp: current time
 * return: elapsed time
 */
unsigned long stopwatch_get_elapsed(stopwatch_t *timer, unsigned long timestamp);

/*
 *  Setting elapsed time 
 */
int stopwatch_set_elapsed(stopwatch_t *timer, unsigned long elapsed);

/*
 *  Returns the remaining time from the set period. If the set period has been reached,
 * always returns 0.
 * timer: pointer to the timer struct to initialize
 * timestamp: current time
 * return: remaining time
 */
unsigned long stopwatch_get_remaining(stopwatch_t *timer, unsigned long timestamp);

/*
 *  Returns 1 if the set period has been reached, 0 otherwise.
 * timer: pointer to the timer struct to initialize
 * timestamp: current time
 * return: 1 if the timer has reached its end, 0 otherwise
 */
int stopwatch_is_timer_reached(stopwatch_t *timer, unsigned long timestamp);

#endif  /* __GENERIC_TIMER_H__ */
