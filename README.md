# Collection of Generic Libraries

This is a collection of mildly useful libraries meant for embedded environments. They have absolutely zero dependencies (not even dynamic memory). The APIs are documented in the header files.

- **Circular buffer**: a circular buffer queue to store bytes (`uint8_t`). It is thread safe in a consumer-producer scenario.
- **Debounce**: logical debouncing library for digital inputs and pulses. You should call the filter function on regular intervals and state how many subsequent call must have the same value for the internal level to change. The focus is on stability, not time, so it relies on number of calls rather then time elapsed.
- **Keypad**: dispatcher of typical keyboard events. The keys are defined outside and passed to the `keypad_routine` function. The various events are driven by time units.
- **Parameter**: library to handle generic parameters.
- **Queue**: macro-generated specific type queues.
- **Timer**: time helper functions. Includes `time_after` from the Linux kernel and a small management library for timers that can be paused and restarted. 

## On Data Hiding

The APIs have no data hiding, meaning the structures used to handle the data are open for the world to see. Data hiding would require dynamic memory allocation through `malloc` (or some variation), and I wanted the libraries to have 0 dependencies whatsoever.

## Tests

A suite of Unity-automated tests is included in the `test` folder. To check the tests complete run:

```
$ scons test
```

## TODO

- Add protocols (simple uart packets, hex packets)
- Add `page_pause` and `page_resume` methods to page manager (to preserve page state)
- Add a Kconfig for ESP-IDF components
- In the page manager, manage popups separately (add a `popup` field and make sure every page that goes on the stack has been paused and closed)
- if any callback is NULL it shouldn't be called (page manager)
- Add the PRESS event to the keyboard
- heavily revise parameter structure and naming convention
