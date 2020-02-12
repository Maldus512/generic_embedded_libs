#ifndef PAGE_MANAGER_H_INCLUDED
#define PAGE_MANAGER_H_INCLUDED

#include "pman_conf.h"
#include "collections/stack.h"


typedef enum {
    PMAN_PAGE_TYPE_NORMAL,
    PMAN_PAGE_TYPE_POPUP,
} pman_page_type_t;

typedef struct {
    int              id;
    pman_page_type_t type;
    pman_page_data_t data;
    void *           extra;

    pman_page_data_t (*create_page)(pman_model_t model, unsigned long timestamp, void *extra);
    void (*destroy_page)(pman_page_data_t data);
    void (*update_page)(pman_model_t model, pman_page_data_t data);
    pman_update_t (*process_event)(pman_model_t model, pman_page_data_t data, pman_event_t event,
                                        unsigned long timestamp);
} pman_page_t;

STACK_DECLARATION(navigation_stack, pman_page_t, PMAN_NAVIGATION_DEPTH);

typedef struct {
    pman_page_t             current_page;
    struct navigation_stack navq;
} page_manager_t;

void init_page_manager(page_manager_t *pman);

void pman_back(page_manager_t *pman, pman_model_t model, unsigned long timestamp);
void pman_change_page(page_manager_t *pman, pman_model_t model, pman_page_t page, unsigned long timestamp);
void pman_change_page_extra(page_manager_t *pman, pman_model_t model, pman_page_t page, unsigned long timestamp,
                            void *extra);
void refresh_page(page_manager_t *pman, pman_model_t model, unsigned long timestamp);
void page_update(page_manager_t *pman, pman_model_t model);

void null_update(pman_model_t model, pman_page_data_t data);
void null_destroy(pman_page_data_t data);
#endif