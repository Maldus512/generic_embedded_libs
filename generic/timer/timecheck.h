#ifndef __TIMECHECK_H__
#define __TIMECHECK_H__

#include <stdint.h>


#define typecheck(type, x)                                                                                             \
    ({                                                                                                                 \
        type __dummy;                                                                                                  \
        typeof(x) __dummy2;                                                                                            \
        (void)(&__dummy == &__dummy2);                                                                                 \
        1;                                                                                                             \
    })

#define time_after(a, b) (typecheck(uint32_t, a) && typecheck(uint32_t, b) && ((int32_t)((b) - (a)) < 0))
#define time_before(a, b) time_after(b, a)

static inline __attribute__((always_inline)) int is_expired(uint32_t start, uint32_t current, uint32_t delay)
{
    return time_after(current, start + delay);
}
#endif