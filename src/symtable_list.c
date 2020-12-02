#include "symtable_list.h"

void sym_list_init(symtableList *list){
    *list = NULL;
}

symListItemPtr sym_list_add(symtableList *list, Symtable table){

    symListItemPtr new_item = malloc(sizeof(struct symListItem));

    if (new_item == NULL){
        return NULL;
    }
    new_item->next = NULL;
    new_item->table = *table;

    symListItemPtr curr = *list;

    if (curr == NULL){
        *list = new_item;
        return new_item;
    }

    while (curr->next != NULL){
        curr = curr->next;
    }
    curr->next = new_item;
    return new_item;
}

void sym_list_remove_last(symtableList *list){
    symListItemPtr curr = *list;

    if (curr == NULL)
    {
        return;
    }

    if (curr->next == NULL)
    {
        *list = NULL;
        free(curr);
        return;
    }

    symListItemPtr prev = curr;
    curr = curr->next;

    while (curr->next != NULL){
        prev = curr;
        curr = curr->next;
    }
    free(curr);
    prev->next = NULL;
    return;
}

void sym_list_dispose(symtableList *list){
    while ((*list) != NULL){
        sym_list_remove_last(list);
    }
}
