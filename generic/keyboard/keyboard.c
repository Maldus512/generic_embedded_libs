#include "keyboard.h"

#define NTH(x, i) ((x >> i) & 0x1)

unsigned char get_key_state(raw_key_t *key) {
    return key->_state.value;
}

keycode_event_t keyboard_routine(raw_key_t *keys, int num, unsigned long click, unsigned long longclick,
                                 unsigned long timestamp, unsigned long bitvalue) {
    int             i, found = -1;
    raw_key_t *     key;
    keycode_event_t event   = {0};
    key_event_t     current = 0;

    for (i = 0; i < num; i++) {
        // If a key is found
        if (keys[i].bitvalue == bitvalue) {
            keys[i]._state.value = 1;
            found                = i;
            event.code           = keys[i].code;
            break;
        }
        // If the key was pressed but it's not anymore
        else if (keys[i]._state.oldvalue && (keys[i].bitvalue & bitvalue) == 0) {
            keys[i]._state.value = 0;
            found                = i;
            event.code           = keys[i].code;
            break;
        }
        // If the last event was a click and the key is not pressed anymore
        else if (keys[i]._state.lastevent != KEY_RELEASE && keys[i]._state.lastevent != KEY_NOTHING) {
            found      = i;
            event.code = keys[i].code;
            // Do not break; favor clicks over releases
        }
    }

    if (found == -1)
        return event;

    key = &keys[found];

    if (key->_state.value == key->_state.oldvalue && !key->_state.ignore) {
        if (timestamp > key->_state.time + longclick) {
            if (key->_state.lastevent == KEY_LONGCLICK || key->_state.lastevent == KEY_LONGPRESS)
                event.event = KEY_LONGPRESS;
            else
                event.event = KEY_LONGCLICK;

            key->_state.lastevent = event.event;
        } else if (timestamp > key->_state.time + click) {
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
    }

    return event;
}

void reset_keys(raw_key_t *keys, int num) {
    int i;
    for (i = 0; i < num; i++) {
        keys[i]._state.time      = 0;
        keys[i]._state.lastevent = KEY_NOTHING;
        keys[i]._state.oldvalue  = keys[i]._state.value;
        keys[i]._state.ignore    = 1;
    }
}
