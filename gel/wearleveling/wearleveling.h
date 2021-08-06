#ifndef WEARLEVELING_H_INCLUDED
#define WEARLEVELING_H_INCLUDED

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>


#define MAX_BLOCKS_IN_PAGE (0xFF + 1)


typedef int (*wl_read_block_t)(size_t block, uint8_t *buffer, size_t len);
typedef int (*wl_write_block_t)(size_t block, uint8_t marker, uint8_t *buffer, size_t len);
typedef int (*wl_read_marker_t)(size_t block, uint8_t *marker);


typedef struct {
    wl_read_block_t  read_block;
    wl_write_block_t write_block;
    wl_read_marker_t read_marker;

    size_t  blocks_in_page;
    size_t  last_active_block;
    uint8_t last_marker;
} wear_leveled_memory_t;


int wearleveling_read(wear_leveled_memory_t *wlm, uint8_t *buffer, size_t len);

int wearleveling_write(wear_leveled_memory_t *wlm, uint8_t *buffer, size_t len);

void wearleveling_init(wear_leveled_memory_t *wlm, wl_read_block_t read_block, wl_write_block_t write_block,
                       wl_read_marker_t read_marker, size_t blocks_in_page);


#endif
