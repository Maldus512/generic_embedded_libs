#include "wearleveling/wearleveling.h"
#include "unity.h"

#define MEMORY_SIZE 256
#define LEN 12
wear_leveled_memory_t memory;

uint8_t eeprom [MEMORY_SIZE][WL_BLOCK_SIZE];



void setUp() {}

void tearDown() {}

int read_block(size_t block_num, uint8_t * buffer){
  if ( block_num > MEMORY_SIZE){
    return 1;
  }
  for (size_t i =0; i<WL_BLOCK_SIZE; i++){
    buffer[i]=eeprom[block_num][i];
  }
  return 0;
}

int write_block(size_t block_num, uint8_t * buffer){
  if ( block_num> MEMORY_SIZE){
    return 1;
  }
  for (size_t i =0; i<WL_BLOCK_SIZE; i++){
    eeprom[block_num][i]=buffer[i];
  }
  return 0;
}

void test_init() {
  for (int i = 0; i< MEMORY_SIZE; i++){
    for (int j = 0; j< WL_BLOCK_SIZE; j++){
        eeprom[i][j]=0;
    }
  }

  wl_init(&memory, write_block, read_block, LEN, 3);
  TEST_ASSERT_EQUAL(write_block, memory.write_block);
  TEST_ASSERT_EQUAL(read_block, memory.read_block);
  TEST_ASSERT_EQUAL(LEN, memory.blocks_in_page);
  TEST_ASSERT_EQUAL(3, memory.bytes_used_in_block);
}


void test_naive(){

    uint8_t buffer_in[3];
    uint8_t buffer_out[3];
    int ret;

    //ret= wl_read(&memory,buffer_out);
    //TEST_ASSERT_EQUAL(1, ret);

    buffer_in[0]=42;
    buffer_in[1]=43;
    buffer_in[2]=44;

    ret= wl_write(&memory,buffer_in);
    TEST_ASSERT_EQUAL(0, ret);
    ret= wl_read(&memory,buffer_out);
    TEST_ASSERT_EQUAL(0, ret);
    TEST_ASSERT_EQUAL(buffer_in[0], buffer_out[0]);
    TEST_ASSERT_EQUAL(buffer_in[1], buffer_out[1]);
    TEST_ASSERT_EQUAL(buffer_in[2], buffer_out[2]);
}


void test_multiple_write(){
  uint8_t buffer_in[3];
  int ret;
  int pos;

  buffer_in[0]=42;
  buffer_in[1]=43;
  buffer_in[2]=44;

  ret= wl_write(&memory,buffer_in);
  TEST_ASSERT_EQUAL(0, ret);
  pos=search_last(&memory);

  buffer_in[0]=45;
  buffer_in[1]=46;
  buffer_in[2]=47;
  ret= wl_write(&memory,buffer_in);
  TEST_ASSERT_EQUAL(0, ret);
  TEST_ASSERT_EQUAL(pos+1, search_last(&memory));

}

void test_no_rewrite(){

  uint8_t buffer_in[3];
  int ret;
  int pos;
  buffer_in[0]=50;
  buffer_in[1]=51;
  buffer_in[2]=52;


  ret= wl_write(&memory,buffer_in);
  TEST_ASSERT_EQUAL(0, ret);
  pos=search_last(&memory);
  ret= wl_write(&memory,buffer_in);
  TEST_ASSERT_EQUAL(0, ret);
  ret= wl_write(&memory,buffer_in);
  TEST_ASSERT_EQUAL(0, ret);
  TEST_ASSERT_EQUAL(pos, search_last(&memory));


}

void test_rebegin(){
  uint8_t buffer_in[3];
  int ret;
  int pos;
  buffer_in[0]=50;
  buffer_in[1]=51;
  buffer_in[2]=52;


  ret= wl_write(&memory,buffer_in);
  TEST_ASSERT_EQUAL(0, ret);
  pos=search_last(&memory);

  for (int i =0; i< LEN-1; i++){
    buffer_in[0]=i;


    ret= wl_write(&memory,buffer_in);
    TEST_ASSERT_EQUAL(0, ret);

  }
  TEST_ASSERT_EQUAL(pos-1, search_last(&memory));

}
