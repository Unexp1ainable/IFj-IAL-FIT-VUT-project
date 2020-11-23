/**
 * @file symtable.h
 * 
 * @author Timotej Kamenský, xkamen24, +421 944 687 328
 * @author Michal Řezník,  xrezni28, +420731748122
 * @author Jiří Hofírek, xhofir06, +420774883191
 * @author Samuel Repka, xrepka07, +421907587090
 * 
 * @brief Header for symbol hash table
 * 
 * @date 2020-11-20
 */
#include <stdbool.h>
#ifndef SYMTABLE_H
#define SYMTABLE_H

#define MAX_SIZE_OF_SYM_TABLE 52937 //random prime number

/**
 * @brief hash function 
 * 
 * @note http://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)HashTables.html 4.2.multiplication
 * @param str keyword to be hashed
 * @return hash
 */
unsigned long hash(char * str);

typedef struct ItemData{
    //TODO INSERT DATA HERE!!!!!!!!!!!!!!!!!!!
    bool defined;
}ItemData;

/**
 * @struct Single item representation
 */
typedef struct htab_tabitem
{
	char * key;
    struct htab_tabitem * next;
    ItemData itemData;
    
} Symtable_item;

typedef Symtable_item * Symtable[MAX_SIZE_OF_SYM_TABLE];//define symbol table as array of symtable items


/**
 * @brief initialiser of symbol table
 * 
 * @param symtable pointer to the initialised table
 */
void symtable_init(Symtable * symtable);


/**
 * @brief add symbol to symtable
 * 
 * @param table pointer to the symbol table
 * @param key keyword
 * @param noerror true if no memory allocation failed
 * @return NULL+true if already existing,NULL+false if failed memory allocation, pointer+true if successful
 */
Symtable_item * symtable_add(Symtable * table, char * key,bool * noerror);

/**
 * @brief find the tableitem by the keyword
 * 
 * @param table symbol table
 * @param key keyword
 * @return NULL if not found, pointer if found
 */
Symtable_item * symtable_search(Symtable * table, char * key);

/**
 * @brief remove tableitem by keyword
 * 
 * @param table symbol table
 * @param key keyword
 */
void symtable_remove(Symtable * table, char * key);

/**
 * @brief clear table and all items of it
 * @param table Symbol table
 */
void symtable_free(Symtable * table);





#endif //_SYMTABLE_H