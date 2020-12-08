#ifndef MAYBE_H_INCLUDED
#define MAYBE_H_INCLUDED

/*
 *  Metaprogrammed maybe type for C
 * While technically MAYBE(X) is a monad and a functor, using higher order functions in C (i.e. function pointers)
 * is quite cumbersome. I just want a nullable type.
 */

#define MAYBE(TYPE) maybe_##TYPE##_t

#define MAYBE_NOTHING                                                                                                  \
    { .nothing = 1 }

#define MAYBE_DECLARATION(TYPE)                                                                                        \
    typedef struct {                                                                                                   \
        TYPE just;                                                                                                     \
        int  nothing;                                                                                                  \
    } MAYBE(TYPE);                                                                                                     \
    MAYBE(TYPE) maybe_##TYPE##_nothing(void);                                                                          \
    MAYBE(TYPE) maybe_##TYPE##_return(TYPE value);                                                                     \
    int  maybe_##TYPE##_is_just(MAYBE(TYPE) * maybe);                                                                  \
    TYPE maybe_##TYPE##_with_default(MAYBE(TYPE) * maybe, TYPE defval);                                                \
    MAYBE(TYPE) maybe_##TYPE##_map(MAYBE(TYPE) m, TYPE (*fn)(TYPE));                                                   \
    MAYBE(TYPE) maybe_##TYPE##_bind(MAYBE(TYPE) m, MAYBE(TYPE) (*fn)(TYPE));

#define MAYBE_DEFINITION(TYPE)                                                                                         \
    MAYBE(TYPE) maybe_##TYPE##_return(TYPE value) {                                                                    \
        return (maybe_##TYPE##_t){.just = value, .nothing = 0};                                                        \
    }                                                                                                                  \
    int maybe_##TYPE##_is_just(MAYBE(TYPE) * maybe) {                                                                  \
        return !maybe->nothing;                                                                                        \
    }                                                                                                                  \
    MAYBE(TYPE) maybe_##TYPE##_nothing(void) {                                                                         \
        return (maybe_##TYPE##_t){.nothing = 1};                                                                       \
    }                                                                                                                  \
    TYPE maybe_##TYPE##_with_default(MAYBE(TYPE) * maybe, TYPE defval) {                                               \
        if (maybe_##TYPE##_is_just(maybe))                                                                             \
            return maybe->just;                                                                                        \
        else                                                                                                           \
            return defval;                                                                                             \
    }                                                                                                                  \
    MAYBE(TYPE) maybe_##TYPE##_map(MAYBE(TYPE) m, TYPE (*fn)(TYPE)) {                                                  \
        if (maybe_##TYPE##_is_just(&m))                                                                                \
            return maybe_##TYPE##_return(fn(m.just));                                                                  \
        else                                                                                                           \
            return m;                                                                                                  \
    }                                                                                                                  \
    MAYBE(TYPE) maybe_##TYPE##_bind(MAYBE(TYPE) m, MAYBE(TYPE) (*fn)(TYPE)) {                                          \
        if (maybe_##TYPE##_is_just(&m))                                                                                \
            return fn(m.just);                                                                                         \
        else                                                                                                           \
            return m;                                                                                                  \
    }

#endif