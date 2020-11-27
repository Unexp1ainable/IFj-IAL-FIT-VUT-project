#include "newscanner.h"
#include "dynamic_string.h"

void print_token(TOKEN t, int code)
{
    const char *t_type_str[] = {
        "TOKEN_TYPE_ADD", "TOKEN_TYPE_SUBTRACT", "TOKEN_TYPE_MULTIPLY", "TOKEN_TYPE_DIVIDE", "TOKEN_TYPE_EQUAL",
        "TOKEN_TYPE_NOT_EQUAL", "TOKEN_TYPE_LOWER_THAN", "TOKEN_TYPE_GREATER_THAN", "TOKEN_TYPE_LOWER_EQUAL",
        "TOKEN_TYPE_GREATER_EQUAL", "TOKEN_TYPE_SUB1", "TOKEN_TYPE_ADD1", "TOKEN_TYPE_OPENING_CLASSIC_BRACKET",
        "TOKEN_TYPE_CLOSING_CLASSIC_BRACKET", "TOKEN_TYPE_OPENING_CURVY_BRACKET", "TOKEN_TYPE_CLOSING_CURVY_BRACKET",
        "TOKEN_TYPE_COMMA", "TOKEN_TYPE_SEMICOLON", "TOKEN_TYPE_UNDERSCORE", "TOKEN_TYPE_EOF", "TOKEN_TYPE_EOL",
        "TOKEN_TYPE_IDENTIFIER", "TOKEN_TYPE_STRING", "TOKEN_TYPE_INTEGER", "TOKEN_TYPE_FLOAT64", "TOKEN_TYPE_RESERVED_KEYWORD",
        "TOKEN_TYPE_DEFINE_AND_ASSIGN", "TOKEN_TYPE_ASSIGN", "TOKEN_TYPE_MAIN"};

    const char *keyword_str[] = {
        "KEYWORD_ELSE", "KEYWORD_FLOAT64", "KEYWORD_FOR", "KEYWORD_FUNC", "KEYWORD_IF", "KEYWORD_INT", "KEYWORD_PACKAGE",
        "KEYWORD_RETURN", "KEYWORD_STRING", "KEYWORD_NONE"};

    printf("------New token------\n");
    printf("Type: %s\n", t_type_str[t.tokentype]);

    if (t.tokentype == TOKEN_TYPE_STRING)
    {
        printf("Payload: %s\n", t.string->string);
    }
    else if (t.tokentype == TOKEN_TYPE_INTEGER)
    {
        printf("Payload: %li\n", t.integer);
    }
    else if (t.tokentype == TOKEN_TYPE_FLOAT64)
    {
        printf("Payload: %f\n", t.floater);
    }
    else if (t.tokentype == TOKEN_TYPE_RESERVED_KEYWORD)
    {
        printf("Payload: %s\n", keyword_str[t.keyword]);
    }
    else if (t.tokentype == TOKEN_TYPE_IDENTIFIER)
    {
        printf("Payload: %s\n", t.string->string);
    }
    printf("Return code: %i\n",code);
    printf("---------------------\n");
}

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        fileptr = fopen(argv[1], "r");
        if (fileptr == NULL)
        {
            fputs("File not found.\n", stderr);
            return 1;
        }
    }
    else
    {
        fileptr = stdin;
    }

    TOKEN t = {TOKEN_TYPE_EMPTY, {0}};
    int r_code;
    while (true)
    {
        r_code = get_next_token(&t);
        print_token(t, r_code);
    }

    return 0;
}
