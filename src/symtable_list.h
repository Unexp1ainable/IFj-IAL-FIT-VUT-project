/**
 * @file symtable_list.h
 * @author Samuel Repka
 * @brief 
 * @version 0.1
 * @date 2020-12-02
 * 
 */
#ifndef SYMTABLE_LIST_H
#define SYMTABLE_LIST_H

#include "symtable.h"



/**
 * @brief Item of linked list of symtables
 * 
 */
typedef struct symListItem
{
    Symtable *table;
    struct symListItem *next;
} *symListItemPtr;

/**
 * @brief First element of the list defined as symtable
 * 
 */
typedef symListItemPtr symtableList;

/**
 * @brief Initialise list of symtables
 * 
 * @param list 
 */
void sym_list_init(symtableList *list);

/**
 * @brief Add symtable to the end of the list
 * 
 * @param list List to be added to
 * @param table Symtable to be added
 * @return symListItemPtr Pointer to the item if all good, NULL if failed
 */
symListItemPtr sym_list_add(symtableList *list, Symtable *table);

/**
 * @brief Adds variable (not function) to the last symtable in the list
 * 
 * @note Expects list of symtables with at least 1 symtable
 * 
 * @param symlist List of symtables
 * @param id Name of the variable
 * @param var Token with the variable's value
 * @return Symtable_item* Pointer to the item if succeeded, NULL otherwise
 */
Symtable_item *sym_list_add_to_last(symtableList symlist, char *id, TermType type);

/**
 * @brief Remove last item from the list
 * 
 * @param list List to be removed from
 */
void sym_list_remove_last(symtableList *list);

/**
 * @brief Safely delete list
 * 
 * @param list List to be deleted
 */
void sym_list_dispose(symtableList *list);

#endif // SYMTABLE_LIST_H

