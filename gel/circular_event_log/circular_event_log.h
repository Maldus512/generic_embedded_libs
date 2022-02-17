#ifndef CIRCULAR_EVENT_LOG_H_INCLUDED
#define CIRCULAR_EVENT_LOG_H_INCLUDED

#include <stdint.h>
#include <stdlib.h>


typedef struct {
    size_t       next_position;
    const size_t max_events;
    const size_t event_size;
    int          full_circle;

    int (*save_new_event)(uint8_t *event, size_t position);
    int (*load_events)(uint8_t *events, size_t number, size_t start);
} circular_event_log_t;


int    circular_event_log_new_event(circular_event_log_t *logger, uint8_t *event);
size_t circular_event_log_total_events(circular_event_log_t logger);
int    circular_event_log_load_last_events(circular_event_log_t logger, uint8_t *events, size_t number, size_t jump);


#endif