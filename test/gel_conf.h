#ifndef PMAN_CONF_H_INCLUDED
#define PMAN_CONF_H_INCLUDED

#define GEL_PARAMETER_MAX_SIZE 8

#define PMAN_NAVIGATION_DEPTH 5
#define PMAN_VIEW_NULL        0
#define PMAN_DATA_NULL        0

typedef int pman_message_t;

typedef int pman_event_t;

typedef int pman_page_data_t;

typedef int pman_model_t;

typedef int pman_view_t;

typedef int pid_control_type_t;
typedef int pid_output_type_t;

#define GEL_PID_INTEGRAL_HISTORY_SIZE 200
#define GEL_PID_DERIVATIVE_DELTA      5

#define GEL_MALLOC_AVAILABLE  0
#define GEL_STATIC_BLOCK_SIZE 80

#define WL_BLOCK_SIZE 4

typedef int parameter_user_data_t;

#endif
