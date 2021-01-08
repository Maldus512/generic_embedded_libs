#include "pulsecounter.h"

#define NTH(x, i)          ((x >> i) & 0x1)
#define SET_NTH(x, i, val) x = ((x & (~((unsigned int)(1 << i)))) | val << i)

void pulse_filter_init(pulse_filter_t *filter, pulse_type_t type, unsigned int init) {
    int i;

    filter->old_input = init;
    filter->type      = type;
    for (i = 0; i < PULSE_NUM; i++)
        pulse_clear(filter, i);
}

int pulse_filter(pulse_filter_t *filter, unsigned int input, int debounce) {
    int i = 0, change = 0;

    for (i = 0; i < PULSE_NUM; i++) {
        if (NTH(input, i) == NTH(filter->old_input, i)) {
            filter->filters[i] = 0;
        } else {
            if (filter->filters[i] > 0)
                filter->filters[i]--;
            else
                filter->filters[i] = debounce;

            if (filter->filters[i] == 0) {
                switch (filter->type) {
                    case COUNT_HIGH_PULSE:
                        if (NTH(filter->old_input, i) == 1 && NTH(input, i) == 0) {
                            change = 1;
                            filter->count[i]++;
                        }

                        break;
                    case COUNT_LOW_PULSE:
                        if (NTH(filter->old_input, i) == 0 && NTH(input, i) == 1) {
                            change = 1;
                            filter->count[i]++;
                        }
                        break;
                }
                SET_NTH(filter->old_input, i, NTH(input, i));
            }
        }
    }
    return change;
}


int pulse_level(pulse_filter_t *filter, int i) {
    if (i < 0 || i >= PULSE_NUM)
        return -1;
    return NTH(filter->old_input, i);
}