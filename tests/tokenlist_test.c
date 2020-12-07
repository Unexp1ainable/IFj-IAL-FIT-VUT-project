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
    TOKEN tokeninteger1; tokeninteger1.tokentype = TOKEN_TYPE_INTEGER; tokeninteger1.integer = 42;
    TOKEN tokenfloat1; tokenfloat1.tokentype = TOKEN_TYPE_FLOAT64; tokenfloat1.floater = 7.2;
    TOKEN tokenres1; tokenres1.tokentype = TOKEN_TYPE_RESERVED_KEYWORD; tokenres1.keyword = KEYWORD_RETURN;

    TOKEN tokenstr1; tokenstr1.tokentype = TOKEN_TYPE_STRING;
    Dynamic_string stringfortoken4;
    tokenstr1.string = &stringfortoken4;
    if(!dynamic_string_init(tokenstr1.string)){printf("\n\nfailed memory operation\n\n");}
    dynamic_string_add_string(tokenstr1.string,"THIS IS STRING 1");

    TOKEN tokenstr2; tokenstr2.tokentype = TOKEN_TYPE_STRING; 
    tokenstr2.string= malloc(sizeof(Dynamic_string));
    if(!dynamic_string_init(tokenstr2.string)){printf("\n\nfailed memory operation\n\n");}
    dynamic_string_add_string(tokenstr2.string, "THIS IS STRING 2");

    TOKEN tokenid1; 
    tokenid1.tokentype = TOKEN_TYPE_IDENTIFIER; 
    tokenid1.string = malloc(sizeof(Dynamic_string));
    if(!dynamic_string_init(tokenid1.string)){printf("\n\nfailed memory operation\n\n");}
    dynamic_string_add_string(tokenid1.string, "THIS IS IDENT 1");

    TOKEN tokenstr3; tokenstr3.tokentype = TOKEN_TYPE_STRING; tokenstr3.string = malloc(sizeof(Dynamic_string));if(!dynamic_string_init(tokenstr3.string)){printf("memory error.\n");}
    dynamic_string_add_string(tokenstr3.string, "THIS IS STRING 3");

    TOKEN tokenres2; 
    tokenres2.tokentype = TOKEN_TYPE_RESERVED_KEYWORD;
    tokenres2.keyword = KEYWORD_ELSE;

    TOKEN tokeninteger2;
    tokeninteger2.tokentype = TOKEN_TYPE_INTEGER;
    tokeninteger2.integer = 77;

    TOKEN tokenfloat2;
    tokenfloat2.tokentype = TOKEN_TYPE_FLOAT64;
    tokenfloat2.floater = 1.23;

    TOKEN tokenid2;
    tokenid2.tokentype = TOKEN_TYPE_IDENTIFIER;
    tokenid2.string = malloc(sizeof(Dynamic_string));
    dynamic_string_init(tokenid2.string);
    dynamic_string_add_string(tokenid2.string,"THIS IS IDENT2");

    save_next_token(&list,&tokeninteger1);
    save_next_token(&list,&tokenfloat1);
    save_next_token(&list,&tokenstr1);
    save_next_token(&list,&tokenres1);
    printtokenlist(&list);
    // dynamic_string_free(tokenstr1.string);



    printf("\n\n\nnow, i will get them back one by one.\n\n");
    TOKEN tokenptr; tokenptr.tokentype = TOKEN_TYPE_EMPTY;
    int counter = 0;
    while(get_next_token_list(&tokenptr, &list) == true){
        printtoken(&tokenptr,counter);
        counter ++;
    }



    printf("\n\n\n\n i will now add two strings in a row.\n\n\n");
    

    
    save_next_token(&list,&tokenstr2);
    save_next_token(&list,&tokenid1);
    list.head = 0;
    printtokenlist(&list);
    



    printf("\n\n\n\nnow i will add third string.\n\n\n");

    
    save_next_token(&list,&tokenstr3);
    list.head = 0;
    printtokenlist(&list);
    

    printf("\n\n\n now i will add something completely different (monty python pun intended.\n");

    
    
    if (!save_next_token(&list, &tokenres2))printf("SAVE FAILED!\n\n");
    printtoken(&tokenres2,1);
    list.head = 0;
    printtokenlist(&list);



    

    printf("\n\n and NOW, all the combinations.\n");
    save_next_token(&list,&tokenstr1);
    save_next_token(&list,&tokenstr2);
    save_next_token(&list,&tokenid1);
    save_next_token(&list,&tokenstr3);
    save_next_token(&list,&tokenres1);
    save_next_token(&list,&tokenstr1);
    save_next_token(&list,&tokeninteger1);
    save_next_token(&list,&tokenstr2);
    save_next_token(&list,&tokenfloat1);
    save_next_token(&list,&tokenstr2);
    save_next_token(&list,&tokenid1);
    save_next_token(&list,&tokenid2);
    save_next_token(&list,&tokenres1);
    save_next_token(&list,&tokenid1);
    save_next_token(&list,&tokeninteger1);
    save_next_token(&list,&tokenid2);
    save_next_token(&list,&tokenfloat1);
    save_next_token(&list,&tokenid1);
    save_next_token(&list,&tokenres1);
    save_next_token(&list,&tokenres2);
    save_next_token(&list,&tokeninteger2);
    save_next_token(&list,&tokenres1);
    save_next_token(&list,&tokenfloat2);
    save_next_token(&list,&tokenres2);
    save_next_token(&list,&tokeninteger2);
    save_next_token(&list,&tokeninteger1);
    save_next_token(&list,&tokenfloat1);
    save_next_token(&list,&tokeninteger1);
    save_next_token(&list,&tokenfloat1);
    save_next_token(&list,&tokenfloat2);
    printf("\n\n\none by one\n\n\n");
    TOKEN returntoken;
    counter = 0;

    while(get_next_token_list(&returntoken, &list)){
        printtoken(&returntoken,counter);counter++;
    }
    
    print_token_list(&list);
    print_token_list(&list);
    free_token_list(&list);
    dynamic_string_free(tokenstr1.string);
    dynamic_string_free(tokenstr2.string);
    dynamic_string_free(tokenid1.string);
    dynamic_string_free(tokenid2.string);
    //free(tokenstr1.string);blud
    free(tokenstr2.string);
    free(tokenid1.string);
    free(tokenid2.string);
    dynamic_string_free(tokenstr3.string);
    free(tokenstr3.string);


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
    else if (token->tokentype == TOKEN_TYPE_IDENTIFIER){
        printf("item %d is ident> %s .\n",count,token->string->string);
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
void print_token_list(TokenList * list){
    printf("## This is the token table print, NEW AND IMPROVED -> \n");
    TOKEN token;
    int count = 0;
    while(1){
        if (!get_next_token_list(&token, list))break;
        printtoken(&token,count);
        count ++;
        if (printtoken == TOKEN_TYPE_EMPTY){
            break;
        }
    }
    return;
}
