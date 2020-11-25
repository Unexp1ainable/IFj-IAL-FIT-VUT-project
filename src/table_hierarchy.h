/**
 * @file table_hierarchy.h
 * 
 * @author Timotej Kamenský, xkamen24, +421 944 687 328
 * @author Michal Řezník,  xrezni28, +420731748122
 * @author Jiří Hofírek, xhofir06, +420774883191
 * @author Samuel Repka, xrepka07, +421907587090
 * 
 * @version 0.1
 * @brief header file for hierarchical system of tables, to deal with scope of variables.
 * @date 2020-11-25
 */
/**
 * The system of tables is implemented as a stack of Symtable structs, with dynamic allocation
 * */
#include "symtable.h"
#ifndef TABLE_HIERARCHY_H
#define TABLE_HIERARCHY_H
#define DEFAULT_STACK_SIZE 50
typedef struct {
    Symtable ** table;
    int top;
    int size;
}SymtableStack;


/**
 * @brief Initialises empty stack of tables, used only when declaring
 * @return false if failed operation, true if operation finished successfuly
 * @note initialises the stack to the default size, using dynamic allocation
 * function MUST be accompanied by stackfree, to prevent memory leaks
 * */
bool stackInit(SymtableStack *symtablestack);


/**
 * @brief double the size of dynamically allocated stack, DO NOT USE if unitilialised
 * @param symtablestack stack of symbol tables
 * @return true if done, false if failed
 * */
bool stackDoubleSize(SymtableStack * symtablestack);

/**
 * @brief is the stack empty? yes/no
 * @return true if empty stack, false if not
 * */
bool stackEmpty(SymtableStack * symtablestack);

/**
 * @brief is stack of tables full?
 * */
bool stackFull(SymtableStack * symtablestack);

/**
 * @brief push another table into stack. Can make stack larger
 * @return successfull operation
 * @note can use realloc
 * */
bool stackPush(SymtableStack * symtablestack, Symtable * table);


/**
 * @brief return top table from the table
 * */
Symtable * stackPop(SymtableStack * symtablestack);

/**
 * 
 * */
void stackFree(SymtableStack * symtablestack);

#endif