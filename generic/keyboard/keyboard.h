#ifndef __GENERIC_KEYBOARD_H__
#define __GENERIC_KEYBOARD_H__

#include <stdint.h>

typedef enum {
    KEY_NOTHING,     // Nothing has happened
    KEY_PRESS,       // The button was pressed initially. Not repeated
    KEY_PRESSING,
    KEY_CLICK,         // The button was pressed for "click" time. Not repeated
    KEY_LONGCLICK,     // The button was pressed for "longclick" time. Not repeated
    KEY_LONGPRESS,     // The button was kept pressed after a longclick event
    KEY_RELEASE,       // The button was released after a click event (long or short)
} key_event_t;


// Struct containing information about the key
typedef struct {
    unsigned long bitvalue;     // Bit map of the key
    int           code;         // Key code

    // Internal state, not to be handled outside
    struct {
        unsigned long time;
        uint8_t       value;
        uint8_t       oldvalue;
        uint8_t       ignore;
        key_event_t   lastevent;
    } _state;
} raw_key_t;


typedef struct {
    int         code;
    key_event_t event;
} keycode_event_t;

keycode_event_t keyboard_routine(raw_key_t *keys, int num, unsigned long click, unsigned long longclick,
                                 unsigned long timestamp, unsigned long bitvalue);
unsigned char   get_key_state(raw_key_t *key);
void            reset_keys(raw_key_t *keys, int num);
unsigned long   get_click_time(raw_key_t *keys, int num, int code, unsigned long timestamp);

#endif
