#include "../src/newscanner.h"
#include "../src/dynamic_string.h"
void printtokenlist(TokenList * list);
void printtoken(TOKEN * token, int count);
int main(){
    TokenList list;
    init_token_list(&list);
    printf("expect empty table: \n\n");
    printtokenlist(&list);


    printf("\n\n\nadded 4 items of varying types.\n");
    TOKEN token1; token1.tokentype = TOKEN_TYPE_INTEGER; token1.integer = 42;
    TOKEN token2; token2.tokentype = TOKEN_TYPE_FLOAT64; token2.floater = 7.2;
    TOKEN token3; token3.tokentype = TOKEN_TYPE_RESERVED_KEYWORD; token3.keyword = KEYWORD_RETURN;
    TOKEN token4; token4.tokentype = TOKEN_TYPE_STRING;
    Dynamic_string stringfortoken4;
    token4.string = &stringfortoken4;
    if(!dynamic_string_init(token4.string)){printf("\n\nfailed memory operation\n\n");}
    dynamic_string_add_string(token4.string,"I WANNA SEE YOU!");
    save_next_token(&list,&token1);
    save_next_token(&list,&token2);
    save_next_token(&list,&token4);
    save_next_token(&list,&token3);
    printtokenlist(&list);
    dynamic_string_free(token4.string);
    printf("\n\n\nnow, i will get them back one by one.\n\n");
    TOKEN tokenptr; tokenptr.tokentype = TOKEN_TYPE_EMPTY;
    int counter = 0;
    while(get_next_token_list(&tokenptr, &list) == true){
        printtoken(&tokenptr,counter);
        counter ++;
    }
    free_token_list(&list);
    printf("################    END    #############################################################################################################################################################################.\n");
    
}

void printtoken(TOKEN * token, int count){
    if (token->tokentype == TOKEN_TYPE_STRING){
        printf("item %d is string: %s .\n",count,token->string->string);
    }
    else if (token->tokentype == TOKEN_TYPE_INTEGER){
        printf("item %d is integer: %ld .\n",count,token->integer);
    }
    else if (token->tokentype == TOKEN_TYPE_FLOAT64){
        printf("item %d is float:  %f .\n",count,token->floater);
    }
    else if (token->tokentype == TOKEN_TYPE_RESERVED_KEYWORD){
        printf("item %d is a reserved keyword. too lazy to write them out.\n",count);
    }
    else{
        printf("item %d is something else .\n",count);
    }
    return;
}
void printtokenlist(TokenList * list){
    printf("## This is the token table print -> \n");
    TokenListItem * item = list->first;
    int count = 0;
    while(item != NULL){
        printtoken(item->token,count);
        count ++;
        item = item->next;
    }
    if (count == 0){printf("LIST EMPTY!");}
    return;
}
