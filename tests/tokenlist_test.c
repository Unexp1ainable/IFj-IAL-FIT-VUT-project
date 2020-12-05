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

    printf("\n\n\n\n i will now add two strings in a row.\n\n\n");
    TOKEN token5; token5.tokentype = TOKEN_TYPE_STRING; token5.string= malloc(sizeof(Dynamic_string));if(!dynamic_string_init(token5.string)){printf("\n\nfailed memory operation\n\n");}
    TOKEN token6; token6.tokentype = TOKEN_TYPE_STRING; token6.string = malloc(sizeof(Dynamic_string));if(!dynamic_string_init(token6.string)){printf("\n\nfailed memory operation\n\n");}
    dynamic_string_add_string(token5.string, "NUMBER 1 here");
    dynamic_string_add_string(token6.string, "NUMBER 2 here");
    save_next_token(&list,&token5);
    save_next_token(&list,&token6);
    list.head = 0;
    printtokenlist(&list);
    dynamic_string_free(token5.string);
    dynamic_string_free(token6.string);
    free(token5.string);
    free(token6.string);



    printf("\n\n\n\nnow i will add third string.\n\n\n");

    TOKEN token7; token7.tokentype = TOKEN_TYPE_STRING; token7.string = malloc(sizeof(Dynamic_string));if(!dynamic_string_init(token7.string)){printf("memory error.\n");}
    dynamic_string_add_string(token7.string, "NUMBER 3 here");
    save_next_token(&list,&token7);
    list.head = 0;
    printtokenlist(&list);
    dynamic_string_free(token7.string);
    free(token7.string);

    printf("\n\n\n now i will add something completely different (monty python pun intended.\n");

    TOKEN token8; token8.tokentype = TOKEN_TYPE_RESERVED_KEYWORD;
    token8.keyword = KEYWORD_ELSE;
    save_next_token(&list, &token8);
    list.head = 0;
    printtokenlist(&list);

    
    //
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
