#include <string.h>

#include "wearleveling.h"


/*
  search for last used block.
  return code:
    negative: error
    positive (or 0): index of last
      i = wlm->blocks_in_page means some kind of error, but it should never happen

    every block can be described by (index, marker)
    every block (i1,m1) is followed by another block (i2,m2) such that:
    (i2 == MOD_NEXT(i1,blocks_in_page)) <==> (m2 == MOD_NEXT(m2,256)) XOR i2 is last used
    this means that the only block which is not followed by a block marked by its next in module is the last used.
    this stands true also for the last block of the list, hence the MOD_NEXT on the indexes (left side of the
  implication before)

    initialization:
      during the first round the property will be true only for the blocks already used, and will always fail on the
  first new block. since blocks are listed iteratively, they will be used in such order.

    At most 255 blocks can be marked (256-1)(need one to ensure the property)
    256 is hardcoded, it can be increased to 65536 using 2 bytes, but this is not considered a priority.

    read can detect if the block is valid by checking the prevoius one
*/
int search_last(wear_leveled_memory_t *wlm) {
    uint8_t tmp_buffer[WL_BLOCK_SIZE];
    int32_t err;
    size_t  last_idx;
    size_t  i;

    // start from 0, and move right
    if ((err = wlm->read_block(0, tmp_buffer))) {
        return err;
    }
    last_idx = tmp_buffer[0];

    for (i = 1; i < wlm->blocks_in_page; i++) {
        if ((err = wlm->read_block(i, tmp_buffer))) {
            return err;
        }

        if (tmp_buffer[0] != MOD_NEXT(last_idx, 256)) {
            last_idx = tmp_buffer[0];
            break;
        }
        last_idx = tmp_buffer[0];
    }
    return i - 1;
}


int wl_read(wear_leveled_memory_t *wlm, uint8_t *buffer) {
    uint8_t tmp_buffer[WL_BLOCK_SIZE];
    int32_t err;
    int     last_idx = search_last(wlm);
    if (last_idx < 0) {
        return last_idx;
    }
    if (last_idx == (int)wlm->blocks_in_page) {
        return -1;
        // what to do in a case that should never happen?
        // it may mean the memory got overwritten or corrupted
    }


    if ((err = wlm->read_block(last_idx, tmp_buffer))) {
        return err;
    }
    memcpy(buffer, tmp_buffer + 1, wlm->bytes_used_in_block);
    return 0;
}


int wl_write(wear_leveled_memory_t *wlm, uint8_t *buffer) {

    uint8_t tmp_buffer[WL_BLOCK_SIZE];
    int32_t err;
    int     last_idx = search_last(wlm);
    size_t  last_mark;
    if (last_idx < 0) {
        return last_idx;
    }
    if (last_idx == (int)wlm->blocks_in_page) {
        return -1;
        // what to do in a case that should never happen?
        // it may mean the memory got overwritten or corrupted
    }

    if ((err = wlm->read_block(last_idx, tmp_buffer))) {
        return err;
    }

    last_mark = tmp_buffer[0];
    // print("last mark found %d",last_mark);
    if (memcmp(tmp_buffer + 1, buffer, wlm->bytes_used_in_block)) {
        tmp_buffer[0] = 1;
        memcpy(tmp_buffer + 1, buffer, wlm->bytes_used_in_block);
        tmp_buffer[0] = MOD_NEXT(last_mark, 256);
        if ((err = wlm->write_block(MOD_NEXT(last_idx, wlm->blocks_in_page), tmp_buffer))) {
            return err;
        }
    }
    return 0;
}


void wl_init(wear_leveled_memory_t *wlm, memory_cb_t write_block, memory_cb_t read_block, size_t blocks_in_page,
             size_t bytes_used_in_block) {
    assert(bytes_used_in_block < WL_BLOCK_SIZE);
    assert(blocks_in_page < 256);
    wlm->write_block         = write_block;
    wlm->read_block          = read_block;
    wlm->blocks_in_page      = blocks_in_page;
    wlm->bytes_used_in_block = bytes_used_in_block;
}
