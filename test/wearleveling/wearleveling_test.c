#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "wearleveling/wearleveling.h"
#include "unity.h"

#define MEMORY_SIZE 128
#define BLOCK_SIZE  12

wear_leveled_memory_t memory;

uint8_t eeprom[MEMORY_SIZE][BLOCK_SIZE];
uint8_t markers[MEMORY_SIZE];
size_t  write_count = 0;


void setUp() {
    srand(42);
}

void tearDown() {}

int read_marker(size_t block_num, uint8_t *marker) {
    if (block_num > MEMORY_SIZE) {
        return 1;
    }

    *marker = markers[block_num];
    return 0;
}

int read_block(size_t block_num, uint8_t *buffer, size_t len) {
    if (block_num > MEMORY_SIZE) {
        return 1;
    }

    memcpy(buffer, eeprom[block_num], len);
    return 0;
}

int write_block(size_t block_num, uint8_t marker, uint8_t *buffer, size_t len) {
    if (block_num > MEMORY_SIZE) {
        return 1;
    }

    markers[block_num] = marker;
    memcpy(eeprom[block_num], buffer, len);
    write_count++;
    return 0;
}

void test_init() {
    wearleveling_init(&memory, read_block, write_block, read_marker, MEMORY_SIZE);
    TEST_ASSERT_EQUAL(write_block, memory.write_block);
    TEST_ASSERT_EQUAL(read_block, memory.read_block);
    TEST_ASSERT_EQUAL(read_marker, memory.read_marker);
    TEST_ASSERT_EQUAL(MEMORY_SIZE, memory.blocks_in_page);
}


void naive_test(void) {
    write_count = 0;

    uint8_t buffer_in[3];
    uint8_t buffer_out[3];
    int     ret;

    buffer_in[0] = 42;
    buffer_in[1] = 43;
    buffer_in[2] = 44;

    ret = wearleveling_write(&memory, buffer_in, sizeof(buffer_in));
    TEST_ASSERT_EQUAL(1, write_count);
    TEST_ASSERT_EQUAL(0, ret);
    ret = wearleveling_read(&memory, buffer_out, sizeof(buffer_out));
    TEST_ASSERT_EQUAL(0, ret);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(buffer_in, buffer_out, sizeof(buffer_in));

    ret = wearleveling_write(&memory, buffer_in, sizeof(buffer_in));
    TEST_ASSERT_EQUAL(0, ret);
    TEST_ASSERT_EQUAL(1, write_count);

    buffer_in[2]++;
    ret = wearleveling_write(&memory, buffer_in, sizeof(buffer_in));
    TEST_ASSERT_EQUAL(0, ret);
    TEST_ASSERT_EQUAL(2, write_count);
    ret = wearleveling_read(&memory, buffer_out, sizeof(buffer_out));
    TEST_ASSERT_EQUAL(0, ret);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(buffer_in, buffer_out, sizeof(buffer_in));
}


void thorugh_test(void) {
    uint8_t buffer_in[BLOCK_SIZE];
    uint8_t buffer_out[BLOCK_SIZE];

    for (size_t i = 0; i < sizeof(buffer_in); i++) {
        buffer_in[i] = rand() % 256;
    }


    for (size_t i = 0; i < MEMORY_SIZE * 16; i++) {
        int ret = wearleveling_write(&memory, buffer_in, sizeof(buffer_in));
        TEST_ASSERT_EQUAL(0, ret);
        ret = wearleveling_read(&memory, buffer_out, sizeof(buffer_out));
        TEST_ASSERT_EQUAL(0, ret);
        TEST_ASSERT_EQUAL_UINT8_ARRAY(buffer_in, buffer_out, sizeof(buffer_in));
    }
}


void test_wear_leveling(void) {
    // Starting from 0
    memset(markers, 0, sizeof(markers));
    naive_test();
    thorugh_test();

    // Starting from 0xFF
    memset(markers, 0xFF, sizeof(markers));
    naive_test();
    thorugh_test();

    // Starting from random
    for (size_t i = 0; i < sizeof(markers); i++) {
        ((uint8_t *)markers)[i] = rand() % 256;
    }
    naive_test();
    thorugh_test();
}