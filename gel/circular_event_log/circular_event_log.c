#include "circular_event_log.h"


static size_t nth_last_event(circular_event_log_t logger, size_t jump);


int circular_event_log_new_event(circular_event_log_t *logger, uint8_t *event) {
    int res = logger->save_new_event(event, logger->next_position);

    if (res == 0) {
        logger->next_position++;
        if (logger->next_position >= logger->max_events) {
            logger->next_position = 0;
            logger->full_circle   = 1;
        }
    }

    return res;
}


size_t circular_event_log_total_events(circular_event_log_t logger) {
    if (logger.full_circle) {
        return logger.max_events;
    } else {
        return logger.next_position;
    }
}


int circular_event_log_load_last_events(circular_event_log_t logger, uint8_t *events, size_t number, size_t jump) {
    size_t total_events = circular_event_log_total_events(logger);

    if (jump > total_events) {
        return 0;
    }
    size_t actual_number = number;
    if (jump + actual_number > total_events) {
        actual_number = total_events - jump;
    }

    size_t start = nth_last_event(logger, actual_number + jump);

    if (start + actual_number > logger.max_events) {
        size_t to_read = logger.max_events - start;
        int    res     = logger.load_events(&events[0], to_read, start);
        if (res <= 0) {
            return res;
        }

        size_t second_round = actual_number - to_read;
        return res + logger.load_events(&events[to_read * logger.event_size], second_round, 0);
    } else {
        return logger.load_events(&events[0], actual_number, start);
    }
}


static size_t nth_last_event(circular_event_log_t logger, size_t jump) {
    if (logger.next_position >= jump) {
        return logger.next_position - jump;
    } else if (logger.full_circle) {
        return logger.max_events - (jump - logger.next_position);
    } else {
        return 0;
    }
}
