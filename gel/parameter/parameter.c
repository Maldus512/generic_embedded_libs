#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "parameter.h"


#define SET_DEFAULT(p, TYPE, name)                                                                                     \
    {                                                                                                                  \
        TYPE *var = (TYPE *)p.pointer;                                                                                 \
        TYPE  def = p.defaultv.name;                                                                                   \
        *var      = def;                                                                                               \
    }


#define CHECK_RANGE_AGAINST(p, newv, TYPE, name)                                                                       \
    ({                                                                                                                 \
        TYPE min = p.pmin != NULL ? *((TYPE *)p.pmin) : p.min.name;                                                    \
        TYPE max = p.pmax != NULL ? *((TYPE *)p.pmax) : p.max.name;                                                    \
        int  res = 0;                                                                                                  \
        if (newv > max)                                                                                                \
            res = 1;                                                                                                   \
        else if (newv < min)                                                                                           \
            res = -1;                                                                                                  \
        res;                                                                                                           \
    })


#define CHECK_RANGE(p, TYPE, name)                                                                                     \
    ({                                                                                                                 \
        TYPE *var = (TYPE *)p.pointer;                                                                                 \
        TYPE  min = p.pmin != NULL ? *((TYPE *)p.pmin) : p.min.name;                                                   \
        TYPE  max = p.pmax != NULL ? *((TYPE *)p.pmax) : p.max.name;                                                   \
        int   res = 0;                                                                                                 \
        if (*var > max)                                                                                                \
            res = 1;                                                                                                   \
        else if (*var < min)                                                                                           \
            res = -1;                                                                                                  \
        res;                                                                                                           \
    })


#define APPLY_OPERATOR(p, TYPE, name, mod)                                                                             \
    {                                                                                                                  \
        TYPE *var  = (TYPE *)p.pointer;                                                                                \
        TYPE  min  = p.pmin != NULL ? *((TYPE *)p.pmin) : p.min.name;                                                  \
        TYPE  max  = p.pmax != NULL ? *((TYPE *)p.pmax) : p.max.name;                                                  \
        TYPE  newv = *var + p.step.name * (mod);                                                                       \
        int   res  = CHECK_RANGE_AGAINST(p, newv, TYPE, name);                                                         \
        if (res != 0 && mod > 0)                                                                                       \
            *var = min;                                                                                                \
        else if (res != 0 && mod < 0)                                                                                  \
            *var = max;                                                                                                \
        else                                                                                                           \
            *var = newv;                                                                                               \
    }


#define GET_RANGE(p, TYPE, name)                                                                                       \
    ({                                                                                                                 \
        TYPE min = p.pmin != NULL ? *((TYPE *)p.pmin) : p.min.name;                                                    \
        TYPE max = p.pmax != NULL ? *((TYPE *)p.pmax) : p.max.name;                                                    \
        max - min + 1;                                                                                                 \
    })




parameter_handle_t *parameter_get_handle(parameter_handle_t *ps, size_t length, size_t num, unsigned int al) {
    assert(num < length);

    size_t i = 0;
    for (i = 0; i < length; i++) {
        if (ps[i].runtime)
            ps[i].runtime(&ps[i], ps[i].arg);

        // A null access level means free access
        if (ps[i].access_level == 0 || ((ps[i].access_level & al) > 0)) {
            if (num == 0)
                return &ps[i];
            else
                num--;
        }
    }

    return NULL;
}


