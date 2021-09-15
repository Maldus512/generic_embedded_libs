#ifndef TIMECHECK_H_INCLUDED
#define TIMECHECK_H_INCLUDED

#include <stdint.h>

typedef unsigned long timestamp_t;

#ifdef GEL_IGNORE_TIME_TYPECHECK
#define typecheck(type, x) 1
#else
#define typecheck(type, x)                                                                                             \
    ({                                                                                                                 \
        type __dummy;                                                                                                  \
        typeof(x) __dummy2;                                                                                            \
        (void)(&__dummy == &__dummy2);                                                                                 \
        1;                                                                                                             \
    })
#endif

#define time_after(a, b) (typecheck(unsigned long, a) && typecheck(unsigned long, b) && ((long)((b) - (a)) < 0))
#define time_after_or_equal(a, b)                                                                                      \
    (typecheck(unsigned long, a) && typecheck(unsigned long, b) && ((long)((b) - (a)) <= 0))
#define time_before(a, b) time_after(b, a)

#define is_expired(start, current, delay) is_loosely_expired(start, current, delay)


static inline __attribute__((always_inline)) unsigned long time_interval(unsigned long a, unsigned long b) {
    if (time_after(a, b))
        return -((unsigned long)b - (unsigned long)a);
    else
        return (unsigned long)b - (unsigned long)a;
}


static inline __attribute__((always_inline)) int is_loosely_expired(unsigned long start, unsigned long current,
                                                                    unsigned long delay) {
    return time_after_or_equal(current, start + delay);
}


static inline __attribute__((always_inline)) int is_strictly_expired(unsigned long start, unsigned long current,
                                                                     unsigned long delay) {
    return time_after(current, start + delay);
}


#endif /* __TIMECHECK_H__ */
