#include "page_manager.h"

STACK_DEFINITION(navigation_stack, pman_page_t);

void init_page_manager(page_manager_t *pman, pman_page_t page) {
    pman->current_page = page;
    navigation_stack_init(&pman->navq);
}

pman_update_t pman_process_page_event(page_manager_t *pman, pman_model_t model, pman_event_t event,
                                      unsigned long timestamp) {
    return pman->current_page.process_event(model, pman->current_page.data, event, timestamp);
}


void pman_change_page_extra(page_manager_t *pman, pman_model_t model, pman_page_t page, unsigned long timestamp,
                            void *extra) {
    pman_page_t  oldpage;
    pman_page_t *curpage, *oldp;

    curpage = &pman->current_page;
    oldp    = NULL;

    if (navigation_stack_peek(&pman->navq, &oldpage) == POP_RESULT_SUCCESS)
        oldp = &oldpage;

    if (page.type == PMAN_PAGE_TYPE_POPUP) {
        // Se sono su un popup e passo a un altro popup distruggo quello attuale
        if (curpage && curpage->type == PMAN_PAGE_TYPE_POPUP && curpage->destroy_page) {
            curpage->destroy_page(curpage->data);
        }
        // Se invece la pagina attuale e' normale la mantengo
        else {
            if (navigation_stack_is_full(&pman->navq))
                navigation_stack_dequeue(&pman->navq);
            navigation_stack_push(&pman->navq, curpage);
        }
    } else {
        // Se ero su un popup e passo a una nuova pagina devo distruggere anche quella precedente
        // e non salvarla come old_page
        if (curpage && curpage->type == PMAN_PAGE_TYPE_POPUP) {
            if (oldp && oldp->destroy_page)
                oldp->destroy_page(oldp->data);
        }
        // Se invece arrivo da una pagina normale la salvo
        else {
            if (navigation_stack_is_full(&pman->navq))
                navigation_stack_dequeue(&pman->navq);
            navigation_stack_push(&pman->navq, curpage);
        }

        // In ogni caso distruggo la pagina corrente
        if (curpage && curpage->destroy_page)
            curpage->destroy_page(curpage->data);
    }

    pman->current_page      = page;
    pman->current_page.data = pman->current_page.create_page(model, timestamp, extra);
    pman->current_page.update_page(model, pman->current_page.data);
}

void refresh_page(page_manager_t *pman, pman_model_t model, unsigned long timestamp) {
    pman_page_t *curpage = &pman->current_page;

    if (curpage) {
        if (curpage->destroy_page)
            curpage->destroy_page(curpage->data);

        if (curpage->create_page)
            curpage->data = curpage->create_page(model, timestamp, curpage->extra);
    }
}

void pman_change_page(page_manager_t *pman, pman_model_t model, pman_page_t page, unsigned long timestamp) {
    pman_change_page_extra(pman, model, page, timestamp, NULL);
}

void pman_back(page_manager_t *pman, pman_model_t model, unsigned long timestamp) {
    pman_page_t page;
    if (navigation_stack_pop(&pman->navq, &page) == POP_RESULT_SUCCESS) {
        if (pman->current_page.destroy_page)
            pman->current_page.destroy_page(pman->current_page.data);

        pman->current_page      = page;
        pman->current_page.data = pman->current_page.create_page(model, timestamp, NULL);
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
void null_destroy(pman_page_data_t data) {
    (void)data;
}