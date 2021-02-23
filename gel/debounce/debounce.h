#ifndef DEBOUNCE_H_INCLUDED
#define DEBOUNCE_H_INCLUDED

#include <stdlib.h>

#define NUM_INPUTS ((int)sizeof(unsigned int) * 8)
#define NTH(x, i)  ((x >> i) & 0x1)

typedef struct {
    unsigned int old_input;
    unsigned int buffered;
    unsigned int debounce_count[NUM_INPUTS];
} debounce_filter_t;

/*
 *  Returns the current value of the i-th input
 *
 *  i : input to return
 *  filter: pointer to the debounce_filter_t struct
 *  return: 0 or 1
 */
static inline int debounce_read(debounce_filter_t *filter, size_t i) {
    if (i >= NUM_INPUTS)
        return -1;
    return NTH(filter->buffered, i);
}

static inline unsigned int debounce_value(debounce_filter_t *filter) {
    return filter->buffered;
}

/*
 *  Initializes a debounce filter struct. The struct holds the necessary data to
 * filter fluctuations in the input. The input is received in the form of a bitmap as
 * bit as an unsigned int (typically a word in the target system). The struct fields
 * are not encapsulated to avoid using dynamic memory.
 *
 *  filter: pointer to the debounce_filter_t struct
 */
void debounce_filter_init(debounce_filter_t *filter);

/*
 *  Filters fluctuations in the received input. The value held into the structure is modified
 * only after a change that lasted through at least debounce+1 calls.
 * The debounce effect is calculated on number of calls instead of time because the
 * focus of this tool is on stability, not on duration. Therefore, this function should
 * be called at more-or-less regular intervals.
 *
 *  filter: pointer to the debounce_filter_t struct
 *  input: bitmap containing the current input
 *  debounce: number of consecutive calls where the input must be different for its
 * value to change.
 *  return: returns 1 if there was a change
 */
unsigned int debounce_filter(debounce_filter_t *filter, unsigned int input, int debounce);

unsigned int debounce_filter_single(debounce_filter_t *filter, unsigned int input, size_t i, int debounce);

/*
 *  Sets the saved value for all inputs. Useful to reset the reading.
 *
 *  filter: pointer to the debounce_filter_t struct
 *  set: input values to reset to
 */
void debounce_filter_set(debounce_filter_t *filter, unsigned int set);

#endif
