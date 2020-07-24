#include <string.h>
#include <stdlib.h>
#include "gel/serializer/serializer.h"
#include "unity.h"

void setUp() {
    srand(0);
}

void tearDown() {}

void test_identity() {
    uint8_t value = rand() % 256;
    uint8_t buffer;

    TEST_ASSERT_EQUAL(1, serialize_uint8(&buffer, value));
    TEST_ASSERT_EQUAL(value, buffer);

    uint8_t res;
    TEST_ASSERT_EQUAL(1, deserialize_uint8(&res, &buffer));
    TEST_ASSERT_EQUAL(value, res);
}

void test_one_byte() {
    uint8_t buffer[1];
    int8_t  resi8 = 0, int8 = -(rand() % 0x100);

    TEST_ASSERT_EQUAL(1, serialize_uint8(buffer, int8));
    TEST_ASSERT_EQUAL(1, deserialize_int8(&resi8, buffer));
    TEST_ASSERT_EQUAL(int8, resi8);
}

void test_two_bytes() {
    uint8_t  buffer[2];
    uint16_t resu16 = 0, uint16 = rand() % 0x10000;
    int16_t  resi16 = 0, int16 = -(rand() % 0x10000);

    TEST_ASSERT_EQUAL(2, serialize_uint16_le(buffer, uint16));
    TEST_ASSERT_EQUAL(2, deserialize_uint16_le(&resu16, buffer));
    TEST_ASSERT_EQUAL(uint16, resu16);

    TEST_ASSERT_EQUAL(2, serialize_uint16_be(buffer, uint16));
    TEST_ASSERT_EQUAL(2, deserialize_uint16_be(&resu16, buffer));
    TEST_ASSERT_EQUAL(uint16, resu16);

    TEST_ASSERT_EQUAL(2, serialize_uint16_le(buffer, int16));
    TEST_ASSERT_EQUAL(2, deserialize_int16_le(&resi16, buffer));
    TEST_ASSERT_EQUAL(int16, resi16);

    TEST_ASSERT_EQUAL(2, serialize_uint16_be(buffer, int16));
    TEST_ASSERT_EQUAL(2, deserialize_int16_be(&resi16, buffer));
    TEST_ASSERT_EQUAL(int16, resi16);
}

void test_four_bytes() {
    uint8_t  buffer[4];
    uint32_t resu32 = 0, uint32 = rand() % 0x100000000;
    int32_t  resi32 = 0, int32 = -(rand() % 0x100000000);

    TEST_ASSERT_EQUAL(4, serialize_uint32_le(buffer, uint32));
    TEST_ASSERT_EQUAL(4, deserialize_uint32_le(&resu32, buffer));
    TEST_ASSERT_EQUAL(uint32, resu32);

    TEST_ASSERT_EQUAL(4, serialize_uint32_le(buffer, int32));
    TEST_ASSERT_EQUAL(4, deserialize_int32_le(&resi32, buffer));
    TEST_ASSERT_EQUAL(int32, resi32);

    TEST_ASSERT_EQUAL(4, serialize_uint32_be(buffer, uint32));
    TEST_ASSERT_EQUAL(4, deserialize_uint32_be(&resu32, buffer));
    TEST_ASSERT_EQUAL(uint32, resu32);

    TEST_ASSERT_EQUAL(4, serialize_uint32_be(buffer, int32));
    TEST_ASSERT_EQUAL(4, deserialize_int32_be(&resi32, buffer));
    TEST_ASSERT_EQUAL(int32, resi32);
}

void test_eight_bytes() {
    uint8_t  buffer[8];
    uint64_t resu64 = 0, uint64 = ((uint64_t)rand() * (uint64_t)rand()) % 0xFFFFFFFFFFFFFFFFUL;
    int64_t  resi64 = 0, int64 = -(((int64_t)rand() * (int64_t)rand()) % 0xFFFFFFFFFFFFFFFFUL);

    TEST_ASSERT_EQUAL(8, serialize_uint64_le(buffer, uint64));
    TEST_ASSERT_EQUAL(8, deserialize_uint64_le(&resu64, buffer));
    TEST_ASSERT_EQUAL(uint64, resu64);

    TEST_ASSERT_EQUAL(8, serialize_uint64_be(buffer, uint64));
    TEST_ASSERT_EQUAL(8, deserialize_uint64_be(&resu64, buffer));
    TEST_ASSERT_EQUAL(uint64, resu64);

    TEST_ASSERT_EQUAL(8, serialize_uint64_le(buffer, int64));
    TEST_ASSERT_EQUAL(8, deserialize_int64_le(&resi64, buffer));
    TEST_ASSERT_EQUAL(int64, resi64);

    TEST_ASSERT_EQUAL(8, serialize_uint64_be(buffer, int64));
    TEST_ASSERT_EQUAL(8, deserialize_int64_be(&resi64, buffer));
    TEST_ASSERT_EQUAL(int64, resi64);
}