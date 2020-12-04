/**
 * @file expression.h
 * 
 * @author Timotej Kamenský, xkamen24
 * @author Michal Řezník,  xrezni28
 * @author Jiří Hofírek, xhofir06
 * @author Samuel Repka, xrepka07
 * 
 * @brief Processing of expressions by precedent analysis
 */

#ifndef EXPRESSION_H
#define EXPRESSION_H
#include "error.h"
<<<<<<< HEAD
#include"newscanner.h"
#include<stdbool.h>
#include"types_definition.h"
#include"symtable.h"
#include"syntax.h"
=======
#include "newscanner.h"
#include "symtable_list.h"
#include <stdbool.h>
//#include "types_definition.h" temporary disabled
>>>>>>> 8d2bd9d10d93a19c8e5a1de05af1d7d371ccc6d7

#define START_STACK_SIZE 32





/*************STACK*************/
typedef struct TheStack{
    int top;            //index of highest item
    int size;           //size of stack
    Item *p;           //array of items
}*MyStack;

/*************TYPE OF ITEM ON STACK*************/
typedef enum{
    IT_TERM,        
    IT_NONTERM,
    IT_OPEN
}ItemType;

/*************TYPE OF TERMS*************/
typedef enum{
    T_INT,              //integer
    T_FLOAT,            //floater
    T_STRING,           //string
    T_EMPTY,            //null
    T_UNKNOWN           //variable use
} TermType;

/*************RELATION************/
typedef enum{
    R_OPEN,       //<
    R_CLOSE,      //>
    R_EQUAL,      //=
    R_EMPTY       //doesn't exist
} Relation;

/*************TYPE OF RELATION************/
typedef enum{
    TR_MUL_DIV,
    TR_ADD_SUB,
    TR_NOT,
    TR_AND_OR,
    TR_GT_LT,     
    TR_EQUAL,      
    TR_VALUE,
    TR_LBRACKET,
    TR_RBRACKET,      
    TR_$
} RelType;

/*************VALUE OF ITEM***********/
typedef union{
    TOKEN term;
    TermType type;
}ItemVal;

/*************ITEM IN STACK***********/

typedef struct item{
    ItemType type;
    ItemVal val;
}*Item;


void InitStack(MyStack *Stack);                           //stack initialization

void DisposeStack(MyStack *Stack);                        //free the stack and its memory

void PushStack(MyStack Stack, Item item);                 //put item to top of the stack

Item PopStack(MyStack Stack);                             //delete the item at the top of the stack and returns it

int FirstFindedTerm(MyStack Stack);                       //find first terminal from top

RelType TokenToTerm(TOKEN_TYPES token);

<<<<<<< HEAD
Relation PrecedenceTable(RelType First, RelType Second);  //with precedence table return relation
=======
int s_expr(symtableList list, TermType *type); // added temporary function in order for gcc to shut up
#endif /* EXPRESSION_H */
>>>>>>> 8d2bd9d10d93a19c8e5a1de05af1d7d371ccc6d7
