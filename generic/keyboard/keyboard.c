#include "keyboard.h"

#define NTH(x, i) ((x >> i) & 0x1)

unsigned char get_key_state(raw_key_t *key)
{
    return key->value;
}

keycode_event_t keyboard_routine(raw_key_t *keys, int num, unsigned int debounce, unsigned long timestamp,
                                 unsigned long longpress, uint32_t bitvalue)
{
    int             i, found = -1;
    raw_key_t *     key;
    keycode_event_t event   = {0};
    key_event_t     current = 0;

    for (i = 0; i < num; i++)
    {
        if (keys[i].bitvalue == bitvalue)
        {
            keys[i].value = 1;
            found         = i;
            event.code    = keys[i].code;
            break;
        }
        else if (keys[i].oldvalue == 1 && (keys[i].bitvalue & bitvalue) == 0)
        {
            keys[i].value = 0;
            found         = i;
            event.code    = keys[i].code;
            break;
        }
    }

    if (found == -1)
        return event;

    key = &keys[found];

    if (key->value == key->oldvalue && !key->ignore)
    {
        if (timestamp > key->time + longpress)
        {
            if (key->lastevent == KEY_LONGCLICK || key->lastevent == KEY_LONGPRESS)
                event.event    = KEY_LONGPRESS;
            else
                event.event    = KEY_LONGCLICK;

            key->lastevent = event.event;
        }
        else if (timestamp > key->time + debounce)
        {
            current = key->value ? KEY_CLICK : KEY_RELEASE;
            if (current != key->lastevent)
            {
                event.event    = current;
                key->lastevent = event.event;
            }
        }
    }
    else if (key->value != key->oldvalue)
    {
        key->ignore    = 0;
        key->oldvalue  = key->value;
        key->time      = timestamp;
        event.event    = KEY_NOTHING;
        key->lastevent = event.event;
    }

    return event;
}

void reset_keys(raw_key_t *keys, int num)
{
    int i;
    for (i = 0; i < num; i++)
    {
        keys[i].time      = 0;
        keys[i].lastevent = KEY_NOTHING;
        keys[i].oldvalue  = keys[i].value;
        keys[i].ignore    = 1;
    }
}
