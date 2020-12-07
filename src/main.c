#include "newscanner.h"
#include "dynamic_string.h"

void print_token(TOKEN t, int code, FILE *fptr)
{
    const char *t_type_str[] = {
        "TOKEN_TYPE_ADD", 
        "TOKEN_TYPE_SUBTRACT", 
        "TOKEN_TYPE_MULTIPLY", 
        "TOKEN_TYPE_DIVIDE",

        "TOKEN_TYPE_EQUAL",
        "TOKEN_TYPE_NOT_EQUAL", 
        "TOKEN_TYPE_LOWER_THAN", 
        "TOKEN_TYPE_GREATER_THAN", 
        "TOKEN_TYPE_LOWER_EQUAL",
        "TOKEN_TYPE_GREATER_EQUAL", 
        "TOKEN_TYPE_SUB1", 
        "TOKEN_TYPE_ADD1", 

        "TOKEN_TYPE_OPENING_CLASSIC_BRACKET",
        "TOKEN_TYPE_CLOSING_CLASSIC_BRACKET", 
        "TOKEN_TYPE_OPENING_CURVY_BRACKET", 
        "TOKEN_TYPE_CLOSING_CURVY_BRACKET",
        "TOKEN_TYPE_COMMA", 
        "TOKEN_TYPE_SEMICOLON", 
        "TOKEN_TYPE_UNDERSCORE", 
        "TOKEN_TYPE_EOF", 
        "TOKEN_TYPE_EOL",
        "TOKEN_TYPE_IDENTIFIER", 

        "TOKEN_TYPE_RESERVED_KEYWORD",
        "TOKEN_TYPE_STRING", 
        "TOKEN_TYPE_INTEGER", 
        "TOKEN_TYPE_FLOAT64", 
        
        "TOKEN_TYPE_DEFINE", 
        "TOKEN_TYPE_ASSIGN", 
        "TOKEN_TYPE_MAIN"};

    const char *keyword_str[] = {
        "KEYWORD_ELSE", 
        "KEYWORD_FOR", 
        "KEYWORD_FUNC", 
        "KEYWORD_IF", 
        "KEYWORD_PACKAGE",
        "KEYWORD_RETURN", 
        "KEYWORD_NONE"
        "KEYWORD_FLOAT64", 
        "KEYWORD_INT", 
        "KEYWORD_STRING", 
        };

    fprintf(fptr,"------New token------\n");
    fprintf(fptr,"Type: %s\n", t_type_str[t.tokentype]);

    if (t.tokentype == TOKEN_TYPE_STRING)
    {
        fprintf(fptr,"Payload: %s\n", t.string->string);
    }
    else if (t.tokentype == TOKEN_TYPE_INTEGER)
    {
        fprintf(fptr,"Payload: %li\n", t.integer);
    }
    else if (t.tokentype == TOKEN_TYPE_FLOAT64)
    {
        fprintf(fptr,"Payload: %f\n", t.floater);
    }
    else if (t.tokentype == TOKEN_TYPE_RESERVED_KEYWORD)
    {
        fprintf(fptr,"Payload: %s\n", keyword_str[t.keyword]);
    }
    else if (t.tokentype == TOKEN_TYPE_IDENTIFIER)
    {
        fprintf(fptr,"Payload: %s\n", t.string->string);
    }
    fprintf(fptr,"Return code: %i\n",code);
    fprintf(fptr,"---------------------\n");
}

int main(/*int argc, char *argv[]*/)
{
    //if (argc == 2)
    // {
    //     fptr1 = fopen(argv[1], "r");
    //     if (fptr1 == NULL)
    //     {
    //         fputs("File not found.\n", stderr);
    //         return 1;
    //     }
    // }
    // else
    // {
    //     fptr1 = stdin;
    // }
    FILE * fptr1 = fopen("output1.out","w");

    FILE * fptr2 = fopen("output2.out","w");
    if (!fptr2 || !fptr1){
        fputs("File not found.\n", stderr);
        return 1;
    }
    dynamic_string_init(&stringbuffer);

    TOKEN * t = malloc(sizeof(TOKEN));
    t->tokentype = TOKEN_TYPE_EMPTY;
    t->string = NULL;
    TokenList  * list = malloc(sizeof(TokenList)); 
    init_token_list(list);
    int r_code = 0;
    printf("lets start:\n");
    fprintf(fptr1,"this is file containing tokens from original scanner.\n\n");
    fprintf(fptr2,"this is file containing tokens from saved token list.\n\n");
    while (!end && !r_code)
    {
        //printf("\n\n->");
        r_code = get_next_token(t,list);
        if(r_code == 0)
            print_token(*t, r_code,fptr1);
        else
            printf("LEX_ERROR\n");
    }
    while(get_next_token_list(&t,list)){
        print_token(*t,r_code,fptr2);
    }
    free_token_list(list);
    //dynamic_string_free(t->string);
    //free(t->string);
    free(t);
    free(list);
    free(stringbuffer.string);
    fclose(fptr1);
    fclose(fptr2);
    return r_code;
}
