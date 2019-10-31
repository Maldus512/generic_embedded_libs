#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

typedef enum
{
    KEY_NOTHING,
    KEY_PRESS,
    KEY_RELEASE,
    KEY_LONGPRESS
} key_event_t;


typedef struct _raw_key
{
    unsigned long time;
    unsigned int  bitvalue;
    unsigned char code;
    unsigned char value;
    unsigned char oldvalue;
    unsigned char ignore;
    key_event_t   lastevent;
} raw_key_t;


typedef struct
{
    unsigned char code;
    key_event_t   event;
} keycode_event_t;

keycode_event_t keyboard_routine(raw_key_t *keys, int num, unsigned int debounce, unsigned long timestamp,
                                 unsigned long longpress, unsigned int bitvalue);
unsigned char   get_key_state(raw_key_t *key);
void            reset_keys(raw_key_t *keys, int num);

#endif