size_t parameter_to_index(parameter_handle_t *handle) {
    if (handle) {
        switch (handle->type) {
            case PARAMETER_TYPE_UINT8:
                return (size_t) * (uint8_t *)handle->pointer;
            case PARAMETER_TYPE_INT8:
                return (size_t) * (uint8_t *)handle->pointer;
#if GEL_PARAMETER_MAX_SIZE >= 2
            case PARAMETER_TYPE_UINT16:
                return (size_t) * (uint16_t *)handle->pointer;
            case PARAMETER_TYPE_INT16:
                return (size_t) * (int16_t *)handle->pointer;
#endif
#if GEL_PARAMETER_MAX_SIZE >= 4
            case PARAMETER_TYPE_UINT32:
                return (size_t) * (uint32_t *)handle->pointer;
            case PARAMETER_TYPE_INT32:
                return (size_t) * (int32_t *)handle->pointer;
#endif
#if GEL_PARAMETER_MAX_SIZE >= 8
            case PARAMETER_TYPE_UINT64:
                return (size_t) * (uint64_t *)handle->pointer;
            case PARAMETER_TYPE_INT64:
                return (size_t) * (int64_t *)handle->pointer;
#endif
#if GEL_PARAMETER_MAX_SIZE >= 4
            case PARAMETER_TYPE_FLOAT:
                return 0;
#endif
#if GEL_PARAMETER_MAX_SIZE >= 8
            case PARAMETER_TYPE_DOUBLE:
                return 0;
#endif
        }

        return 0;
    } else {
        return 0;
    }
}


void parameter_to_string_format(parameter_handle_t *handle, char *result, char *format) {
    if (handle) {
        switch (handle->type) {
            case PARAMETER_TYPE_UINT8:
                sprintf(result, format, *(uint8_t *)handle->pointer);
                break;
            case PARAMETER_TYPE_INT8:
                sprintf(result, format, *(int8_t *)handle->pointer);
                break;
#if GEL_PARAMETER_MAX_SIZE >= 2
            case PARAMETER_TYPE_UINT16:
                sprintf(result, format, *(uint16_t *)handle->pointer);
                break;
            case PARAMETER_TYPE_INT16:
                sprintf(result, format, *(int16_t *)handle->pointer);
                break;
#endif
#if GEL_PARAMETER_MAX_SIZE >= 4
            case PARAMETER_TYPE_UINT32:
                sprintf(result, format, *(uint32_t *)handle->pointer);
                break;
            case PARAMETER_TYPE_INT32:
                sprintf(result, format, *(int32_t *)handle->pointer);
                break;
#endif
#if GEL_PARAMETER_MAX_SIZE >= 8
            case PARAMETER_TYPE_UINT64:
                sprintf(result, format, *(uint64_t *)handle->pointer);
                break;
            case PARAMETER_TYPE_INT64:
                sprintf(result, format, *(int64_t *)handle->pointer);
                break;
#endif
#if GEL_PARAMETER_MAX_SIZE >= 4
            case PARAMETER_TYPE_FLOAT:
                sprintf(result, format, *(float *)handle->pointer);
                break;
#endif
#if GEL_PARAMETER_MAX_SIZE >= 8
            case PARAMETER_TYPE_DOUBLE:
                sprintf(result, format, *(double *)handle->pointer);
                break;
#endif
        }
    }
}


size_t parameter_get_count(parameter_handle_t *ps, size_t length, unsigned int al) {
    size_t count = 0;

    size_t i = 0;
    for (i = 0; i < length; i++) {
        if (ps[i].runtime)
            ps[i].runtime(&ps[i], ps[i].arg);

        if ((ps[i].access_level & al) > 0)
            count++;
    }

    return count;
}


