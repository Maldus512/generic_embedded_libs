/**
 * @file wearleveling.c
 * @author Maldus512
 * @brief Wear leveling abstraction module. It requires access to an external storage device organized in blocks, each
 * indexed in order and with an associated byte marker (how the marker is associate is not a concern of this library).
 * The access is gained through three function pointers:
 *  - read_block: reads some bytes from a specified block
 *  - read_marker: read just the marker from a specified block
 *  - write_block: writes data (and the marker) to a specified block
 *
 * The idea is to abstract a storage section as big as a block that can be read and written normally but in reality is
 * moved to a different block when changed. The marker for each block is incremented every time, so the last block that
 * is not followed by a consequent marker is the last one that was active.
 *
 */

#include <string.h>
#include "wearleveling.h"


#define NEXT_BLOCK(x, mod) ((x + 1) % mod)


static int search_last(wear_leveled_memory_t *wlm, uint8_t *last_marker);


/**
 * @brief Read data from the wear leveled memory
 *
 * @param wlm Pointer to the wear leveled memory struct
 * @param buffer Buffer where the data will be stored
 * @param len Number of bytes to read
 * @return int 0 if everything was ok, something else otherwise
 */
int wearleveling_read(wear_leveled_memory_t *wlm, uint8_t *buffer, size_t len) {
    return wlm->read_block(wlm->last_active_block, buffer, len);
}


/**
 * @brief Write data to the wear leveled memory
 *
 * @param wlm Pointer to the wear leveled memory struct
 * @param buffer Buffer containing the data to be written
 * @param len Number of bytes to write
 * @return int 0 if everything was ok, something else otherwise
 */
int wearleveling_write(wear_leveled_memory_t *wlm, uint8_t *buffer, size_t len) {
    uint8_t intermediate_buffer[len];
    int     res;

    if ((res = wlm->read_block(wlm->last_active_block, intermediate_buffer, len))) {
        return res;
    }

    if (memcmp(intermediate_buffer, buffer, len)) {
        wlm->last_marker       = NEXT_BLOCK(wlm->last_marker, MAX_BLOCKS_IN_PAGE);
        wlm->last_active_block = NEXT_BLOCK(wlm->last_active_block, wlm->blocks_in_page);

        return wlm->write_block(wlm->last_active_block, wlm->last_marker, buffer, len);
    }
    return 0;
}


/**
 * @brief Initialize the wear leveled memory data structure
 *
 * @param wlm Pointer to the structure to initialize
 * @param read_block Function to read from a specific block of memory
 * @param write_block Function to write a specific block of memory (and marker)
 * @param read_marker Function to read the marker of a block
 * @param blocks_in_page Number of blocks in page
 */
void wearleveling_init(wear_leveled_memory_t *wlm, wl_read_block_t read_block, wl_write_block_t write_block,
                       wl_read_marker_t read_marker, size_t blocks_in_page) {
    assert(blocks_in_page < MAX_BLOCKS_IN_PAGE);
    wlm->read_block     = read_block;
    wlm->write_block    = write_block;
    wlm->read_marker    = read_marker;
    wlm->blocks_in_page = blocks_in_page;

    wlm->last_active_block = search_last(wlm, &wlm->last_marker);
}


/**
 * @brief Search for the last used block
 *
 * @param wlm Wear leveled memory data structure
 * @return int 0 if everything was ok, something else otherwise
 */
static int search_last(wear_leveled_memory_t *wlm, uint8_t *last_marker) {
    uint8_t marker;
    int     err;

    // start from 0 and move on
    if ((err = wlm->read_marker(0, last_marker))) {
        return err;
    }

    size_t i;
    for (i = 1; i < wlm->blocks_in_page; i++) {
        // Read the next marker
        if ((err = wlm->read_marker(i, &marker))) {
            return err;
        }

        // If the current marker is not the one that should follow from the previous one (i.e. last_marker + 1 with mod
        // limit) then we found the last active one
        if (marker != NEXT_BLOCK(*last_marker, MAX_BLOCKS_IN_PAGE)) {
            break;
        } else {
            // Otherwise keep looking
            *last_marker = marker;
        }
    }

    if (i >= wlm->blocks_in_page) {
        // Something very wrong happened, reset to 0
        return 0;
    }

    // Return the index of the last active marker
    return i - 1;
}