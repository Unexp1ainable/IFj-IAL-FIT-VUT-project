/**
 * @file table_hierarchy.h
 * 
 * @author Timotej Kamenský, xkamen24, +421 944 687 328
 * @author Michal Řezník,  xrezni28, +420731748122
 * @author Jiří Hofírek, xhofir06, +420774883191
 * @author Samuel Repka, xrepka07, +421907587090
 * 
 * @version 0.1
 * @brief implementation of hierarchical system of tables, to deal with scope of variables.
 * @date 2020-11-25
 */

#include <stdlib.h>
#include "table_hierarchy.h"
#include <stdio.h>//debug only


bool stackInit(SymtableStack *symtablestack){
    //alloc the array proper
    if (symtablestack == NULL)
    {
        return false;
    }
    bool alloc = false;
    symtablestack->table = malloc(sizeof(Symtable *) *DEFAULT_STACK_SIZE);
    if (symtablestack->table == NULL)
    {
        return false;
    }
    alloc = true;
    symtablestack->size = DEFAULT_STACK_SIZE;
    symtablestack->top = -1;//-1 == empty
    for (int i = 0;i<symtablestack->size;i++)
    {
        symtablestack->table[i]=NULL;
    }
    return true;
}


bool stackDoubleSize(SymtableStack * symtablestack){
    //basic check
    if (symtablestack == NULL){
        return false;
    }
    //try to realloc, do not overwrite the previous thing if it fails
    void * tmp = realloc(symtablestack->table,sizeof(Symtable)*symtablestack->size*2);
    if (tmp == NULL){
        return false;
    }//save if successful, change size of it
    symtablestack->table = tmp;
    symtablestack->size = symtablestack->size * 2;
    return true;
}



bool stackEmpty(SymtableStack * symtablestack){
    if (symtablestack->top == -1){
        return true;
    }
    else{
        return false;
    }
}
bool stackFull(SymtableStack * symtablestack){
    if (symtablestack->top == symtablestack->size -1){
        return true;
    }
    else{
        return false;
    }
}

bool stackPush(SymtableStack * symtablestack, Symtable * table)
{
    if (symtablestack == NULL || table == NULL){
        return false;
    }
    if (stackFull(symtablestack)){
        bool tmpbool = stackDoubleSize(symtablestack);
        if (tmpbool == false){
            return false;
        }
    }
    symtablestack->table[symtablestack->top+1] = table;
    symtablestack->top++;
    return true;
}


Symtable * stackPop(SymtableStack * symtablestack)
{
    if (stackEmpty(symtablestack)){
        return NULL;
    }
    else
    {
        symtablestack->top--;
        return symtablestack->table[symtablestack->top + 1];
    }
}
void stackFree(SymtableStack * symtablestack)
{
    for (int i = 0;i<symtablestack->top+1;i++){
        symtable_free(symtablestack->table[i]);
    }
    free(symtablestack->table);
    symtablestack->size = 0;
    symtablestack->top = -1;
}
