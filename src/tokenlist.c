#include "tokenlist.h"
int get_next_token_list(TOKEN * token, TokenList * tokenlist){
    TokenListItem * foundtokenitem = tokenlist->first;
    for (int i = 0; i<tokenlist->head;i++){
        foundtokenitem = foundtokenitem->next;
    }
    tokenlist->head++;
    if(!copy_token(token,foundtokenitem)){return false;}
    return true;
}



bool save_next_token(TOKEN * token, TokenList * tokenlist){

    TOKEN * newtoken = malloc(sizeof(TOKEN));
    if (!newtoken){return false;}

    TokenListItem * listitem = malloc(sizeof(TokenListItem));
    if (listitem == NULL){free(newtoken);return false;}

    if(!copy_token(newtoken,token)){free(newtoken);free(listitem);return false;}

    TokenListItem * here = tokenlist->first;
    while(here != NULL){
        here = here->next;
    }
    here = newtoken;
    return true;
}



void init_token_list(TokenList * tokenlist){
    tokenlist->head = 1;
}


bool copy_token(TOKEN * dest, TOKEN * src){
    if (dest == NULL || src == NULL){
        return false;
    }
    dest->tokentype = src->tokentype;
    if (src->tokentype == TOKEN_TYPE_STRING){
        if(!dynamic_string_copy_string(dest,src)){return false;}
    }
    else if(src->tokentype = TOKEN_TYPE_INTEGER){
        dest->integer = src->integer ;
    }
    else if(src->tokentype = TOKEN_TYPE_FLOAT64){
        dest->floater = src->floater;
    }
    else if(src->tokentype = TOKEN_TYPE_RESERVED_KEYWORD){
        dest->keyword = src->keyword;
    }
    return true;
}


void free_token_list(TokenList * tokenlist){
    if (tokenlist == NULL){return;}
    TokenListItem * current = tokenlist->first;
    TokenListItem * previous = NULL;
    while(current != NULL){
        previous = current;
        current = current ->next;
        free(previous->token);
        free(previous);
    }
    return;
}