#ifndef PARAMETER_H_INCLUDED
#define PARAMETER_H_INCLUDED

#include <stdlib.h>
#include <stdint.h>

#include "gel_conf.h"

#define _PARAMETER_VALUE_TYPE(x)                                                                                       \
    _Generic((x), uint8_t                                                                                              \
             : PARAMETER_TYPE_UINT8, int8_t                                                                            \
             : PARAMETER_TYPE_INT8, uint16_t                                                                           \
             : PARAMETER_TYPE_UINT16, int16_t                                                                          \
             : PARAMETER_TYPE_INT16, uint32_t                                                                          \
             : PARAMETER_TYPE_UINT32, int32_t                                                                          \
             : PARAMETER_TYPE_INT32, uint64_t                                                                          \
             : PARAMETER_TYPE_UINT64, int64_t                                                                          \
             : PARAMETER_TYPE_INT64, float                                                                             \
             : PARAMETER_TYPE_FLOAT, double                                                                            \
             : PARAMETER_TYPE_DOUBLE, default : 0)

#define _PARAMETER_LIMIT_VALUE_OPTION(ref, x)                                                                          \
    _Generic((ref), uint8_t                                                                                            \
             : (parameter_type_union_t){.u8 = (uint8_t)((uint8_t)x & 0xFF)}, int8_t                                    \
             : (parameter_type_union_t){.i8 = (int8_t)((int8_t)x & 0xFF)}, uint16_t                                    \
             : (parameter_type_union_t){.u16 = (uint16_t)x}, int16_t                                                   \
             : (parameter_type_union_t){.i16 = (int16_t)x}, uint32_t                                                   \
             : (parameter_type_union_t){.u32 = (uint32_t)x}, int32_t                                                   \
             : (parameter_type_union_t){.i32 = (int32_t)x}, uint64_t                                                   \
             : (parameter_type_union_t){.u64 = (uint64_t)x}, int64_t                                                   \
             : (parameter_type_union_t){.i64 = (int64_t)x}, float                                                      \
             : (parameter_type_union_t){.f = (float)x}, double                                                         \
             : (parameter_type_union_t){.d = (double)x})

#define PARAMETER_TYPE_UINT8_CAST(x)  ((parameter_type_union_t){.u8 = x})
#define PARAMETER_TYPE_INT8_CAST(x)   ((parameter_type_union_t){.i8 = x})
#define PARAMETER_TYPE_UINT16_CAST(x) ((parameter_type_union_t){.u16 = x})
#define PARAMETER_TYPE_INT16_CAST(x)  ((parameter_type_union_t){.i16 = x})
#define PARAMETER_TYPE_UINT32_CAST(x) ((parameter_type_union_t){.u32 = x})
#define PARAMETER_TYPE_INT32_CAST(x)  ((parameter_type_union_t){.i32 = x})
#define PARAMETER_TYPE_UINT64_CAST(x) ((parameter_type_union_t){.u64 = x})
#define PARAMETER_TYPE_INT64_CAST(x)  ((parameter_type_union_t){.i64 = x})
#define PARAMETER_TYPE_FLOAT_CAST(x)  ((parameter_type_union_t){.f = x})
#define PARAMETER_TYPE_DOUBLE_CAST(x) ((parameter_type_union_t){.d = x})

#define PARAMETER_C99(type, ptr, pmin, pmax, min, max, def, step, lvl, udata, runtime, arg)                            \
    ((parameter_handle_t){(type), (ptr), (pmin), (pmax), type##_CAST(min), type##_CAST(max), type##_CAST(def),         \
                          type##_CAST(step), (lvl), (udata), (runtime), (arg)})

#define PARAMETER_FULL(ptr, pmin, pmax, min, max, def, step, lvl, udata, runtime, arg)                                 \
    ((parameter_handle_t){_PARAMETER_VALUE_TYPE(*ptr), (ptr), (pmin), (pmax),                                          \
                          _PARAMETER_LIMIT_VALUE_OPTION(*(ptr), (min)), _PARAMETER_LIMIT_VALUE_OPTION(*(ptr), (max)),  \
                          _PARAMETER_LIMIT_VALUE_OPTION(*(ptr), (def)), _PARAMETER_LIMIT_VALUE_OPTION(*(ptr), (step)), \
                          (lvl), (udata), (runtime), (arg)})

#define PARAMETER(ptr, min, max, def, udata, lvl)                                                                      \
    PARAMETER_FULL(ptr, NULL, NULL, min, max, def, 1, lvl, udata, NULL, NULL)
#define PARAMETER_STEP(ptr, min, max, def, step, udata, lvl)                                                           \
    PARAMETER_FULL(ptr, NULL, NULL, min, max, def, step, lvl, udata, NULL, NULL)
#define PARAMETER_DLIMITS(ptr, pmin, pmax, min, max, def, udata, lvl)                                                  \
    PARAMETER_FULL(ptr, pmin, pmax, min, max, def, 1, lvl, udata, NULL, NULL)



typedef enum {
    PARAMETER_TYPE_UINT8 = 0,
    PARAMETER_TYPE_INT8  = 1,
#if GEL_PARAMETER_MAX_SIZE >= 2
    PARAMETER_TYPE_UINT16 = 2,
    PARAMETER_TYPE_INT16  = 3,
#endif
#if GEL_PARAMETER_MAX_SIZE >= 4
    PARAMETER_TYPE_UINT32 = 4,
    PARAMETER_TYPE_INT32  = 5,
#endif
#if GEL_PARAMETER_MAX_SIZE >= 8
    PARAMETER_TYPE_UINT64 = 6,
    PARAMETER_TYPE_INT64  = 7,
#endif
#if GEL_PARAMETER_MAX_SIZE >= 4
    PARAMETER_TYPE_FLOAT = 8,
#endif
#if GEL_PARAMETER_MAX_SIZE >= 8
    PARAMETER_TYPE_DOUBLE = 9,
#endif
} parameter_type_t;


typedef union {
    uint8_t u8;
    int8_t  i8;
#if GEL_PARAMETER_MAX_SIZE >= 2
    uint16_t u16;
    int16_t  i16;
#endif
#if GEL_PARAMETER_MAX_SIZE >= 4
    uint32_t u32;
    int32_t  i32;
#endif
#if GEL_PARAMETER_MAX_SIZE >= 8
    uint64_t u64;
    int64_t  i64;
#endif
#if GEL_PARAMETER_MAX_SIZE >= 4
    float f;
#endif
#if GEL_PARAMETER_MAX_SIZE >= 8
    double d;
#endif
} parameter_type_union_t;


typedef struct _parameter_handle_t {
    parameter_type_t       type;
    void *                 pointer;
    void *                 pmin, *pmax;
    parameter_type_union_t min, max, defaultv, step;
    unsigned int           access_level;

    parameter_user_data_t data;

    void (*runtime)(struct _parameter_handle_t *, void *);
    void *arg;
} parameter_handle_t;

int                   parameter_operator(parameter_handle_t *handle, int mod);
size_t                parameter_get_count(parameter_handle_t *ps, size_t length, unsigned int al);
parameter_handle_t *  parameter_get_handle(parameter_handle_t *ps, size_t length, size_t num, unsigned int al);
parameter_user_data_t parameter_get_user_data(parameter_handle_t *handle);
void                  parameter_reset_to_defaults(parameter_handle_t *ps, size_t length);
int                   parameter_check_ranges(parameter_handle_t *ps, size_t length);
void                  parameter_to_string_format(parameter_handle_t *handle, char *result, char *format);
size_t                parameter_to_index(parameter_handle_t *handle);

#endif