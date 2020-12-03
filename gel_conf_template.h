#ifndef GEL_CONF_H_INCLUDED
#define GEL_CONF_H_INCLUDED

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

#define GEL_PID_INTEGRAL_HISTORY_SIZE 1
#define GEL_PID_DERIVATIVE_DELTA      1

/*
 *  Watcher
 */
#define GEL_MALLOC_AVAILABLE  0
#define GEL_STATIC_BLOCK_SIZE 16

/*
 * Wear leveling
 */

#define WL_BLOCK_SIZE 4

// funzione scrivi blocco
// 1 no blocco
// -1

/*
 * Parameter
 */

typedef int parameter_user_data_t;

#endif
