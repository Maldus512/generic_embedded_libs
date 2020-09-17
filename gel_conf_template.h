#ifndef GEL_CONF_H_INCLUDED
#define DEL_CONF_H_INCLUDED

/*
 * Page manager
 */

#define PMAN_NAVIGATION_DEPTH 4

typedef void *pman_message_t;

typedef void *pman_event_t;

typedef void *pman_page_data_t;

typedef void *pman_model_t;

/*
 * PID
 */
typedef int pid_control_type_t;
typedef int pid_output_type_t;

#define PID_HISTORY_SIZE 1

/*
 *  Watcher
 */
#define GEL_MALLOC_AVAILABLE  0
#define GEL_STATIC_BLOCK_SIZE 16

#endif