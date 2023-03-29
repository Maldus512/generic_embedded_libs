#include <assert.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "scheduler.h"
#include "gel/serializer/serializer.h"


#ifdef GEL_SCHEDULER_DYNAMIC_ENTRIES
#define NUM_ENTRIES(scheduler) (scheduler->num_entries)
#else
#define NUM_ENTRIES(_scheduler) GEL_SCHEDULER_MAX_ENTRIES
#endif


static unsigned long to_seconds(uint8_t hour, uint8_t minute, uint8_t seconds);
static uint8_t       is_day_included(uint8_t days, uint8_t today);
static unsigned long total_week_timespan(scheduler_entry_t *entry);


static const uint32_t seconds_in_day = 60UL * 60UL * 24UL;


void scheduler_init(scheduler_t *scheduler
#ifdef GEL_SCHEDULER_DYNAMIC_ENTRIES
                    ,
                    size_t entries
#endif
) {
#ifdef GEL_SCHEDULER_DYNAMIC_ENTRIES
    scheduler->entries = (scheduler_entry_t *)malloc(sizeof(scheduler_entry_t) * entries);
    assert(scheduler->entries != NULL) scheduler->num_entries = entries;
#endif

    for (size_t i = 0; i < NUM_ENTRIES(scheduler); i++) {
        memset(&scheduler->entries[i], 0, sizeof(scheduler_entry_t));
    }
}


const scheduler_entry_t *scheduler_get_entry(scheduler_t *scheduler, size_t entry_index) {
    if (entry_index >= NUM_ENTRIES(scheduler)) {
        return NULL;
    } else {
        return (const scheduler_entry_t *)&scheduler->entries[entry_index];
    }
}


scheduler_entry_t *scheduler_get_entry_mut(scheduler_t *scheduler, size_t entry_index) {
    if (entry_index >= NUM_ENTRIES(scheduler)) {
        return NULL;
    } else {
        return &scheduler->entries[entry_index];
    }
}


int scheduler_set_entry(scheduler_t *scheduler, size_t entry_index, scheduler_entry_t entry) {
    if (entry_index >= NUM_ENTRIES(scheduler)) {
        return -1;
    }
    scheduler->entries[entry_index] = entry;

    return 0;
}


uint8_t scheduler_is_schedule_entry_enabled(scheduler_t *scheduler, size_t entry_index) {
    if (entry_index >= NUM_ENTRIES(scheduler)) {
        return -1;
    }

    return scheduler->entries[entry_index].enabled;
}


int scheduler_disable_schedule_entry(scheduler_t *scheduler, size_t entry_index) {
    if (entry_index >= NUM_ENTRIES(scheduler)) {
        return -1;
    }

    scheduler->entries[entry_index].enabled = 0;
    return 0;
}


