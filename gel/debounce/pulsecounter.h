#ifndef __PULSECOUNTER_H__
#define __PULSECOUNTER_H__

#define PULSE_NUM ((int)sizeof(unsigned int) * 8)

typedef enum {
    COUNT_HIGH_PULSE,
    COUNT_LOW_PULSE,
} pulse_type_t;

typedef struct {
    pulse_type_t type;
    unsigned int old_input;
    unsigned int filters[PULSE_NUM];
    unsigned int count[PULSE_NUM];
} pulse_filter_t;

/*
 *  Returns the current pulse count for the i-th counter. It does not change it
 *
 *  i: i-th counter
 *  filter: pointer to the pulse_type_t struct
 *  return: the number of pulses counted
 */
static inline int pulse_count(pulse_filter_t *filter, int i) {
    if (i < 0 || i >= PULSE_NUM)
        return -1;
    return filter->count[i];
}

/*
 *  Returns the current pulse level for the i-th counter.
 *
 *  i: i-th counter
 *  filter: pointer to the pulse_type_t struct
 *  return: 0 or 1, or -1 if the index is out of range
 */
int pulse_level(pulse_filter_t *filter, int i);

/*
 *  Clears the pulse count for the i-th counter
 *
 *  i: i-th counter to clear
 *  filter: pointer to the pulse_type_t struct
 */
static inline void pulse_clear(pulse_filter_t *filter, int i) {
    if (i < 0 || i >= PULSE_NUM)
        return;
    filter->count[i] = 0;
}

/*
 *  Initializes the pulse_filter_t struct. The struct is not encapsulated to avoid
 * using dynamic memory.
 *
 *  filter: pointer to the pulse_type_t struct
 *  type: type of the pulses to count (high-to-low or low-to-high)
 */
void pulse_filter_init(pulse_filter_t *filter, pulse_type_t type, unsigned int init);

/*
 *  Function to call periodically to enact the filtering of irrelevant fluctuations.
 * The counter held into the structure is updated
 * only after a change that lasted through at least debounce+1 calls.
 * The debounce effect is calculated on number of calls instead of time because the
 * focus of this tool is on stability, not on duration. Therefore, this function should
 * be called at more-or-less regular intervals.
 *
 *  filter: pointer to the pulse_type_t struct
 *  input: bitmap containing the current input
 *  debounce: number of consecutive calls where the input must be different for its
 * value to change.
 *  return: returns 1 if there was a change
 */
int pulse_filter(pulse_filter_t *filter, unsigned int input, int debounce);

#endif