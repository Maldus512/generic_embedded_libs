#ifndef __GENERIC_KEYBOARD_H__
#define __GENERIC_KEYBOARD_H__

#include <stdint.h>

typedef enum
{
    KEY_NOTHING,
    KEY_CLICK,
    KEY_RELEASE,
    KEY_LONGCLICK,
    KEY_LONGPRESS,
} key_event_t;


typedef struct _raw_key
{
    uint32_t time;
    uint32_t  bitvalue;
    int code;
    uint8_t value;
    uint8_t oldvalue;
    char ignore;
    key_event_t   lastevent;
} raw_key_t;


typedef struct
{
    int code;
    key_event_t   event;
} keycode_event_t;

keycode_event_t keyboard_routine(raw_key_t *keys, int num, unsigned int debounce, unsigned long timestamp,
                                 unsigned long longpress, uint32_t bitvalue);
unsigned char   get_key_state(raw_key_t *key);
void            reset_keys(raw_key_t *keys, int num);

#endif
