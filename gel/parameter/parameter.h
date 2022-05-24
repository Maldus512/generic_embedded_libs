#ifndef PARAMETER_H_INCLUDED
#define PARAMETER_H_INCLUDED

#include <stdlib.h>
#include <stdint.h>
#include "../gel_internal_conf.h"


#ifdef GEL_SIMPLE_CONFIG_INCLUSION
#include "gel_conf.h"
#elif defined GEL_PARAMETER_CONFIGURATION_HEADER
#include GEL_PARAMETER_CONFIGURATION_HEADER
#endif

#ifndef GEL_PARAMETER_HANDLE_ATTRIBUTES
#define GEL_PARAMETER_HANDLE_ATTRIBUTES
#endif

#ifndef GEL_PARAMETER_MAX_SIZE
#define GEL_PARAMETER_MAX_SIZE 8
#endif

#ifndef GEL_PARAMETER_USER_DATA
#define GEL_PARAMETER_USER_DATA void *
#endif

#if GEL_PARAMETER_MAX_SIZE == 0
#error "Parameter size should be at least 1"
#endif
#if GEL_PARAMETER_MAX_SIZE >= 2
#define _PARAMETER_TYPE_UINT16 PARAMETER_TYPE_UINT16
#define _PARAMETER_TYPE_INT16  PARAMETER_TYPE_INT16
#else
#define _PARAMETER_TYPE_UINT16 PARAMETER_TYPE_UINT8
#define _PARAMETER_TYPE_INT16  PARAMETER_TYPE_INT8
#endif
#if GEL_PARAMETER_MAX_SIZE >= 4
#define _PARAMETER_TYPE_UINT32 PARAMETER_TYPE_UINT32
#define _PARAMETER_TYPE_INT32  PARAMETER_TYPE_INT32
#define _PARAMETER_TYPE_FLOAT  PARAMETER_TYPE_FLOAT
#else
#define _PARAMETER_TYPE_UINT32 PARAMETER_TYPE_UINT8
#define _PARAMETER_TYPE_INT32  PARAMETER_TYPE_INT8
#define _PARAMETER_TYPE_FLOAT  PARAMETER_TYPE_INT8
#endif
#if GEL_PARAMETER_MAX_SIZE >= 8
#define _PARAMETER_TYPE_UINT64 PARAMETER_TYPE_UINT64
#define _PARAMETER_TYPE_INT64  PARAMETER_TYPE_INT64
#define _PARAMETER_TYPE_DOUBLE PARAMETER_TYPE_DOUBLE
#else
#define _PARAMETER_TYPE_UINT64 PARAMETER_TYPE_UINT8
#define _PARAMETER_TYPE_INT64  PARAMETER_TYPE_INT8
#define _PARAMETER_TYPE_DOUBLE PARAMETER_TYPE_INT8
#endif



#define _PARAMETER_VALUE_TYPE(x)                                                                                       \
    _Generic((x), uint8_t                                                                                              \
             : PARAMETER_TYPE_UINT8, int8_t                                                                            \
             : PARAMETER_TYPE_INT8, uint16_t                                                                           \
             : _PARAMETER_TYPE_UINT16, int16_t                                                                         \
             : _PARAMETER_TYPE_INT16, uint32_t                                                                         \
             : _PARAMETER_TYPE_UINT32, int32_t                                                                         \
             : _PARAMETER_TYPE_INT32, uint64_t                                                                         \
             : _PARAMETER_TYPE_UINT64, int64_t                                                                         \
             : _PARAMETER_TYPE_INT64, float                                                                            \
             : _PARAMETER_TYPE_FLOAT, double                                                                           \
             : _PARAMETER_TYPE_DOUBLE, default : 0)


#if GEL_PARAMETER_MAX_SIZE >= 2
#define _PARAMETER_FIELD_UINT16 u16
#define _PARAMETER_FIELD_INT16  i16
#define CAST16(x)               ((uint16_t)(x)&0xFFFF)
#else
#define _PARAMETER_FIELD_UINT16 u8
#define _PARAMETER_FIELD_INT16  u8
#define CAST16(x)               ((uint8_t)(x)&0xFF)
#endif

#if GEL_PARAMETER_MAX_SIZE >= 4
#define _PARAMETER_FIELD_UINT32 u32
#define _PARAMETER_FIELD_INT32  i32
#define _PARAMETER_FIELD_FLOAT  f
#define CAST32(x)               ((uint32_t)(x)&0xFFFFFFFF)
#define CASTFLOAT(x)            ((float)(x))
#else
#define _PARAMETER_FIELD_UINT32 u8
#define _PARAMETER_FIELD_INT32  i8
#define _PARAMETER_FIELD_FLOAT  i8
#define CAST32(x)               ((uint8_t)(x)&0xFF)
#define CASTFLOAT(x)            ((uint8_t)(x)&0xFF)
#endif

