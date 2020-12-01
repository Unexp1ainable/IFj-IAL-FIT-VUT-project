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

Item StackPop(MyStack Stack){
    if(Stack->top == 0){
        return NULL;
    }
    else{
        Stack->top--;
        return Stack->p[Stack->top + 1];
    }
}