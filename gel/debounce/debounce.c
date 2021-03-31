#include "debounce.h"

#define BOOL(x)            ((x) > 0)
#define SET_NTH(x, i, val) ((x & (~((unsigned int)(1 << i)))) | BOOL(val) << i)

void debounce_filter_init(debounce_filter_t *filter) {
    int i;

    filter->buffered = 0;
    for (i = 0; i < NUM_INPUTS; i++) {
        filter->debounce_count[i] = 0;
    }
    filter->old_input = 0;
}

void debounce_filter_set(debounce_filter_t *filter, unsigned int set) {
    filter->old_input = set;
    filter->buffered  = set;
}

unsigned int debounce_filter(debounce_filter_t *filter, unsigned int input, int debounce) {
    unsigned int change = 0;
    size_t i            = 0;

    for (i = 0; i < NUM_INPUTS; i++)
        change |= debounce_filter_single(filter, NTH(input, i), i, debounce);

    return change;
}

unsigned int debounce_filter_single(debounce_filter_t *filter, unsigned int input, size_t i, int debounce) {
    unsigned int change = 0;

    if (BOOL(input) == NTH(filter->old_input, i)) {
        filter->debounce_count[i] = 0;
    } else {
        if (filter->debounce_count[i] > 0)
            filter->debounce_count[i]--;
        else
            filter->debounce_count[i] = debounce;

        if (filter->debounce_count[i] == 0) {
            filter->buffered  = SET_NTH(filter->buffered, i, input);
            filter->old_input = SET_NTH(filter->old_input, i, input);
            change            = SET_NTH(change, i, 1);
        }
    }

    return change;
}