static uint8_t is_entry_active(const scheduler_entry_t *entry, struct tm *tm_info) {
    if (!is_day_included(entry->days, tm_info->tm_wday)) {
        return 0;
    }

    unsigned long current = to_seconds(tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
    unsigned long start   = entry->start_second;
    unsigned long end     = entry->end_second;

    // TODO: manage a scheduler entry that overflows in the next day
    if (end > seconds_in_day) {
        // Inverted schedule
        return (current < end - seconds_in_day) || (current >= start && current < seconds_in_day);
    } else {
        return current >= start && current < end;
    }
}


int scheduler_is_active(scheduler_t *scheduler, struct tm *tm_info) {
    return scheduler_get_active_entry_number(scheduler, tm_info) >= 0;
}


int scheduler_get_active_entry_number(scheduler_t *scheduler, struct tm *tm_info) {
    uint8_t       found      = 0;
    unsigned long foundtotal = 0;
    uint8_t       foundvalue = 0;
    int           foundid    = -1;

    for (size_t i = 0; i < NUM_ENTRIES(scheduler); i++) {
        scheduler_entry_t *entry = &scheduler->entries[i];

        if (!entry->enabled) {
            continue;
        }

        if (is_entry_active(entry, tm_info)) {
            // If multiple entries are active keep the one with the smallest timespan
            if (!found || total_week_timespan(entry) < foundtotal ||
                (total_week_timespan(entry) == foundtotal && entry->value > foundvalue)) {
                found      = 1;
                foundtotal = total_week_timespan(entry);
                foundvalue = entry->value;
                foundid    = i;
            }
        }
    }

    return foundid;
}


size_t scheduler_are_there_overlapping_entries(scheduler_t *scheduler, const scheduler_entry_t *entry) {
#define INCLUDED(instant, entry) (instant >= entry->start_second && instant <= entry->end_second)
    size_t found = 0;

    for (size_t i = 0; i < NUM_ENTRIES(scheduler); i++) {
        scheduler_entry_t *overlapping_entry = &scheduler->entries[i];

        if (!overlapping_entry->enabled) {
            continue;
        }

        if ((overlapping_entry->days & entry->days) > 0) {
            if (INCLUDED(entry->start_second, overlapping_entry) || INCLUDED(entry->end_second, overlapping_entry) ||
                INCLUDED(overlapping_entry->start_second, entry) || INCLUDED(overlapping_entry->end_second, entry)) {
                found++;
            }
        }
    }

    return found;
#undef INCLUDED
}


int scheduler_current_value(scheduler_t *scheduler, struct tm *tm_info, int *value) {
    int id;

    if ((id = scheduler_get_active_entry_number(scheduler, tm_info)) >= 0) {
        if (value != NULL) {
            *value = scheduler->entries[id].value;
        }
        return 1;
    }

    return 0;
}


int scheduler_get_next_enabled_entry(scheduler_t *scheduler, size_t start) {
    for (size_t i = start; i < NUM_ENTRIES(scheduler); i++) {
        if (scheduler_is_schedule_entry_enabled(scheduler, i)) {
            return i;
        }
    }

    return -1;
}


int scheduler_get_first_available_entry(scheduler_t *scheduler) {
    for (size_t i = 0; i < NUM_ENTRIES(scheduler); i++) {
        if (!scheduler_is_schedule_entry_enabled(scheduler, i)) {
            return i;
        }
    }

    return -1;
}


uint16_t scheduler_get_active_entries_map(scheduler_t *scheduler, struct tm *tm_info) {
    uint16_t result = 0;

    for (size_t i = 0; i < NUM_ENTRIES(scheduler); i++) {
        result |= (is_entry_active(&scheduler->entries[i], tm_info) ? 1 : 0) << i;
    }

    return result;
}


unsigned long scheduler_get_schedule_remaining_duration(scheduler_t *scheduler, size_t entry_index,
                                                        struct tm *tm_info) {
    const scheduler_entry_t *entry = scheduler_get_entry(scheduler, entry_index);
    if (entry == NULL) {
        return 0;
    }

    if (!is_entry_active(entry, tm_info)) {
        return 0;
    }

    unsigned long current = to_seconds(tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
    unsigned long end     = entry->end_second;

    return end - current;
}


unsigned long scheduler_get_current_remaining_duration(scheduler_t *scheduler, struct tm *tm_info) {
    int id;

    if ((id = scheduler_get_active_entry_number(scheduler, tm_info)) >= 0) {
        return scheduler_get_schedule_remaining_duration(scheduler, id, tm_info);
    } else {
        return 0;
    }
}


size_t scheduler_get_max_schedule_entries(scheduler_t *scheduler) {
    return NUM_ENTRIES(scheduler);
}


size_t scheduler_entry_serialize(uint8_t *buffer, const scheduler_entry_t *entry) {
    size_t i = 0;
    i += serialize_uint8(&buffer[i], entry->enabled);
    i += serialize_uint8(&buffer[i], entry->days);
    i += serialize_uint32_le(&buffer[i], entry->start_second);
    i += serialize_uint32_le(&buffer[i], entry->end_second);
    i += serialize_uint8(&buffer[i], (uint8_t)entry->value);

    assert(i == SCHEDULER_ENTRY_SERIALIZED_SIZE);
    return i;
}


size_t scheduler_entry_deserialize(scheduler_entry_t *entry, uint8_t *buffer) {
    size_t i = 0;
    i += deserialize_uint8(&entry->enabled, &buffer[i]);
    i += deserialize_uint8(&entry->days, &buffer[i]);
    i += deserialize_uint32_le(&entry->start_second, &buffer[i]);
    i += deserialize_uint32_le(&entry->end_second, &buffer[i]);
    uint8_t value;
    i += deserialize_uint8(&value, &buffer[i]);
    entry->value = value;

    assert(i == SCHEDULER_ENTRY_SERIALIZED_SIZE);
    return i;
}


unsigned long scheduler_get_entry_duration(const scheduler_entry_t *entry) {
    if (entry == NULL) {
        return 0;
    }

    if (entry->start_second < entry->end_second) {
        return entry->end_second - entry->start_second;
    } else {
        return entry->start_second - entry->end_second;
    }
}


static unsigned long to_seconds(uint8_t hour, uint8_t minute, uint8_t seconds) {
    return hour * 60UL * 60UL + minute * 60UL + seconds;
}


static uint8_t is_day_included(uint8_t days, uint8_t today) {
    return (days & (1 << today)) > 0;
}


static unsigned long total_week_timespan(scheduler_entry_t *entry) {
    size_t num_days = 0;
    for (size_t i = 0; i < 7; i++) {
        num_days += is_day_included(entry->days, (uint8_t)i);
    }

    return scheduler_get_entry_duration(entry) * num_days;
}
