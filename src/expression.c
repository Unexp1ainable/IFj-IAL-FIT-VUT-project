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
#include"expression.h"



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
            return TR_LBRACKET;
        case TOKEN_TYPE_CLOSING_CLASSIC_BRACKET:
            return TR_RBRACKET;
        case TOKEN_TYPE_INTEGER:
            return TR_VALUE;
        case TOKEN_TYPE_FLOAT64:
            return TR_VALUE;
        case TOKEN_TYPE_IDENTIFIER:
            return TR_VALUE;
        case TOKEN_TYPE_STRING:
            return TR_VALUE;
        default:
            return TR_OTHER;

    }
}

Relation PrecedenceTable(RelType First, RelType Second){
    switch(First){
        case TR_ADD_SUB:
            if(Second == TR_LBRACKET || Second == TR_VALUE || Second == TR_MUL_DIV){
                return R_OPEN;
            }
            else if(Second == TR_NOT){
                return R_EMPTY;
            }
            return R_CLOSE;
        case TR_MUL_DIV:
            if(Second == TR_LBRACKET || Second == TR_VALUE){
                return R_OPEN;
            }
            else if(Second == TR_NOT){
                return R_EMPTY;
            }
            return R_CLOSE;
        case TR_EQUAL:
            if(Second == TR_RBRACKET || Second == TR_AND_OR || Second == TR_OTHER ){
                return R_CLOSE;
            }
            else if(Second == TR_EQUAL){
                return R_EMPTY;
            }
            else return R_OPEN;
        case TR_GT_LT:
            if( Second == TR_RBRACKET ||Second == TR_EQUAL | Second == TR_OTHER || Second == TR_AND_OR){
                return R_CLOSE;
            }
            else if(Second == TR_GT_LT){
                return R_EMPTY;
            }
            else return R_OPEN;
        case TR_LBRACKET:
            if(Second == TR_OTHER){
                 return R_EMPTY;
            }
            else if(Second == TR_RBRACKET){
                return R_EQUAL;
            }
            else return R_OPEN;
        case TR_RBRACKET:
            if(Second = TR_LBRACKET || Second == TR_VALUE || Second == TR_NOT){
                return R_EMPTY;
            }
            else return R_CLOSE;
        case TR_NOT:
            if(Second == TR_RBRACKET || Second == TR_OTHER || Second == TR_AND_OR){
                return R_CLOSE;
            }
            else return R_OPEN;
        case TR_VALUE:
            if(Second == TR_LBRACKET || Second == TR_VALUE || Second == TR_NOT){
                return R_EMPTY;
            }
            else return R_CLOSE;
        case TR_AND_OR:
            if(Second == TR_RBRACKET || Second == TR_OTHER){
                return R_CLOSE;
            }
            else return R_OPEN;
        case TR_OTHER:
            if(Second == TR_RBRACKET || Second == TR_OTHER){
                return R_EMPTY;
            }
            else return R_OPEN;
        
    }
    return R_EMPTY;
}
 int CheckAndParse(MyStack stack, Symtable Table){
    Item item = PopStack(stack);
    if(item->type == IT_TERM){
        if(item->val.term.tokentype == TOKEN_TYPE_CLOSING_CLASSIC_BRACKET){
            free(item);
            item = PopStack(stack);
            Item leftbracket = PopStack(stack);    //<expression> => ( <expression> )
            free(leftbracket);
         }
        else{
            char *interpret;
            TermType type = T_UNKNOWN;
            switch(item->val.term.tokentype){
                case TOKEN_TYPE_INTEGER:
                    type =  T_INT;
                    *interpret = malloc(sizeof(char) * 60);
                    if(interpret == NULL){
                        return INTERN_ERROR;
                        }
                    sprintf(interpret,"#INT#%ld", strtol(item->val.term.string, NULL,10));
                    realloc(interpret, strlen(interpret));
                    if(interpret == NULL){
                        return INTERN_ERROR;
                    }
                break;
                case TOKEN_TYPE_FLOAT64:
                    type = T_FLOAT;
                   *interpret = malloc(sizeof(char) * 60);
                    if(interpret == NULL){
                        return INTERN_ERROR;
                        }
                    sprintf(interpret,"#FLOAT#%ld", strtod(item->val.term.string, NULL));
                    realloc(interpret, strlen(interpret));
                    if(interpret == NULL){
                        return INTERN_ERROR;
                    }
                break;
                case TOKEN_TYPE_STRING:

                    *interpret = malloc(sizeof(char)*(strlen(item->val.term.string)+10));
                    if(interpret == NULL){
                        return INTERN_ERROR;
                    }
                    strcpy(interpret,"#STRING#");
                    strcat(interpret,item->val.term.string);
                    break;
                case TOKEN_TYPE_IDENTIFIER:
                    if(was_it_defined(Table, item->val.term.string) == false){
                        free(item);
                        return SEM_ERR_UNDEF;
                    }
                    else{
                        *interpret = malloc(sizeof(char)*(strlen(item->val.term.string)+5));
                            if(interpret == NULL){
                                return INTERN_ERROR;
                            }
                        strcpy(interpret, "#ID#");
                        strcat(interpret, item->val.term.string);
                        break;
                    }
                    break;    
                case TOKEN_TYPE_EMPTY:
                    *interpret = malloc(sizeof(char)*7);
                        if(interpret == NULL){
                                return INTERN_ERROR;
                        }
                    strcpy(interpret, "#NULL#");
                    type = T_EMPTY;
                    break;
                default:
                    free(item);
                    return INTERN_ERROR;
                
            }
            item->type = IT_NONTERM;
            free(interpret);
            item->val.type = type;
        }
        free(PopStack(stack));
        PushStack(stack,item);
        return 0; //item na zásobník
    }

    




 }
