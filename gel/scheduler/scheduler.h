#ifndef SCHEDULER_H_INCLUDED
#define SCHEDULER_H_INCLUDED

#include <stdint.h>
#include <sys/time.h>
#include <time.h>


#ifndef GEL_SCHEDULER_MAX_ENTRIES
#define GEL_SCHEDULER_MAX_ENTRIES 7
#endif


#define SCHEDULER_ENTRY_SERIALIZED_SIZE   11
#define SCHEDULER_ENTRY_DESERIALIZED_SIZE 11


typedef struct {
    uint8_t  enabled;
    uint8_t  days;
    uint32_t start_second;
    uint32_t duration;
    int      value;
} scheduler_entry_t;


typedef struct {
#ifdef GEL_SCHEDULER_DYNAMIC_ENTRIES
    size_t            num_entries;
    schedule_entry_t *entries;
#else
    scheduler_entry_t entries[GEL_SCHEDULER_MAX_ENTRIES];
#endif
} scheduler_t;


void scheduler_init(scheduler_t *scheduler
#ifdef GEL_SCHEDULER_DYNAMIC_ENTRIES
                    ,
                    size_t entries
#endif
);
const scheduler_entry_t *scheduler_get_entry(scheduler_t *scheduler, size_t entry_index);
scheduler_entry_t       *scheduler_get_entry_mut(scheduler_t *scheduler, size_t entry_index);
void                     scheduler_zero_entry(scheduler_t *scheduler, size_t entry_index);
int                      scheduler_set_entry(scheduler_t *scheduler, size_t entry_index, scheduler_entry_t entry);
int scheduler_enable_entry(scheduler_t *scheduler, size_t entry_index, uint8_t days, uint32_t start_second,
                           uint32_t duration, int value);
uint8_t scheduler_is_schedule_entry_enabled(scheduler_t *scheduler, size_t entry_index);
int scheduler_disable_schedule_entry(scheduler_t *scheduler, size_t entry_index);


#endif