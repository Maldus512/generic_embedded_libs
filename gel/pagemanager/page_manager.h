#ifndef PAGE_MANAGER_H_INCLUDED
#define PAGE_MANAGER_H_INCLUDED

#include "gel_conf.h"
#include "../collections/stack.h"


typedef struct {
    int              id;
    pman_page_data_t data;
    void *           extra;

    // Called when the page is first created; it initializes and returns the data structures used by the page
    pman_page_data_t (*create)(pman_model_t model, void *extra);
    // Called when the page definitively exits the scenes; should free all used memory
    void (*destroy)(pman_page_data_t data, void *extra);

    // Called when the page enters view
    void (*open)(pman_model_t model, pman_page_data_t data);
    // Called when the page exits view
    void (*close)(pman_page_data_t data);

    // Called when the page is back in focus
    void (*resume)(pman_page_data_t data);
    // Called when the page is still in view, but not the foremost element (e.g. a popup appeared)
    void (*pause)(pman_page_data_t data);

    // Called to update the page's content
    void (*update)(pman_model_t model, pman_page_data_t data);
    // Called to process an event
    pman_message_t (*process_event)(pman_model_t model, pman_page_data_t data, pman_event_t event);
} pman_page_t;

STACK_DECLARATION(navigation_stack, pman_page_t, PMAN_NAVIGATION_DEPTH);

typedef struct {
    int                     initialized;
    pman_page_t             current_page;
    struct navigation_stack page_stack;
} page_manager_t;

void init_page_manager(page_manager_t *pman);

void pman_back(page_manager_t *pman, pman_model_t model);
void pman_change_page(page_manager_t *pman, pman_model_t model, pman_page_t page);
void pman_change_page_extra(page_manager_t *pman, pman_model_t model, pman_page_t page, void *extra);
void pman_page_update(page_manager_t *pman, pman_model_t model);
void pman_rebase_page(page_manager_t *pman, pman_model_t model, pman_page_t newpage);
void pman_rebase_page_extra(page_manager_t *pman, pman_model_t model, pman_page_t newpage, void *extra);

void null_update(pman_model_t model, pman_page_data_t data);
void null_destroy(pman_page_data_t data, void *extra);
void null_open(pman_model_t model, pman_page_data_t data);
void null_close(pman_page_data_t data);
#endif