#include "page_manager.h"

STACK_DEFINITION(navigation_stack, pman_page_t);

void init_page_manager(page_manager_t *pman) {
    pman->initialized = 0;
    navigation_stack_init(&pman->navq);
}

pman_update_t pman_process_page_event(page_manager_t *pman, pman_model_t model, pman_event_t event,
                                      unsigned long timestamp) {
    return pman->current_page.process_event(model, pman->current_page.data, event, timestamp);
}


void pman_change_page_extra(page_manager_t *pman, pman_model_t model, pman_page_t newpage, unsigned long timestamp,
                            void *extra) {
    // If it is the first page or a popup do not add it to the navigation stack
    if (pman->initialized) {
        pman_page_t *curpage = &pman->current_page;

        // If I was on a popup destroy it
        if (curpage->popup) {
            if (curpage->close_page)
                curpage->close_page(curpage->data, timestamp);
            if (curpage->destroy_page)
                curpage->destroy_page(curpage->data, curpage->extra);
        }
        // Close the previous (non-popup) page, but not if the next page is a popup
        else if (!newpage.popup && curpage->close_page)
            curpage->close_page(curpage->data, timestamp);

        // If it's not a popup save the current page
        if (!curpage->popup) {
            if (navigation_stack_is_full(&pman->navq))
                navigation_stack_pop(&pman->navq, NULL);
            navigation_stack_push(&pman->navq, curpage);
        }
    } else {
        pman->initialized = 1;
    }

    pman->current_page       = newpage;
    pman->current_page.extra = extra;
    // Create the newpage
    pman->current_page.data = pman->current_page.create_page(model, timestamp, extra);
    // Open the page
    if (pman->current_page.open_page)
        pman->current_page.open_page(model, pman->current_page.data, timestamp);
    // Update the page
    pman->current_page.update_page(model, pman->current_page.data);
}

void pman_change_page(page_manager_t *pman, pman_model_t model, pman_page_t page, unsigned long timestamp) {
    pman_change_page_extra(pman, model, page, timestamp, NULL);
}

void pman_back(page_manager_t *pman, pman_model_t model, unsigned long timestamp) {
    pman_page_t page;
    int         popup;
    if (navigation_stack_pop(&pman->navq, &page) == POP_RESULT_SUCCESS) {
        popup = pman->current_page.popup;
        if (pman->current_page.close_page)
            pman->current_page.close_page(pman->current_page.data, timestamp);
        if (pman->current_page.destroy_page)
            pman->current_page.destroy_page(pman->current_page.data, pman->current_page.extra);

        pman->current_page = page;
        // If the current page was a popup we did not close, so there is no need to open again
        if (!popup && pman->current_page.open_page) //TODO: What if we move to page -> popup -> page -> back?
                                                    // The first page needs to be closed when we move to the second....
            pman->current_page.open_page(model, pman->current_page.data, timestamp);
        pman->current_page.update_page(model, pman->current_page.data);
    }
}

void page_update(page_manager_t *pman, pman_model_t model) {
    pman_page_t *curpage;

    curpage = &pman->current_page;
    if (curpage && curpage->update_page)
        curpage->update_page(model, curpage->data);
}

void null_update(pman_model_t model, pman_page_data_t data) {
    (void)model;
    (void)data;
}

void null_destroy(pman_page_data_t data, void *extra) {
    (void)data;
    (void)extra;
}

void null_open(pman_model_t model, pman_page_data_t data, unsigned long timestamp) {
    (void)model;
    (void)data;
    (void)timestamp;
}

void null_close(pman_page_data_t data, unsigned long timestamp) {
    (void)data;
    (void)timestamp;
}