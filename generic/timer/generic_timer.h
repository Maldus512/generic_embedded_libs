#ifndef __GENERIC_TIMER_H__
#define __GENERIC_TIMER_H__

#define NULL_TIMER  {.starting_time = 0, .elapsed_time = 0, .total_time = 0, .state = TIMER_STOPPED}

#define setngo(timer, duration, timestamp) set_timer(timer, duration); start_timer(timer, timestamp)

// Three possible states of a timer
typedef enum {
    TIMER_STOPPED = 0,
    TIMER_COUNTING,
    TIMER_PAUSED,
} TIMER_STATE;

// No data hiding to avoid depending on dynamic memory
typedef struct {
    unsigned long starting_time;
    unsigned long elapsed_time;
    unsigned long total_time;
    TIMER_STATE   state;
} generic_timer_t;

/*
 *  Initializes the timer struct
 * timer: pointer to the timer struct to initialize
 */
void init_generic_timer(generic_timer_t *timer);

/*
 *  Starts the timer.
 * timer: pointer to the timer struct to initialize
 * timestamp: current time
 * return: -1 if the timer is already running, 0 otherwise
 */
int start_timer(generic_timer_t *timer, unsigned long timestamp);

/*
 *  Stops the timer. This resets the elapsed time to 0.
 * timer: pointer to the timer struct to initialize
 * timestamp: current time
 * return: -1 if the timer is already stopped, 0 otherwise
 */
int stop_timer(generic_timer_t *timer);

/*
 *  Pauses the timer. This does not change the elapsed time.
 * timer: pointer to the timer struct to initialize
 * timestamp: current time
 * return: -1 if the timer is not running, 0 otherwise
 */
int pause_timer(generic_timer_t *timer, unsigned long timestamp);

/*
 *  Restarts the timer (setting the elapsed time to 0).
 * timer: pointer to the timer struct to initialize
 * timestamp: current time
 * return: 0
 */
int restart_timer(generic_timer_t *timer, unsigned long timestamp);

/*
 *  Sets the target period to be reached. It only works on stopped timers.
 * timer: pointer to the timer struct to initialize
 * period: period of time the timer should run for
 * return: -1 if the timer is not stopped, 0 otherwise
 */
int set_timer(generic_timer_t *timer, unsigned long period);

/*
 *  Similar to `set_timer` but with no restrictions on the timer's state.
 * timer: pointer to the timer struct to initialize
 * period: period of time the timer should run for
 */
void change_timer(generic_timer_t *timer, unsigned long period);

/*
 *  Returns the state of the timer.
 * timer: pointer to the timer struct to initialize
 * return: current state of the timer
 */
TIMER_STATE get_timer_state(generic_timer_t *timer);

/*
 *  Returns the set period for the timer
 * timer: pointer to the timer struct to initialize
 * return: total set period
 */
unsigned long get_total_time(generic_timer_t *timer);

/*
 *  Returns the elapsed time. Note that the elapsed time keeps counting regardless
 * of whether the timer has reached its set period or not.
 * timer: pointer to the timer struct to initialize
 * timestamp: current time
 * return: elapsed time
 */
unsigned long get_elapsed_time(generic_timer_t *timer, unsigned long timestamp);

/*
 *  Returns the remaining time from the set period. If the set period has been reached,
 * always returns 0.
 * timer: pointer to the timer struct to initialize
 * timestamp: current time
 * return: remaining time
 */
unsigned long get_remaining_time(generic_timer_t *timer, unsigned long timestamp);

/*
 *  Returns 1 if the set period has been reached, 0 otherwise.
 * timer: pointer to the timer struct to initialize
 * timestamp: current time
 * return: 1 if the timer has reached its end, 0 otherwise
 */
int is_timer_reached(generic_timer_t *timer, unsigned long timestamp);

#endif