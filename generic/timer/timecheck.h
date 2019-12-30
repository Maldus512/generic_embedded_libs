#ifndef __TIMECHECK_H__
#define __TIMECHECK_H__

#include <stdint.h>

typedef unsigned long timestamp_t;

#define typecheck(type, x)                                                                                             \
    ({                                                                                                                 \
        type __dummy;                                                                                                  \
        typeof(x) __dummy2;                                                                                            \
        (void)(&__dummy == &__dummy2);                                                                                 \
        1;                                                                                                             \
    })

#define time_after(a, b)  (typecheck(unsigned long, a) && typecheck(unsigned long, b) && ((long)((b) - (a)) < 0))
#define time_before(a, b) time_after(b, a)

static inline __attribute__((always_inline)) unsigned long time_interval(unsigned long a, unsigned long b) {
    if (time_after(a, b))
        return -((unsigned long)b - (unsigned long)a);
    else
        return (unsigned long)b - (unsigned long)a;
}

static inline __attribute__((always_inline)) int is_expired(unsigned long start, unsigned long current,
                                                            unsigned long delay) {
    return time_after(current, start + delay);
}

#endif
