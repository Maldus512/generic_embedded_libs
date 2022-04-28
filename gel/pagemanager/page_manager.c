#include <assert.h>
#include "page_manager.h"

#ifndef PMAN_VIEW_NULL
#define PMAN_VIEW_NULL
#endif

#ifndef PMAN_DATA_NULL
#define PMAN_DATA_NULL NULL
#endif

STACK_DEFINITION(navigation_stack, pman_page_t);

static void clear_page_stack(page_manager_t *pman) {
    pman_page_t page;

    while (navigation_stack_pop(&pman->page_stack, &page) == POP_RESULT_SUCCESS) {
        if (page.destroy) {
            page.destroy(page.data, page.extra);
        }
    }
}


void pman_init(page_manager_t *pman) {
    pman->initialized = 0;
    navigation_stack_init(&pman->page_stack);
}


pman_message_t pman_process_page_event(page_manager_t *pman, pman_model_t model, pman_event_t event) {
    assert(pman->initialized);
    return pman->current_page.process_event(model, pman->current_page.data, event);
}


pman_view_t pman_swap_page_extra(page_manager_t *pman, pman_model_t model, pman_page_t newpage, void *extra) {
    pman_page_t *current = &pman->current_page;

    if (current->close)
        current->close(current->data);
    if (current->destroy)
        current->destroy(current->data, current->extra);

    pman->current_page       = newpage;
    pman->current_page.extra = extra;
    // Create the newpage
    if (pman->current_page.create)
        pman->current_page.data = pman->current_page.create(model, pman->current_page.extra);
    else
        pman->current_page.data = PMAN_DATA_NULL;

    // Open the page
    if (pman->current_page.open)
        pman->current_page.open(model, pman->current_page.data);
    // Resume the page
    if (pman->current_page.resume)
        pman->current_page.resume(pman->current_page.data);
    // Update the page
    if (pman->current_page.update)
        return pman->current_page.update(model, pman->current_page.data);
    else
        return PMAN_VIEW_NULL;
}


pman_view_t pman_swap_page(page_manager_t *pman, pman_model_t model, pman_page_t newpage) {
    return pman_swap_page_extra(pman, model, newpage, NULL);
}


pman_view_t pman_reset_to_page(page_manager_t *pman, pman_model_t model, int id) {
    pman_page_t *current = &pman->current_page;

    if (current->close) {
        current->close(current->data);
    }
    if (current->destroy) {
        current->destroy(current->data, current->extra);
    }

    pman_page_t page;

    while (navigation_stack_pop(&pman->page_stack, &page) == POP_RESULT_SUCCESS) {
        if (page.id == id) {
            *current = page;
            if (current->open) {
                current->open(model, pman->current_page.data);
            }
            if (current->update) {
                return current->update(model, pman->current_page.data);
            } else {
                return PMAN_VIEW_NULL;
            }
        } else if (page.destroy) {
            page.destroy(page.data, page.extra);
        }
    }

    return PMAN_VIEW_NULL;
}


pman_view_t pman_rebase_page_extra(page_manager_t *pman, pman_model_t model, pman_page_t newpage, void *extra) {
    pman_page_t *current = &pman->current_page;

    if (current->close) {
        current->close(current->data);
    }
    if (current->destroy) {
        current->destroy(current->data, current->extra);
    }

    clear_page_stack(pman);

    pman->current_page       = newpage;
    pman->current_page.extra = extra;
    // Create the newpage
    if (pman->current_page.create)
        pman->current_page.data = pman->current_page.create(model, pman->current_page.extra);
    else
        pman->current_page.data = PMAN_DATA_NULL;

    // Open the page
    if (pman->current_page.open)
        pman->current_page.open(model, pman->current_page.data);
    // Resume the page
    if (pman->current_page.resume)
        pman->current_page.resume(pman->current_page.data);
    // Update the page
    if (pman->current_page.update)
        return pman->current_page.update(model, pman->current_page.data);
    else
        return PMAN_VIEW_NULL;
}


pman_view_t pman_rebase_page(page_manager_t *pman, pman_model_t model, pman_page_t newpage) {
    return pman_rebase_page_extra(pman, model, newpage, NULL);
}


pman_view_t pman_change_page_extra(page_manager_t *pman, pman_model_t model, pman_page_t newpage, void *extra) {
    // If it is the first page do not add it to the navigation stack
    if (pman->initialized) {
        pman_page_t *current;

        // Save the current page
        current = &pman->current_page;

        // Close the page
        if (current->close)
            current->close(current->data);

        if (navigation_stack_is_full(&pman->page_stack))
            navigation_stack_pop(&pman->page_stack, NULL);
        navigation_stack_push(&pman->page_stack, current);
    } else {
        pman->initialized = 1;
    }

    pman_page_t *dest;
    dest = &pman->current_page;

    *dest       = newpage;
    dest->extra = extra;

    // Create the newpage
    if (dest->create)
        dest->data = dest->create(model, extra);
    else
        dest->data = PMAN_DATA_NULL;

    // Open the page
    if (dest->open)
        dest->open(model, dest->data);
    // Update the page
    if (dest->update)
        return dest->update(model, dest->data);
    else
        return PMAN_VIEW_NULL;
}

pman_view_t pman_change_page(page_manager_t *pman, pman_model_t model, pman_page_t page) {
    return pman_change_page_extra(pman, model, page, NULL);
}

pman_view_t pman_back(page_manager_t *pman, pman_model_t model) {
    pman_page_t page;
    assert(pman->initialized);

    if (navigation_stack_pop(&pman->page_stack, &page) == POP_RESULT_SUCCESS) {
        pman_page_t *current = &pman->current_page;

        if (current->close)
            current->close(pman->current_page.data);
        if (current->destroy)
            current->destroy(current->data, current->extra);

        *current = page;
        if (current->open)
            current->open(model, pman->current_page.data);
        if (current->update)
            return current->update(model, pman->current_page.data);
    }

    return PMAN_VIEW_NULL;
}

pman_view_t pman_page_update(page_manager_t *pman, pman_model_t model) {
    pman_page_t *current;
    assert(pman->initialized);

    current = &pman->current_page;
    if (current && current->update)
        return current->update(model, current->data);
    else
        return PMAN_VIEW_NULL;
}