int parameter_operator(parameter_handle_t *handle, int mod) {
    if (handle) {
        switch (handle->type) {
            case PARAMETER_TYPE_UINT8:
                APPLY_OPERATOR((*handle), uint8_t, u8, mod);
                break;
            case PARAMETER_TYPE_INT8:
                APPLY_OPERATOR((*handle), int8_t, i8, mod);
                break;
#if GEL_PARAMETER_MAX_SIZE >= 2
            case PARAMETER_TYPE_UINT16:
                APPLY_OPERATOR((*handle), uint16_t, u16, mod);
                break;
            case PARAMETER_TYPE_INT16:
                APPLY_OPERATOR((*handle), int16_t, i16, mod);
                break;
#endif
#if GEL_PARAMETER_MAX_SIZE >= 4
            case PARAMETER_TYPE_UINT32:
                APPLY_OPERATOR((*handle), uint32_t, u32, mod);
                break;
            case PARAMETER_TYPE_INT32:
                APPLY_OPERATOR((*handle), int32_t, i32, mod);
                break;
#endif
#if GEL_PARAMETER_MAX_SIZE >= 8
            case PARAMETER_TYPE_UINT64:
                APPLY_OPERATOR((*handle), uint64_t, u64, mod);
                break;
            case PARAMETER_TYPE_INT64:
                APPLY_OPERATOR((*handle), int64_t, i64, mod);
                break;
#endif
#if GEL_PARAMETER_MAX_SIZE >= 4
            case PARAMETER_TYPE_FLOAT:
                APPLY_OPERATOR((*handle), float, f, mod);
                break;
#endif
#if GEL_PARAMETER_MAX_SIZE >= 8
            case PARAMETER_TYPE_DOUBLE:
                APPLY_OPERATOR((*handle), double, d, mod);
                break;
#endif
        }

        return 0;
    } else {
        return -1;
    }
}


void parameter_reset_to_defaults(parameter_handle_t *ps, size_t length) {
    size_t i = 0;
    for (i = 0; i < length; i++) {
        parameter_handle_t *handle = parameter_get_handle(ps, length, i, (unsigned int)0xFFFFFFFF);

        if (handle) {
            switch (handle->type) {
                case PARAMETER_TYPE_UINT8:
                    SET_DEFAULT((*handle), uint8_t, u8);
                    break;
                case PARAMETER_TYPE_INT8:
                    SET_DEFAULT((*handle), int8_t, i8);
                    break;
#if GEL_PARAMETER_MAX_SIZE >= 2
                case PARAMETER_TYPE_UINT16:
                    SET_DEFAULT((*handle), uint16_t, u16);
                    break;
                case PARAMETER_TYPE_INT16:
                    SET_DEFAULT((*handle), int16_t, i16);
                    break;
#endif
#if GEL_PARAMETER_MAX_SIZE >= 4
                case PARAMETER_TYPE_UINT32:
                    SET_DEFAULT((*handle), uint32_t, u32);
                    break;
                case PARAMETER_TYPE_INT32:
                    SET_DEFAULT((*handle), int32_t, i32);
                    break;
#endif
#if GEL_PARAMETER_MAX_SIZE >= 8
                case PARAMETER_TYPE_UINT64:
                    SET_DEFAULT((*handle), uint64_t, u64);
                    break;
                case PARAMETER_TYPE_INT64:
                    SET_DEFAULT((*handle), int64_t, i64);
                    break;
#endif
#if GEL_PARAMETER_MAX_SIZE >= 4
                case PARAMETER_TYPE_FLOAT:
                    SET_DEFAULT((*handle), float, f);
                    break;
#endif
#if GEL_PARAMETER_MAX_SIZE >= 8
                case PARAMETER_TYPE_DOUBLE:
                    SET_DEFAULT((*handle), double, d);
                    break;
#endif
            }
        }
    }
}


