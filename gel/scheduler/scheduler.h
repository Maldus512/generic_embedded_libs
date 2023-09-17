#ifndef SCHEDULER_H_INCLUDED
#define SCHEDULER_H_INCLUDED

#include <stdint.h>
#include <sys/time.h>
#include <time.h>


#ifndef GEL_SCHEDULER_MAX_ENTRIES
#define GEL_SCHEDULER_MAX_ENTRIES 7
#endif


#define SCHEDULER_ENTRY_SERIALIZED_SIZE 11


typedef enum {
    SCHEDULER_DOW_SUNDAY = 0,
    SCHEDULER_DOW_MONDAY,
    SCHEDULER_DOW_TUESDAY,
    SCHEDULER_DOW_WEDNESDAY,
    SCHEDULER_DOW_THURSDAY,
    SCHEDULER_DOW_FRIDAY,
    SCHEDULER_DOW_SATURDAY,
} scheduler_dow_t;


typedef struct {
    uint8_t  enabled;
    uint8_t  days;
    uint32_t start_second;
    uint32_t end_second;
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
uint8_t                  scheduler_is_schedule_entry_enabled(scheduler_t *scheduler, size_t entry_index);
int                      scheduler_disable_schedule_entry(scheduler_t *scheduler, size_t entry_index);
int                      scheduler_get_active_entry_number(scheduler_t *scheduler, struct tm *tm_info);
unsigned long            scheduler_get_entry_duration(const scheduler_entry_t *entry);
size_t                   scheduler_entry_serialize(uint8_t *buffer, const scheduler_entry_t *entry);
size_t                   scheduler_entry_deserialize(scheduler_entry_t *entry, uint8_t *buffer);
size_t scheduler_are_there_overlapping_entries(scheduler_t *scheduler, const scheduler_entry_t *entry);


#endif