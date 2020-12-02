/**
 * @file expression.c
 * 
 * @author Timotej Kamenský, xkamen24
 * @author Michal Řezník,  xrezni28
 * @author Jiří Hofírek, xhofir06
 * @author Samuel Repka, xrepka07
 * 
 * @brief  Processing of expressions by precedent analysis
 */
#include "expression.h"



void InitStack(MyStack *Stack){
    if(Stack == NULL){
            return;
    }
    else
    {
        *Stack = malloc(sizeof(struct TheStack));
        if((*Stack) == NULL){
            return INTERN_ERROR;
        }
        (*Stack)->p = malloc((START_STACK_SIZE)*sizeof(Item));
        if(((*Stack)->p) == NULL){
            return INTERN_ERROR;
        }
        (*Stack)->top = 0;
        (*Stack)->size = START_STACK_SIZE;
    }
    
}

void DisposeStack(MyStack *Stack){
    if((*Stack == NULL || Stack == NULL)){
        return;
    }
    int i = (*Stack)->top;
    for( i; i > 0; i--){
        free((*Stack)->p[i]);
    }

    free((*Stack)->p);
    free(*Stack);
    *Stack = NULL;
}

void PushStack(MyStack Stack, Item item){
    Stack->top++;
    if(Stack->size = Stack->top){
        Stack->size = Stack->top + START_STACK_SIZE;
        Stack->p = realloc(Stack->p, sizeof(Item) * Stack->size);
        if(Stack->p){
                return INTERN_ERROR;
        }
    }
    Stack->p[Stack->top] = item;
}

Item PopStack(MyStack Stack){
    if(Stack->top == 0){
        return NULL;
    }
    else{
        Stack->top--;
        return Stack->p[Stack->top + 1];
    }
}
//return first term from top
int FirstFindedTerm(MyStack Stack){
    int position ;
    position = Stack->top;
    while(position != 0){
        if(Stack->p[position]->type == IT_TERM){
            return position;
        }
        position--;
    }
    return position;
}
RelType TokenToTerm(TOKEN_TYPES token){
    switch(token){
        //TODO LOGIC
        case TOKEN_TYPE_ADD:
            return TR_ADD_SUB;
        case TOKEN_TYPE_SUBTRACT:
            return TR_ADD_SUB;
        case TOKEN_TYPE_MULTIPLY:
            return TR_MUL_DIV;
        case TOKEN_TYPE_DIVIDE:
            return TR_MUL_DIV;
        case TOKEN_TYPE_EQUAL:
            return TR_EQUAL;   
        case TOKEN_TYPE_NOT_EQUAL:
            return TR_EQUAL;
        case TOKEN_TYPE_OPENING_CLASSIC_BRACKET:
            return TR_LB;
        case TOKEN_TYPE_CLOSING_CLASSIC_BRACKET:
            return TR_RB;
        case TOKEN_TYPE_INTEGER:
            return TR_VAR;
        case TOKEN_TYPE_FLOAT64:
            return TR_VAR;
        case TOKEN_TYPE_IDENTIFIER:
            return TR_VAR;
        case TOKEN_TYPE_STRING:
            return TR_VAR;
        default:
            return TR_OTHER;

    }
}
/*
	    |+- | /*| \ | r | ( | ) | i | $ |
    +-  | > | < | < | > | < | > | < | > | 
    /*  | > | > | > | > | < | > | < | > | 
    \   | > | > | > | > | < | > | < | > | 
relation| < | < | < |   | < | > | < | > | 
    (   | < | < | < | < | < | = | < |   |  
    )   | > | > | > | > |   | > |   | > | 
  	i   | > | > | > | > |   | > |   | > | 
 	$   | < | < | < | < | < |   | < |   | 
*/
Relation CheckRelation(RelType First, RelType Second){
    switch(First){
        case TR_ADD_SUB:
            if(Second == TR_LB || Second == TR_VAR){
                return R_OPEN;
            }
            else if(Second == TR_NOT){
                return
            }


    }
}
