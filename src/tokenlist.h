#ifndef _TOKENLIST_H
#define _TOKENLIST_H
#include "newscanner.h"
#include <stdlib.h>
/**
* @brief get next token from single linked list created by getnexttoken();
*/
int get_next_token_list(TOKEN * token,TokenList * tokenlist);
bool save_next_token(TOKEN * token, TokenList * tokenlist);
void init_token_list(TokenList * tokenlist);
bool copy_token(TOKEN * dest, TOKEN * src);

typedef struct{
    int head;
    TokenListItem * first;
}TokenList;



typedef struct{
    TOKEN * token;
    TokenListItem * next;
}TokenListItem;





# endif //ifndef tokenlist_h