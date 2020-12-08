/**
 * @file symtable_list.h
 * 
 * @brief header file for single linked list of symbol tables
 * 
 * @date 2.12.2020
 * 
 * @copyright Brno University of Technology, Czech Republic
 * 
 * @author Samuel Repka,     xrepka07, +421 907 587 090
 * 
 * @author Michal Reznik,    xrezni28, +420 731 748 122
 * @author Jiri Hofirek,     xhofir06, +420 774 883 191
 * @author Timotej Kamensky  xkamen24, +421 944 687 328
 * 
 * */
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