#if GEL_PARAMETER_MAX_SIZE >= 8
#define _PARAMETER_FIELD_UINT64 u64
#define _PARAMETER_FIELD_INT64  i64
#define _PARAMETER_FIELD_DOUBLE d
#define CAST64(x)               ((uint32_t)(x)&0xFFFFFFFFFFFFFFFF)
#define CASTDOUBLE(x)           ((double)(x))
#else
#define _PARAMETER_FIELD_UINT64 u8
#define _PARAMETER_FIELD_INT64  i8
#define _PARAMETER_FIELD_DOUBLE i8
#define CAST64(x)               ((uint8_t)(x)&0xFF)
#define CASTDOUBLE(x)           ((uint8_t)(x)&0xFF)
#endif

#define _PARAMETER_LIMIT_VALUE_OPTION(ref, x)                                                                          \
    _Generic((ref), uint8_t                                                                                            \
             : (parameter_type_union_t){.u8 = (uint8_t)((uint8_t)x & 0xFF)}, int8_t                                    \
             : (parameter_type_union_t){.i8 = (int8_t)((int8_t)x & 0xFF)}, uint16_t                                    \
             : (parameter_type_union_t){._PARAMETER_FIELD_UINT16 = (uint16_t)CAST16(x)}, int16_t                       \
             : (parameter_type_union_t){._PARAMETER_FIELD_INT16 = (int16_t)CAST16(x)}, uint32_t                        \
             : (parameter_type_union_t){._PARAMETER_FIELD_UINT32 = (uint32_t)CAST32(x)}, int32_t                       \
             : (parameter_type_union_t){._PARAMETER_FIELD_INT32 = (int32_t)CAST32(x)}, uint64_t                        \
             : (parameter_type_union_t){._PARAMETER_FIELD_UINT64 = (uint64_t)CAST64(x)}, int64_t                       \
             : (parameter_type_union_t){._PARAMETER_FIELD_INT64 = (int64_t)CAST64(x)}, float                           \
             : (parameter_type_union_t){._PARAMETER_FIELD_FLOAT = CASTFLOAT(x)}, double                                \
             : (parameter_type_union_t){._PARAMETER_FIELD_DOUBLE = CASTDOUBLE(x)})

#define PARAMETER_TYPE_UINT8_CAST(x)  ((parameter_type_union_t){.u8 = x})
#define PARAMETER_TYPE_INT8_CAST(x)   ((parameter_type_union_t){.i8 = x})
#define PARAMETER_TYPE_UINT16_CAST(x) ((parameter_type_union_t){._PARAMETER_FIELD_UINT16 = x})
#define PARAMETER_TYPE_INT16_CAST(x)  ((parameter_type_union_t){._PARAMETER_FIELD_INT16 = x})
#define PARAMETER_TYPE_UINT32_CAST(x) ((parameter_type_union_t){._PARAMETER_FIELD_UINT32 = x})
#define PARAMETER_TYPE_INT32_CAST(x)  ((parameter_type_union_t){._PARAMETER_FIELD_INT32 = x})
#define PARAMETER_TYPE_UINT64_CAST(x) ((parameter_type_union_t){._PARAMETER_FIELD_UINT64 = x})
#define PARAMETER_TYPE_INT64_CAST(x)  ((parameter_type_union_t){._PARAMETER_FIELD_INT64 = x})
#define PARAMETER_TYPE_FLOAT_CAST(x)  ((parameter_type_union_t){._PARAMETER_FIELD_FLOAT = x})
#define PARAMETER_TYPE_DOUBLE_CAST(x) ((parameter_type_union_t){._PARAMETER_FIELD_DOUBLE = x})

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


typedef struct GEL_PARAMETER_HANDLE_ATTRIBUTES _parameter_handle_t {
    parameter_type_t       type;
    void                  *pointer;
    void                  *pmin, *pmax;
    parameter_type_union_t min, max, defaultv, step;
    unsigned int           access_level;

    GEL_PARAMETER_USER_DATA data;

    void (*runtime)(struct _parameter_handle_t *, void *);
    void *arg;
} parameter_handle_t;

int                     parameter_operator(parameter_handle_t *handle, int mod);
size_t                  parameter_get_count(parameter_handle_t *ps, size_t length, unsigned int al);
parameter_handle_t     *parameter_get_handle(parameter_handle_t *ps, size_t length, size_t num, unsigned int al);
GEL_PARAMETER_USER_DATA parameter_get_user_data(parameter_handle_t *handle);
void                    parameter_reset_to_defaults(parameter_handle_t *ps, size_t length);
int                     parameter_check_ranges(parameter_handle_t *ps, size_t length);
void                    parameter_to_string_format(parameter_handle_t *handle, char *result, char *format);
size_t                  parameter_to_index(parameter_handle_t *handle);
size_t                  parameter_get_total_values(parameter_handle_t *handle);

#endif