#ifndef PAGE_MANAGER_H_INCLUDED
#define PAGE_MANAGER_H_INCLUDED

/*
 *  Module that manages a stack of pages; to be used in tandem with some kind of view or display module.
 *  It heavily relies on typedefs to know which types should be passed to the page callbacks
 */

#include "../collections/stack.h"
#include "../gel_internal_conf.h"

#ifdef GEL_SIMPLE_CONFIG_INCLUSION
#include "gel_conf.h"
#elif defined GEL_PAGEMANAGER_CONFIGURATION_HEADER
#include GEL_PAGEMANAGER_CONFIGURATION_HEADER
#else
#error "Configuration not defined"
#endif


typedef struct {
    int              id;
    pman_page_data_t data;
    void            *extra;

    // Called when the page is first created; it initializes and returns the data structures used by the page
    pman_page_data_t (*create)(pman_model_t model, void *extra);
    // Called when the page definitively exits the scenes; should free all used memory
    void (*destroy)(pman_page_data_t data, void *extra);

    // Called when the page enters view
    void (*open)(pman_model_t model, pman_page_data_t data);
    // Called when the page exits view
    void (*close)(pman_page_data_t data);

    // Called when the page is back in focus
    void (*resume)(pman_page_data_t data);     // currently unused
    // Called when the page is still in view, but not the foremost element (e.g. a popup appeared)
    void (*pause)(pman_page_data_t data);     // currently unused

    // Called to update the page's content
    pman_view_t (*update)(pman_model_t model, pman_page_data_t data);
    // Called to process an event
    pman_message_t (*process_event)(pman_model_t model, pman_page_data_t data, pman_event_t event);
} pman_page_t;

STACK_DECLARATION(navigation_stack, pman_page_t, PMAN_NAVIGATION_DEPTH);

typedef struct {
    int                     initialized;
    pman_page_t             current_page;
    struct navigation_stack page_stack;
} page_manager_t;


/*
 *  Initializes the page manager, mostly setting fields to 0
 *
 * pman: pointer to the page manager struct to initialize
 */
void pman_init(page_manager_t *pman);

/*
 *  Closes and destroys the top page on the stack (if any), going back to the previous one.
 *
 * pman: pointer to the page manager struct
 * model: external data type
 */
pman_view_t pman_back(page_manager_t *pman, pman_model_t model);

/*
 *  Pushes the specified page on the stack, closing the one previously on top.
 *
 * pman: pointer to the page manager struct
 * model: external data type
 * page: page to be pushed
 */
pman_view_t pman_change_page(page_manager_t *pman, pman_model_t model, pman_page_t page);

/*
 *  Pushes the specified page on the stack, closing the one previously on top.
 *
 * pman: pointer to the page manager struct
 * model: external data type
 * page: page to be pushed
 * extra: argument that will be passed to the create callback of the page
 */
pman_view_t pman_change_page_extra(page_manager_t *pman, pman_model_t model, pman_page_t page, void *extra);

/*
 *  Calls the update callback of the current page
 *
 * pman: pointer to the page manager struct
 * model: external data type
 */
pman_view_t pman_page_update(page_manager_t *pman, pman_model_t model);

/*
 *  Empty the stack and push the specified page as sole element.
 *
 * pman: pointer to the page manager struct
 * model: external data type
 * newpage: page to be pushed
 */
pman_view_t pman_rebase_page(page_manager_t *pman, pman_model_t model, pman_page_t newpage);

/*
 *  Empty the stack and push the specified page as sole element.
 *
 * pman: pointer to the page manager struct
 * model: external data type
 * newpage: page to be pushed
 * extra: argument that will be passed to the create callback of the page
 */
pman_view_t pman_rebase_page_extra(page_manager_t *pman, pman_model_t model, pman_page_t newpage, void *extra);


/**
 * @brief Change the current page, closing and destroying the one already present. Passes `extra` to the create
 * callback.
 *
 * @param pman
 * @param model
 * @param newpage
 * @param extra
 * @return pman_view_t
 */
pman_view_t pman_swap_page_extra(page_manager_t *pman, pman_model_t model, pman_page_t newpage, void *extra);


/**
 * @brief Change the current page, closing and destroying the one already present
 *
 * @param pman
 * @param model
 * @param newpage
 * @return pman_view_t
 */
pman_view_t pman_swap_page(page_manager_t *pman, pman_model_t model, pman_page_t newpage);


pman_view_t pman_reset_to_page(page_manager_t *pman, pman_model_t model, int id);

#endif