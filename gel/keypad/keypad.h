#ifndef __GENERIC_KEYBOARD_H__
#define __GENERIC_KEYBOARD_H__

#include <stdint.h>

#define KEYPAD_KEY(b, c)                                                                                               \
    { .bitvalue = (b), .code = (c) }
#define KEYPAD_NULL_KEY                                                                                                \
    { 0 }

typedef enum {
    KEY_NOTHING,     // Nothing has happened
    KEY_PRESS,       // The button was pressed initially. Not repeated
    KEY_PRESSING,    // the button is still pressed, repeated. 
    KEY_CLICK,         // The button was pressed for "click" time. Not repeated
    KEY_LONGCLICK,     // The button was pressed for "longclick" time. Not repeated
    KEY_LONGPRESS,     // The button was kept pressed after a longclick event
    KEY_RELEASE,       // The button was released after a click event (long or short)
} keypad_event_t;


// Struct containing information about the key
typedef struct {
    unsigned long bitvalue;     // Bit map of the key
    int           code;         // Key code

    // Internal state, not to be handled outside
    struct {
        unsigned long  time_state;
        unsigned long  time_period;
        uint8_t        value;
        uint8_t        oldvalue;
        uint8_t        ignore;
        keypad_event_t lastevent;
    } _state;
    
} keypad_key_t;


typedef struct {
    int            code;
    keypad_event_t event;
} keypad_update_t;

keypad_update_t keypad_routine(keypad_key_t *keys, unsigned long click, unsigned long longclick,
                               unsigned long press_period, unsigned long timestamp, unsigned long bitvalue);

unsigned char   keypad_get_key_state(keypad_key_t *key);
void            keypad_reset_keys(keypad_key_t *keys);
unsigned long   keypad_get_click_time(keypad_key_t *keys, int code, unsigned long timestamp);

#endif
