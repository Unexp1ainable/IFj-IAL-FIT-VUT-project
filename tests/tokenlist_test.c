#include "tokenlist.h"

int main(){
    TokenListItem list;
    init_token_list(&list);
    printf("expect empty table: \n\n");
    printtokenlist(&list);


    printf("added 4 items of varying types.\n");
    TOKEN token1; token1.tokentype = TOKEN_TYPE_INTEGER; token1.integer = 42;
    TOKEN token2; token2.tokentype = TOKEN_TYPE_FLOAT64; token2.floater = 7.2;
    TOKEN token3; token3.tokentype = TOKEN_TYPE_RESERVED_KEYWORD; token3.keyword = KEYWORD_RETURN;
    TOKEN token4; token4.tokentype = TOKEN_TYPE_STRING; if(!dynamic_string_init(token4.string)){printf("\n\nfailed memory operation\n\n");}
    dynamic_string_add_string(&token4.string,"I WANNA SEE YOU!\n");
    save_next_token(&list,&token1);
    save_next_token(&list,&token2);
    save_next_token(&list,&token3);
    save_next_token(&list,&token4);
    printtokenlist(&list);


    printf("now, i will get them back one by one.\n");
    
}
void printtokenlist(TokenList * list){
    printf("## This is the token table print -> \n");
    TokenListItem * item = list->first;
    int count = 0;
    while(item != NULL){
        if (item->token->tokentype == TOKEN_TYPE_STRING){
            printf("item %d is string: %s .\n",count, item->token->string->string);
        }
        else if (item->token->tokentype == TOKEN_TYPE_INTEGER){
            printf("item %d is integer: %d .\n",count,item->token->integer);
        }
        else if (item->token->tokentype == TOKEN_TYPE_FLOAT64){
            printf("item %d is float:  %f .\n",count,item->token->floater);
        }
        else if (item->token->tokentype == TOKEN_TYPE_RESERVED_KEYWORD){
            printf("item %d is a reserved keyword. too lazy to write them out.\n",count);
        }
        else{
            printf("item %d is something else .\n",count);
        }
        count ++;
        item = item->next;
    }
    if (count == 0){printf("LIST EMPTY!");}
    return;
}