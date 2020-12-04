#include "symtable_list.h"
#include "newscanner.h"

void sym_list_init(symtableList *list)
{
    *list = NULL;
}

symListItemPtr sym_list_add(symtableList *list, Symtable table)
{

    symListItemPtr new_item = malloc(sizeof(struct symListItem));

    if (new_item == NULL)
    {
        return NULL;
    }
    new_item->next = NULL;
    new_item->table = *table;

    symListItemPtr curr = *list;

    if (curr == NULL)
    {
        *list = new_item;
        return new_item;
    }

    while (curr->next != NULL)
    {
        curr = curr->next;
    }
    curr->next = new_item;
    return new_item;
}

Symtable_item *sym_list_add_to_last(symtableList symlist, char *id, TOKEN var)
{
    Symtable *table;
    symListItemPtr curr_item = symlist;
    bool error;

    while (curr_item->next != NULL)
    {
        curr_item = curr_item->next;
    }
    table = curr_item->table;

    switch (var.tokentype)
    {
    case TOKEN_TYPE_INTEGER:
        return symtable_add_int(table, id, var.integer, &error);
        break;

    case TOKEN_TYPE_FLOAT64:
        return symtable_add_int(table, id, var.floater, &error);
        break;

    case TOKEN_TYPE_STRING:
        return symtable_add_int(table, id, var.string->string, &error);
        break;

    default:
        return NULL;
        break;
    }

}

void sym_list_remove_last(symtableList *list)
{
    symListItemPtr curr = *list;

    if (curr == NULL)
    {
        return;
    }

    if (curr->next == NULL)
    {
        *list = NULL;
        symtable_free(curr->table);
        free(curr);
        return;
    }

    symListItemPtr prev = curr;
    curr = curr->next;

    while (curr->next != NULL)
    {
        prev = curr;
        curr = curr->next;
    }
    symtable_free(curr->table);
    free(curr);
    prev->next = NULL;
    return;
}

void sym_list_dispose(symtableList *list)
{
    while ((*list) != NULL)
    {
        sym_list_remove_last(list);
    }
}
