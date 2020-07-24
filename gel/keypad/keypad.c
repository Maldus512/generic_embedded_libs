#include "keypad.h"
#include "../timer/timecheck.h"

#define NTH(x, i) ((x >> i) & 0x1)

unsigned char keypad_get_key_state(keypad_key_t *key) {
    return key->_state.value;
}

keypad_update_t keypad_routine(keypad_key_t *keys, unsigned long click, unsigned long longclick,
                                 unsigned long timestamp, unsigned long bitvalue) {
    int             i = 0, found = -1;
    keypad_key_t *     key;
    keypad_update_t event   = {0};
    keypad_event_t     current = 0;

    while (keys[i].bitvalue) {
        // If a key is found
        if (keys[i].bitvalue == bitvalue) {
            keys[i]._state.value = 1;
            found                = i;
            event.code           = keys[i].code;
            break;
        }
        // If the key was pressed but it's not anymore
        else if (keys[i]._state.oldvalue && keys[i].bitvalue != bitvalue) {
            keys[i]._state.value = 0;
            found                = i;
            event.code           = keys[i].code;
            break;
        }
        // If the last event was a click and the key is not pressed anymore
        else if (keys[i]._state.lastevent != KEY_RELEASE && keys[i]._state.lastevent != KEY_NOTHING) {
            found      = i;
            event.code = keys[i].code;
            break;
        }

        i++;
    }

    if (found == -1)
        return event;

    key = &keys[found];

    if (key->_state.value == key->_state.oldvalue && !key->_state.ignore) {
        if (is_expired(key->_state.time, timestamp, longclick)) {
            if (key->_state.lastevent == KEY_LONGCLICK || key->_state.lastevent == KEY_LONGPRESS)
                event.event = KEY_LONGPRESS;
            else
                event.event = KEY_LONGCLICK;

            key->_state.lastevent = event.event;
        } else if (is_expired(key->_state.time, timestamp, click)) {
            current = key->_state.value ? KEY_CLICK : KEY_RELEASE;
            if (current != key->_state.lastevent) {
                event.event           = current;
                key->_state.lastevent = event.event;
            }
        }
    } else if (key->_state.value != key->_state.oldvalue) {
        key->_state.ignore   = 0;
        key->_state.oldvalue = key->_state.value;
        key->_state.time     = timestamp;

        if (key->_state.value) {
            event.event           = KEY_PRESS;
            key->_state.lastevent = event.event;
        }
    }

    return event;
}

void keypad_reset_keys(keypad_key_t *keys) {
    int i = 0;
    while(keys[i].bitvalue) {
        keys[i]._state.time      = 0;
        keys[i]._state.lastevent = KEY_NOTHING;
        keys[i]._state.oldvalue  = keys[i]._state.value;
        keys[i]._state.ignore    = 1;

        i++;
    }
}

unsigned long keypad_get_click_time(keypad_key_t *keys, int code, unsigned long timestamp) {
    int i = 0;
    while(keys[i].bitvalue) {
        if (keys[i].code == code &&
            (keys[i]._state.lastevent == KEY_CLICK || keys[i]._state.lastevent == KEY_LONGCLICK ||
             keys[i]._state.lastevent == KEY_LONGPRESS))
            return time_interval(keys[i]._state.time, timestamp);

        i++;
    }

    return 0;
}