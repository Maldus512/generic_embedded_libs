#ifndef CRC32_H_INCLUDED
#define CRC32_H_INCLUDED

#include <stdlib.h>
#include <stdint.h>

uint32_t crc32(const uint8_t *buf, size_t len, uint32_t init);

#endif