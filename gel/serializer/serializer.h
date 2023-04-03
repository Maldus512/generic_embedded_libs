/**
 * @file serializer.h
 * @author Maldus512
 * @brief Utilities to serialize various data types into bytes. Implemented as inline functions (or macros) for
 * efficiency
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef SERIALIZER_H_INCLUDED
#define SERIALIZER_H_INCLUDED

#include <stdint.h>
#include <stdlib.h>


#define UNPACK_UINT16_BE(var, buffer)                                                                                  \
    ({                                                                                                                 \
        uint16_t tmp;                                                                                                  \
        size_t   size = deserialize_uint16_be(&tmp, (buffer));                                                         \
        (var)         = tmp;                                                                                           \
        size;                                                                                                          \
    })

#define UNPACK_UINT32_BE(var, buffer)                                                                                  \
    ({                                                                                                                 \
        uint32_t tmp;                                                                                                  \
        size_t   size = deserialize_uint32_be(&tmp, (buffer));                                                         \
        (var)         = tmp;                                                                                           \
        size;                                                                                                          \
    })

#define UNPACK_UINT8(var, buffer)                                                                                      \
    ({                                                                                                                 \
        uint8_t tmp;                                                                                                   \
        size_t  size = deserialize_uint8(&tmp, (buffer));                                                              \
        (var)        = tmp;                                                                                            \
        size;                                                                                                          \
    })


#define PACK_UINT16_BE(buffer, var)                                                                                    \
    ({                                                                                                                 \
        uint16_t tmp  = (uint16_t)(var);                                                                               \
        size_t   size = serialize_uint16_be((buffer), tmp);                                                            \
        size;                                                                                                          \
    })

#define PACK_UINT8(buffer, var)                                                                                        \
    ({                                                                                                                 \
        uint8_t tmp  = (uint8_t)(var);                                                                                 \
        size_t  size = serialize_uint8((buffer), tmp);                                                                 \
        size;                                                                                                          \
    })


static inline __attribute__((always_inline)) size_t serialize_uint8(uint8_t *buf, uint8_t val) {
    buf[0] = val;
    return 1;
}


static inline __attribute__((always_inline)) size_t serialize_uint16_le(uint8_t *buf, uint16_t val) {
    buf[0] = val & 0xFF;
    buf[1] = (val >> 8) & 0xFF;
    return 2;
}


static inline __attribute__((always_inline)) size_t serialize_uint32_le(uint8_t *buf, uint32_t val) {
    buf[0] = val & 0xFF;
    buf[1] = (val >> 8) & 0xFF;
    buf[2] = (val >> 16) & 0xFF;
    buf[3] = (val >> 24) & 0xFF;
    return 4;
}


static inline __attribute__((always_inline)) size_t serialize_uint64_le(uint8_t *buf, uint64_t val) {
    buf[0] = val & 0xFF;
    buf[1] = (val >> 8) & 0xFF;
    buf[2] = (val >> 16) & 0xFF;
    buf[3] = (val >> 24) & 0xFF;
    buf[4] = (val >> 32) & 0xFF;
    buf[5] = (val >> 40) & 0xFF;
    buf[6] = (val >> 48) & 0xFF;
    buf[7] = (val >> 56) & 0xFF;
    return 8;
}


static inline __attribute__((always_inline)) size_t serialize_uint16_be(uint8_t *buf, uint16_t val) {
    buf[0] = (val >> 8) & 0xFF;
    buf[1] = val & 0xFF;
    return 2;
}


static inline __attribute__((always_inline)) size_t serialize_uint32_be(uint8_t *buf, uint32_t val) {
    buf[0] = (val >> 24) & 0xFF;
    buf[1] = (val >> 16) & 0xFF;
    buf[2] = (val >> 8) & 0xFF;
    buf[3] = val & 0xFF;
    return 4;
}


static inline __attribute__((always_inline)) size_t serialize_uint64_be(uint8_t *buf, uint64_t val) {
    buf[0] = (val >> 56) & 0xFF;
    buf[1] = (val >> 48) & 0xFF;
    buf[2] = (val >> 40) & 0xFF;
    buf[3] = (val >> 32) & 0xFF;
    buf[4] = (val >> 24) & 0xFF;
    buf[5] = (val >> 16) & 0xFF;
    buf[6] = (val >> 8) & 0xFF;
    buf[7] = val & 0xFF;
    return 8;
}


static inline __attribute__((always_inline)) size_t deserialize_uint8(uint8_t *val, uint8_t *buf) {
    *val = buf[0];
    return 1;
}


static inline __attribute__((always_inline)) size_t deserialize_int8(int8_t *val, uint8_t *buf) {
    *val = (int8_t)buf[0];
    return 1;
}


static inline __attribute__((always_inline)) size_t deserialize_uint16_le(uint16_t *val, uint8_t *buf) {
    *val = ((uint16_t)buf[0]) | ((uint16_t)buf[1] << 8);
    return 2;
}


static inline __attribute__((always_inline)) size_t deserialize_int16_le(int16_t *val, uint8_t *buf) {
    *val = ((int16_t)buf[0]) | ((int16_t)buf[1] << 8);
    return 2;
}


static inline __attribute__((always_inline)) size_t deserialize_uint32_le(uint32_t *val, uint8_t *buf) {
    *val = ((uint32_t)buf[0]) | ((uint32_t)buf[1] << 8) | ((uint32_t)buf[2] << 16) | ((uint32_t)buf[3] << 24);
    return 4;
}


static inline __attribute__((always_inline)) size_t deserialize_int32_le(int32_t *val, uint8_t *buf) {
    *val = ((int32_t)buf[0]) | ((int32_t)buf[1] << 8) | ((int32_t)buf[2] << 16) | ((int32_t)buf[3] << 24);
    return 4;
}


static inline __attribute__((always_inline)) size_t deserialize_uint64_le(uint64_t *val, uint8_t *buf) {
    *val = ((uint64_t)buf[0]) | ((uint64_t)buf[1] << 8) | ((uint64_t)buf[2] << 16) | ((uint64_t)buf[3] << 24) |
           ((uint64_t)buf[4] << 32) | ((uint64_t)buf[5] << 40) | ((uint64_t)buf[6] << 48) | ((uint64_t)buf[7] << 56);
    return 8;
}


static inline __attribute__((always_inline)) size_t deserialize_int64_le(int64_t *val, uint8_t *buf) {
    *val = ((int64_t)buf[0]) | ((int64_t)buf[1] << 8) | ((int64_t)buf[2] << 16) | ((int64_t)buf[3] << 24) |
           ((int64_t)buf[4] << 32) | ((int64_t)buf[5] << 40) | ((int64_t)buf[6] << 48) | ((int64_t)buf[7] << 56);
    return 8;
}


static inline __attribute__((always_inline)) size_t deserialize_uint16_be(uint16_t *val, uint8_t *buf) {
    *val = ((uint16_t)buf[1]) | ((uint16_t)buf[0] << 8);
    return 2;
}


static inline __attribute__((always_inline)) size_t deserialize_int16_be(int16_t *val, uint8_t *buf) {
    *val = ((int16_t)buf[1]) | ((int16_t)buf[0] << 8);
    return 2;
}


static inline __attribute__((always_inline)) size_t deserialize_uint32_be(uint32_t *val, uint8_t *buf) {
    *val = ((uint32_t)buf[3]) | ((uint32_t)buf[2] << 8) | ((uint32_t)buf[1] << 16) | ((uint32_t)buf[0] << 24);
    return 4;
}


static inline __attribute__((always_inline)) size_t deserialize_int32_be(int32_t *val, uint8_t *buf) {
    *val = ((int32_t)buf[3]) | ((int32_t)buf[2] << 8) | ((int32_t)buf[1] << 16) | ((int32_t)buf[0] << 24);
    return 4;
}


static inline __attribute__((always_inline)) size_t deserialize_uint64_be(uint64_t *val, uint8_t *buf) {
    *val = ((uint64_t)buf[7]) | ((uint64_t)buf[6] << 8) | ((uint64_t)buf[5] << 16) | ((uint64_t)buf[4] << 24) |
           ((uint64_t)buf[3] << 32) | ((uint64_t)buf[2] << 40) | ((uint64_t)buf[1] << 48) | ((uint64_t)buf[0] << 56);
    return 8;
}


static inline __attribute__((always_inline)) size_t deserialize_int64_be(int64_t *val, uint8_t *buf) {
    *val = ((int64_t)buf[7]) | ((int64_t)buf[6] << 8) | ((int64_t)buf[5] << 16) | ((int64_t)buf[4] << 24) |
           ((int64_t)buf[3] << 32) | ((int64_t)buf[2] << 40) | ((int64_t)buf[1] << 48) | ((int64_t)buf[0] << 56);
    return 8;
}

#endif