#ifndef __TIMECHECK_H__
#define __TIMECHECK_H__

#define typecheck(type, x)                                                                                             \
    ({                                                                                                                 \
        type __dummy;                                                                                                  \
        typeof(x) __dummy2;                                                                                            \
        (void)(&__dummy == &__dummy2);                                                                                 \
        1;                                                                                                             \
    })

#define time_after(a, b) (typecheck(unsigned int, a) && typecheck(unsigned int, b) && ((int32_t)((b) - (a)) < 0))
#define time_before(a, b) time_after(b, a)

int is_expired(unsigned int start, unsigned int current, unsigned int delay)
{
    return time_after(current, start + delay);
}
#endif