int parameter_check_ranges(parameter_handle_t *ps, size_t length) {
    int counter = 0;

    size_t i = 0;
    for (i = 0; i < length; i++) {
        parameter_handle_t *handle = parameter_get_handle(ps, length, i, (unsigned int)0xFFFFFFFF);

        if (handle) {
            int outofrange = 0;

            switch (handle->type) {
                case PARAMETER_TYPE_UINT8:
                    if (CHECK_RANGE((*handle), uint8_t, u8)) {
                        SET_DEFAULT((*handle), uint8_t, u8);
                        outofrange = 1;
                    }
                    break;
                case PARAMETER_TYPE_INT8:
                    if (CHECK_RANGE((*handle), int8_t, i8)) {
                        SET_DEFAULT((*handle), int8_t, i8);
                        outofrange = 1;
                    }
                    break;
#if GEL_PARAMETER_MAX_SIZE >= 2
                case PARAMETER_TYPE_UINT16:
                    if (CHECK_RANGE((*handle), uint16_t, u16)) {
                        SET_DEFAULT((*handle), uint16_t, u16);
                        outofrange = 1;
                    }
                    break;
                case PARAMETER_TYPE_INT16:
                    if (CHECK_RANGE((*handle), int16_t, i16)) {
                        SET_DEFAULT((*handle), int16_t, i16);
                        outofrange = 1;
                    }
                    break;
#endif
#if GEL_PARAMETER_MAX_SIZE >= 4
                case PARAMETER_TYPE_UINT32:
                    if (CHECK_RANGE((*handle), uint32_t, u32)) {
                        SET_DEFAULT((*handle), uint32_t, u32);
                        outofrange = 1;
                    }
                    break;
                case PARAMETER_TYPE_INT32:
                    if (CHECK_RANGE((*handle), int32_t, i32)) {
                        SET_DEFAULT((*handle), int32_t, i32);
                        outofrange = 1;
                    }
                    break;
#endif
#if GEL_PARAMETER_MAX_SIZE >= 8
                case PARAMETER_TYPE_UINT64:
                    if (CHECK_RANGE((*handle), uint64_t, u64)) {
                        SET_DEFAULT((*handle), uint64_t, u64);
                        outofrange = 1;
                    }
                    break;
                case PARAMETER_TYPE_INT64:
                    if (CHECK_RANGE((*handle), int64_t, i64)) {
                        SET_DEFAULT((*handle), int64_t, i64);
                        outofrange = 1;
                    }
                    break;
#endif
#if GEL_PARAMETER_MAX_SIZE >= 4
                case PARAMETER_TYPE_FLOAT:
                    if (CHECK_RANGE((*handle), float, f)) {
                        SET_DEFAULT((*handle), float, f);
                        outofrange = 1;
                    }
                    break;
#endif
#if GEL_PARAMETER_MAX_SIZE >= 8
                case PARAMETER_TYPE_DOUBLE:
                    if (CHECK_RANGE((*handle), double, d)) {
                        SET_DEFAULT((*handle), double, d);
                        outofrange = 1;
                    }
                    break;
#endif
            }

            if (outofrange)
                counter++;
        }
    }

    return counter;
}


GEL_PARAMETER_USER_DATA parameter_get_user_data(parameter_handle_t *handle) {
    return handle->data;
}


size_t parameter_get_total_values(parameter_handle_t *handle) {
    size_t len = 0;

    if (handle != NULL) {
        switch (handle->type) {
            case PARAMETER_TYPE_UINT8:
                len = GET_RANGE((*handle), uint8_t, u8);
                break;
            case PARAMETER_TYPE_INT8:
                len = GET_RANGE((*handle), int8_t, i8);
                break;
#if GEL_PARAMETER_MAX_SIZE >= 2
            case PARAMETER_TYPE_UINT16:
                len = GET_RANGE((*handle), uint16_t, u16);
                break;
            case PARAMETER_TYPE_INT16:
                len = GET_RANGE((*handle), int16_t, i16);
                break;
#endif
#if GEL_PARAMETER_MAX_SIZE >= 4
            case PARAMETER_TYPE_UINT32:
                len = GET_RANGE((*handle), uint32_t, u32);
                break;
            case PARAMETER_TYPE_INT32:
                len = GET_RANGE((*handle), int32_t, i32);
                break;
#endif
#if GEL_PARAMETER_MAX_SIZE >= 8
            case PARAMETER_TYPE_UINT64:
                len = GET_RANGE((*handle), uint64_t, u64);
                break;
            case PARAMETER_TYPE_INT64:
                len = GET_RANGE((*handle), int64_t, i64);
                break;
#endif
#if GEL_PARAMETER_MAX_SIZE >= 4
            case PARAMETER_TYPE_FLOAT:
                len = GET_RANGE((*handle), float, f);
                break;
#endif
#if GEL_PARAMETER_MAX_SIZE >= 8
            case PARAMETER_TYPE_DOUBLE:
                len = GET_RANGE((*handle), double, d);
                break;
#endif
        }
    }

    return len;
}