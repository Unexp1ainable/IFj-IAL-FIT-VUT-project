#include "syntax_common.h"

TOKEN *token_buffer = NULL;
TOKEN *curr_token;
unsigned long int line = 1;
TokenList tokens;
bool first_pass = true;

void get_token(TOKEN **token)
{
    if (token_buffer == NULL)
    {
        if (!get_next_token_list(token, &tokens))
        {
            fputs("Something went extremely wrong. Token list corrupted?\n", stderr);
            exit(1);
        }
    }
    else
    {
        *token = token_buffer;
        token_buffer = NULL;
    }
}

void return_token(TOKEN *token)
{
    token_buffer = token;
}

int s_eols()
{
    do
    {
        get_token(&curr_token);
        line++;
    } while (TOKEN_IS(TOKEN_TYPE_EOL));
    line--;
    return_token(curr_token);
    return NO_ERR;
}

void describe_error(ERR_CODE_SYN err)
{
    switch (err)
    {
    case NO_ERR:
        fprintf(stderr, "Your prayers were answered.Probably.\n");
        break;

    case ERR_PROLOG:
        fprintf(stderr, "Line %li: Prolog invalid or missing.\n", line);
        break;

    case ERR_EOL_EXPECTED:
        fprintf(stderr, "Line %li: Unexpected token, EOL expected.\n", line);
        break;

    case ERR_EOF_EXPECTED:
        fprintf(stderr, "Line %li: Unexpected token, EOF expected.\n", line);
        break;

    case ERR_FUNC_EXPECTED:
        fprintf(stderr, "Line %li: Unexpected token, \"func\" expected.\n", line);
        break;

    case ERR_FUNC_ID_EXPECTED:
        fprintf(stderr, "Line %li: Unexpected token, function name expected.\n", line);
        break;

    case ERR_MULTIPLE_MAIN_FUNC:
        fprintf(stderr, "Line %li: Second definition of func main().\n", line);
        break;

    case ERR_F_PAR_L_BRACKET:
        fprintf(stderr, "Line %li: Function parameter declaration. \"(\" expected.\n", line);
        break;

    case ERR_F_PAR_R_BRACKET:
        fprintf(stderr, "Line %li: Function parameter declaration. \")\" expected.\n", line);
        break;

    case ERR_BODY_START:
        fprintf(stderr, "Line %li: {} block. \"{\" expected.\n", line);
        break;

    case ERR_BODY_END:
        fprintf(stderr, "Line %li: {} block. \"}\" expected.\n", line);
        break;

    case ERR_FUNC_DEF_UNEXPECTED:
        fprintf(stderr, "Line %li: Unexpected token in function parameters definition.\n", line);
        break;

    case ERR_FUNC_DEF_RET_UNEXPECTED:
        fprintf(stderr, "Line %li: Unexpected token in function parameters definition.\n", line);
        break;

    case ERR_STAT_UNEXPECTED:
        fprintf(stderr, "Line %li: Statement expected.\n", line);
        break;

    case ERR_IF_EXPECTED:
        fprintf(stderr, "Line %li: \"if\" expected.\n", line);
        break;

    case ERR_FOR_EXPECTED:
        fprintf(stderr, "Line %li: \"for\" expected.\n", line);
        break;

    case ERR_SEMICOLON_EXPECTED:
        fprintf(stderr, "Line %li: \"for\" expected.\n", line);
        break;

    case ERR_RETURN_EXPECTED:
        fprintf(stderr, "Line %li: \"for\" expected.\n", line);
        break;

    case ERR_COMMA_EXPECTED:
        fprintf(stderr, "Line %li: \",\" or EOL expected.\n", line);
        break;

    case ERR_UNKNOWN_ID_OPERATION:
        fprintf(stderr, "Line %li: Unknown operation with id.\n", line);
        break;

    case ERR_ID_DEF_EXPECTED:
        fprintf(stderr, "Line %li: Variable definition expected.\n", line);
        break;

    case ERR_ID_EXPECTED:
        fprintf(stderr, "Line %li: Identifier expected.\n", line);
        break;

    case ERR_UNEXPECTED_TOKEN:
        fprintf(stderr, "Line %li: Unexpected token.\n", line);
        break;

    case ERR_ID_ASSIGN_EXPECTED:
        fprintf(stderr, "Line %li: \" = \" expected.\n", line);
        break;

    case ERR_TYPE_EXPECTED:
        fprintf(stderr, "Line %li: Data type expected.\n", line);
        break;

    case ERR_ID_REDEFINITION:
        fprintf(stderr, "Line %li: Identifier was already defined.\n", line);
        break;

    case ERR_ID_UNDEFINED:
        fprintf(stderr, "Line %li: Undefined reference.\n", line);
        break;

    case ERR_WRONG_FUNC_PARAM:
        fprintf(stderr, "Line %li: Wrong function parameters.\n", line);
        break;

    case ERR_MAIN_MISSING:
        fprintf(stderr, "No main function defined.\n");
        break;

    case ERR_INVALID_MAIN_DEFINITION:
        fprintf(stderr, "Invalid definition of main function.\n");
        break;

    case ERR_INVALID_EXPRESSION:
        fprintf(stderr, "Line %li: Invalid expression.\n", line);
        break;

    case ERR_WRONG_NUMBER_LVALUES:
        fprintf(stderr, "Line %li: Wrong number of lvalues.\n", line);
        break;

    case ERR_TYPE_MISMATCH:
        fprintf(stderr, "Line %li: ID-term type mismatch.\n", line);
        break;

    case ERR_SYMTABLE:
        fprintf(stderr, "Line %li: FUNC_INIT failure. Symtable error has occured.\n", line);
        break;

    case ERR_MISSING_ELSE:
            fprintf(stderr, "Line %li: Else is missing.\n", line);
            break;

    case  ERR_TOO_MANY_RVALUES:
            fprintf(stderr, "Line %li: Too many Rvalues.\n", line);
            break;

    default:
        fputs("Something is wrong, I can feel it.\n", stderr);
        break;
    }
}

int map_err_code(int code)
{
    // I know that it is probably not the best solution, but I really like it
    if (code < LEXICAL_ERRORS)
        return 0;
    if (code < SYNTAX_ERRORS)
        return LEXICAL_ERROR;
    if (code < SEM_ERR_UNDEFS)
        return SYNTAX_ERROR;
    if (code < SEM_ERR_NEW_VARS)
        return SEM_ERR_UNDEF;
    if (code < SEM_ERR_TYPE_COMPATS)
        return SEM_ERR_NEW_VAR;
    if (code < SEM_ERR_PAR_NUMS)
        return SEM_ERR_TYPE_COMPAT;
    if (code < SEM_ERR_OTHERS)
        return SEM_ERR_PAR_NUM;
    if (code < SEM_ERR_DIV_ZEROS)
        return SEM_ERR_OTHER;
    if (code < INTERN_ERRORS)
        return SEM_ERR_DIV_ZERO;
    else
        return INTERN_ERROR;
}

Symtable_item *was_it_defined(SymtableStack *stack, char *key)
{
    Symtable_item *symbol = NULL;

    for (int i = 0; i <= stack->top; i++){
        symbol = symtable_search(stack->table[stack->top - i], key);
        if(symbol)
            return symbol;
    }

    return NULL;
}

Symtable_item *was_it_defined_top(SymtableStack *stack, char *key)
{
    return symtable_search(stack->table[stack->top], key);
}
