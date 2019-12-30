#include "debounce.h"

#define SET_NTH(x, i, val) x = ((x & (~((unsigned int)(1 << i)))) | val << i)

void init_debounce_filter(debounce_filter_t *filter) {
    int i;

    filter->value = 0;
    for (i = 0; i < NUM_INPUTS; i++) {
        filter->filters[i] = 0;
    }
    filter->old_input = 0;
}

void set_debounce_filter(debounce_filter_t *filter, unsigned int set) {
    filter->old_input = set;
}

int debounce_filter(debounce_filter_t *filter, unsigned int input, int debounce) {
    int i = 0, change = 0;

    for (i = 0; i < NUM_INPUTS; i++) {
        if (NTH(input, i) == NTH(filter->old_input, i)) {
            filter->filters[i] = 0;
        } else {
            if (filter->filters[i] > 0)
                filter->filters[i]--;
            else
                filter->filters[i] = debounce;

            if (filter->filters[i] == 0) {
                SET_NTH(filter->value, i, NTH(input, i));
                SET_NTH(filter->old_input, i, NTH(input, i));
                change = 1;
            }
        }
    }
    return change;
}