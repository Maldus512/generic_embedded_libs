#include <string.h>

#include "wearleveling.h"

int wl_read(wear_leveled_memory_t* wlm, uint8_t* buffer){
  uint8_t tmp_buffer[WL_BLOCK_SIZE];
  int32_t err;

  for(size_t i=0; i < wlm->blocks_in_page; i++){
    if ((err = wlm->read_block(i,tmp_buffer))){
      return err;
    }
    if (tmp_buffer[0]==1){
        memcpy(buffer, tmp_buffer+1, wlm->bytes_used_in_block);
        return 0;
    }
  }
  return 1;
}

int wl_write(wear_leveled_memory_t* wlm, uint8_t* buffer){
  uint8_t tmp_buffer[WL_BLOCK_SIZE];
  int32_t err;
  size_t last_idx=0;
  for(size_t i=0; i < wlm->blocks_in_page; i++){
    if ((err = wlm->read_block(i,tmp_buffer))){
      return err;
    }
    if (tmp_buffer[0]==1){
        last_idx = i;
        break;
    }
  }
  //TODO check that value has changed
  err=memcmp(tmp_buffer+1, buffer, wlm->bytes_used_in_block);
  if(err){
    tmp_buffer[0] = 1;
    memcpy(tmp_buffer+1, buffer, wlm->bytes_used_in_block);

    if ((err = wlm->write_block((last_idx+1)%wlm->blocks_in_page,tmp_buffer))){
      return err;
    }

    tmp_buffer[0] = 0;
    if ((err = wlm->write_block(last_idx,tmp_buffer))){
      return err;
    }
  }
  return 0;
}


void wl_init(wear_leveled_memory_t* wlm,memory_cb_t write_block, memory_cb_t read_block, size_t blocks_in_page, size_t bytes_used_in_block){
  assert(bytes_used_in_block<WL_BLOCK_SIZE);
  wlm->write_block = write_block;
  wlm->read_block = read_block;
  wlm->blocks_in_page = blocks_in_page;
  wlm->bytes_used_in_block = bytes_used_in_block;

}
