#include "debounce.h"

#define NTH(x, i) ((x >> i) & 0x1)
#define SET_NTH(x, i, val) x = ((x & (~((unsigned int)(1 << i)))) | val << i)

void init_digin_filter(digin_filter_t *filter, unsigned int active, unsigned int type)
{
    int i;

    for (i = 0; i < 16; i++)
    {
        filter->filters[i] = 0;
        filter->value[i]   = 0;
    }
    filter->previous_input = 0;
    filter->is_counter     = type;
    filter->active         = active;
}


int digin_filter(digin_filter_t *filter, unsigned int input, unsigned long debounce)
{
    int i = 0, change = 0;

    for (i = 0; i < 16; i++)
    {
        if (!NTH(filter->active, i))
            continue;

        if (NTH(input, i) == NTH(filter->previous_input, i))
        {
            filter->filters[i] = 0;
        }
        else
        {
            if (filter->filters[i] > 0)
                filter->filters[i]--;
            else
                filter->filters[i] = debounce;

            if (filter->filters[i] == 0)
            {
                if (NTH(filter->is_counter, i))
                    filter->value[i]++;
                else
                    filter->value[i] = NTH(input, i);

                SET_NTH(filter->previous_input, i, NTH(input, i));
                change = 1;
            }
        }
    }
    return change;
}
