#ifndef STATE_MACHINE_H_INCLUDED
#define STATE_MACHINE_H_INCLUDED


#define DECLARE_STATE_MACHINE(name, event_t, model_t)                                                                  \
    int name##_sm_send_event(name##_state_machine_t *sm, model_t *pmodel, event_t event);


#define DEFINE_STATE_MACHINE(name, event_t, model_t)                                                                   \
    typedef int (*name##_event_manager_t)(model_t *, event_t);                                                         \
    typedef struct {                                                                                                   \
        name##_event_manager_t *managers;                                                                              \
        unsigned int            state;                                                                                 \
    } name##_state_machine_t;                                                                                          \
                                                                                                                       \
    int name##_sm_send_event(name##_state_machine_t *sm, model_t *pmodel, event_t event) {                             \
        int res = sm->managers[sm->state](pmodel, event);                                                              \
                                                                                                                       \
        if (res >= 0) {                                                                                                \
            sm->state = res;                                                                                           \
            return 1;                                                                                                  \
        }                                                                                                              \
        return 0;                                                                                                      \
    }


#endif