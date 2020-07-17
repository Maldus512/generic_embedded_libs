#ifndef PAGE_MANAGER_H_INCLUDED
#define PAGE_MANAGER_H_INCLUDED

#include "pman_conf.h"
#include "collections/stack.h"


typedef struct {
    int              id;
    pman_page_data_t data;
    void *           extra;
    int              popup;

    // Called when the page is first created; it initializes and returns the data structures used by the page
    pman_page_data_t (*create_page)(pman_model_t model, unsigned long timestamp, void *extra);
    // Called when the page definitively exits the scenes; should free all used memory
    void (*destroy_page)(pman_page_data_t data, void *extra);

    // Called when the page enters view; it should setup all the required widgets
    void (*open_page)(pman_model_t model, pman_page_data_t data, unsigned long timestamp);
    // Called when the page exits view
    void (*close_page)(pman_page_data_t data, unsigned long timestamp);

    // Called to update the page's content
    void (*update_page)(pman_model_t model, pman_page_data_t data);
    // Called to process an event
    pman_update_t (*process_event)(pman_model_t model, pman_page_data_t data, pman_event_t event,
                                   unsigned long timestamp);
} pman_page_t;

STACK_DECLARATION(navigation_stack, pman_page_t, PMAN_NAVIGATION_DEPTH);

typedef struct {
    int                     initialized;
    pman_page_t             current_page;
    struct navigation_stack navq;
} page_manager_t;

void init_page_manager(page_manager_t *pman);

void pman_back(page_manager_t *pman, pman_model_t model, unsigned long timestamp);
void pman_change_page(page_manager_t *pman, pman_model_t model, pman_page_t page, unsigned long timestamp);
void pman_change_page_extra(page_manager_t *pman, pman_model_t model, pman_page_t page, unsigned long timestamp,
                            void *extra);
void pman_rebase_page(page_manager_t *pman, pman_model_t model, pman_page_t newpage, unsigned long timestamp);
void page_update(page_manager_t *pman, pman_model_t model);

void null_update(pman_model_t model, pman_page_data_t data);
void null_destroy(pman_page_data_t data, void *extra);
void null_open(pman_model_t model, pman_page_data_t data, unsigned long timestamp);
void null_close(pman_page_data_t data, unsigned long timestamp);
#endif