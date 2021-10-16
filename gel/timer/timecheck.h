#ifndef TIMECHECK_H_INCLUDED
#define TIMECHECK_H_INCLUDED

#include <stdint.h>
#ifdef GEL_TIMECHECK_CONFIGURATION_HEADER
#include GEL_TIMECHECK_CONFIGURATION_HEADER
#endif

#ifndef GEL_CUSTOM_TIMESTAMP_TYPE
#define GEL_CUSTOM_TIMESTAMP_TYPE unsigned long
#endif

typedef GEL_CUSTOM_TIMESTAMP_TYPE timestamp_t;

#ifdef GEL_IGNORE_TIME_TYPECHECK
#define typecheck(type, x) 1
#else
#define typecheck(type, x)                                                                                             \
    ({                                                                                                                 \
        type      __dummy;                                                                                             \
        typeof(x) __dummy2;                                                                                            \
        (void)(&__dummy == &__dummy2);                                                                                 \
        1;                                                                                                             \
    })
#endif

#define time_after(a, b)          (typecheck(timestamp_t, a) && typecheck(timestamp_t, b) && ((long)((b) - (a)) < 0))
#define time_after_or_equal(a, b) (typecheck(timestamp_t, a) && typecheck(timestamp_t, b) && ((long)((b) - (a)) <= 0))
#define time_before(a, b)         time_after(b, a)

#define is_expired(start, current, delay) is_loosely_expired(start, current, delay)


static inline __attribute__((always_inline)) timestamp_t time_interval(timestamp_t a, timestamp_t b) {
    if (time_after(a, b))
        return -((timestamp_t)b - (timestamp_t)a);
    else
        return (timestamp_t)b - (timestamp_t)a;
}


static inline __attribute__((always_inline)) int is_loosely_expired(timestamp_t start, timestamp_t current,
                                                                    timestamp_t delay) {
    return time_after_or_equal(current, start + delay);
}


static inline __attribute__((always_inline)) int is_strictly_expired(timestamp_t start, timestamp_t current,
                                                                     timestamp_t delay) {
    return time_after(current, start + delay);
}


#endif /* __TIMECHECK_H__ */
