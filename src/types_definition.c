// this file contains types definition, data structrs and funinctions needet to work with it

#include <stdlib.h>

#include "types_definition.h"




token_t *new_token(enum token_type_t type, int id) {
    token_t *tp = (token_t *) malloc(sizeof(token_t));
    if (tp == NULL)
        return NULL;

    tp->type = type;
    tp->id   = id;

    return tp;
}

void elem_free(element_t *ep) {
    if (ep->token_p != NULL)
        free(ep->token_p);

    free(ep);
}

void ll_init(list_t* lp) {
    lp->first      = NULL;
    lp->last       = NULL;
    lp->read_index = NULL;
}

// adds element to the end of list
// returns EXIT_SUCCESS on success and EXIT_FAIULURE otherways
int ll_add(list_t *lp, token_t *tp) {
    element_t *ep = (element_t *) malloc(sizeof(element_t));
    if (ep == NULL)
        return EXIT_FAILURE;

    ep->token_p = tp;
    ep->next    = NULL;

    // add first element
    if (lp->last == NULL) {
        lp->first      = ep;
        lp->last       = ep;
        lp->read_index = ep;
        return EXIT_SUCCESS;
    }

    // add non-first element
    lp->last->next = ep;
    lp->last       = lp->last->next;
    return EXIT_SUCCESS;
}

// reads token from the read_index and stores it to variable tp
// returns 1 afrer reading the last element 0 otherways
token_t *ll_read(list_t *lp, token_t *tp) {
    element_t *ep = lp->read_index;
    *tp = *(ep->token_p);

    lp->read_index = lp->read_index->next;

    if (lp->read_index == NULL) {
        lp->read_index = lp->first;
        return 1;
    }

    return 0;
}

void ll_free(list_t *lp) {
    element_t *curr = lp->first;
    element_t *next;

    while (curr != NULL) {
        next = curr->next;

        elem_free(curr);

        curr = next;
    }

    free(lp);
}
