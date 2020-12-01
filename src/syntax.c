#include "syntax.h"
#include "newscanner.h"
#include "expression.h"

// ########################## global variables #########################

TOKEN token_buffer = {TOKEN_TYPE_EMPTY, {NULL}};
TOKEN curr_token;
unsigned long int line = 1;
bool main_defined = false;
unsigned int assign_list_id_n = 1;
unsigned int assign_list_expr_n = 0;

// ############################# helper functions ###########################

int get_token(TOKEN *token)
{
    if (token_buffer.tokentype == TOKEN_TYPE_EMPTY)
    {
        int r_code = get_next_token(token);
        if (!r_code)
        {
            return 0;
        }
        else
        {
            exit(1);
        }
    }
    else
    {
        memcpy(token, &token_buffer, sizeof(TOKEN));
        token_buffer.tokentype = TOKEN_TYPE_EMPTY;
        return 0;
    }
}

void return_token(TOKEN token)
{
    memcpy(&token_buffer, &token, sizeof(TOKEN));
}

void describe_error(ERR_CODE_SYN err)
{
    switch (err)
    {
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

    default:
        fputs("Something is wrong, I can feel it.\n", stderr);
        break;
    }
}

bool eol_required()
{
    get_token(&curr_token);
    if (curr_token.tokentype != TOKEN_TYPE_EOL)
    {
        return_token(curr_token);
        return false;
    }

    line++;
    return true;
}

// ################### end of helper functions #################

// ############################# STATES ##################################

int s_prolog()
{
    s_eols();
    get_token(&curr_token); // TODO failure check - possibly implement in get_token?

    // package
    if (curr_token.tokentype != TOKEN_TYPE_RESERVED_KEYWORD ||
        curr_token.keyword != KEYWORD_PACKAGE)
    {
        return ERR_PROLOG;
    }

    // main
    get_token(&curr_token);
    if (curr_token.tokentype != TOKEN_TYPE_MAIN)
    {
        return ERR_PROLOG;
    }

    // eol
    if (!eol_required())
    {
        return ERR_EOL_EXPECTED;
    }

    // <eols>
    s_eols();

    // <f_list>
    int r_code = s_f_list();
    if (r_code)
    {
        return r_code;
    }

    // <eols>
    s_eols();

    // eof
    get_token(&curr_token);
    if (curr_token.tokentype != TOKEN_TYPE_EOF)
    {
        return ERR_EOF_EXPECTED;
    }

    return 0;
} // ---------- end of s_prolog() -------------

int s_f_list()
{
    // eof
    get_token(&curr_token);
    return_token(curr_token);
    if (curr_token.tokentype == TOKEN_TYPE_EOF)
    {

        return 0;
    }

    // func
    get_token(&curr_token);
    if (curr_token.tokentype != TOKEN_TYPE_RESERVED_KEYWORD ||
        curr_token.keyword != KEYWORD_FUNC)
    {
        return ERR_FUNC_EXPECTED;
    }

    // <func>
    int r_code = s_func();
    if (r_code)
    {
        return r_code;
    }

    // <eols>
    s_eols();

    // f_list
    return s_f_list();
}

int s_func()
{
    //main || id
    get_token(&curr_token);
    if (curr_token.tokentype == TOKEN_TYPE_MAIN)
    {
        if (main_defined)
        {
            return ERR_MULTIPLE_MAIN_FUNC;
        }
        else
        {
            main_defined = true;
        }
    }
    else if (curr_token.tokentype == TOKEN_TYPE_IDENTIFIER)
    {
        // TODO symtable work
    }
    else
    {
        return ERR_FUNC_ID_EXPECTED;
    }

    int r_code = s_f_init();
    if (r_code)
    {
        return r_code;
    }

    r_code = s_body();

    return r_code;
}

int s_f_init()
{
    // (
    get_token(&curr_token);
    if (curr_token.tokentype != TOKEN_TYPE_OPENING_CLASSIC_BRACKET)
    {
        return ERR_F_PAR_L_BRACKET;
    }

    // param_def_list
    int r_code = s_param_def_list();
    if (r_code)
    {
        return r_code;
    }

    // )
    get_token(&curr_token);
    if (curr_token.tokentype != TOKEN_TYPE_CLOSING_CLASSIC_BRACKET)
    {
        return ERR_F_PAR_R_BRACKET;
    }

    // ret_t_list
    r_code = s_ret_t_list();

    return r_code;
}

int s_f_call()
{
    // param_def_list
    get_token(&curr_token);
    if (TOKEN_IS_NOT(TOKEN_TYPE_OPENING_CLASSIC_BRACKET))
    {
        return ERR_UNEXPECTED_TOKEN;
    }
    return s_param_list();
}

int s_body()
{
    // {
    get_token(&curr_token);
    if (curr_token.tokentype != TOKEN_TYPE_OPENING_CURVY_BRACKET)
    {
        return ERR_BODY_START;
    }

    // eol
    if (!eol_required())
    {
        return ERR_EOL_EXPECTED;
    }

    // <eols>
    s_eols();

    // <stat_list>
    int r_code = s_stat_list();
    if (r_code)
    {
        return r_code;
    }

    // }
    get_token(&curr_token);
    if (curr_token.tokentype != TOKEN_TYPE_CLOSING_CURVY_BRACKET)
    {
        return ERR_BODY_START;
    }

    // eol
    if (!eol_required())
    {
        return ERR_EOL_EXPECTED;
    }

    // <eols>
    s_eols();

    return 0;
}

int s_param_def_list()
{
    // e
    get_token(&curr_token);
    if (TOKEN_IS(TOKEN_TYPE_CLOSING_CLASSIC_BRACKET))
    {
        return_token(curr_token);
        return 0;
    }

    // id
    if (TOKEN_IS_NOT(TOKEN_TYPE_IDENTIFIER))
    {
        return ERR_FUNC_DEF_UNEXPECTED;
    }

    // <type>
    int r_code = s_type();
    if (r_code)
    {
        return r_code;
    }

    // <param_def_list_n>
    r_code = s_param_def_list_n();

    return r_code;
}

int s_param_def_list_n()
{
    // e
    get_token(&curr_token);
    if (TOKEN_IS(TOKEN_TYPE_CLOSING_CLASSIC_BRACKET))
    {
        return_token(curr_token);
        return 0;
    }

    // ,
    if (TOKEN_IS_NOT(TOKEN_TYPE_COMMA))
    {
        return ERR_FUNC_DEF_UNEXPECTED;
    }

    // eols
    s_eols();

    // id
    get_token(&curr_token);
    if (TOKEN_IS_NOT(TOKEN_TYPE_IDENTIFIER))
    {
        return ERR_ID_EXPECTED;
    }

    // <param_def_list_n>
    int r_code = s_type();
    if (r_code)
    {
        return r_code;
    }

    // <param_def_list_n>
    r_code = s_param_def_list_n();

    return r_code;
}

int s_ret_t_list()
{
    // e
    get_token(&curr_token);
    if (TOKEN_IS(TOKEN_TYPE_OPENING_CURVY_BRACKET))
    {
        return_token(curr_token);
        return 0;
    }

    // (
    if (TOKEN_IS_NOT(TOKEN_TYPE_OPENING_CLASSIC_BRACKET))
    {
        return ERR_FUNC_DEF_RET_UNEXPECTED;
    }

    // <type>
    int r_code = s_type();
    if (r_code)
    {
        return r_code;
    }
    // <ret_t_list_n>
    r_code = s_ret_t_list_n();
    if (r_code)
    {
        return r_code;
    }

    // )
    get_token(&curr_token);
    if (TOKEN_IS_NOT(TOKEN_TYPE_CLOSING_CLASSIC_BRACKET))
    {
        return ERR_FUNC_DEF_RET_UNEXPECTED;
    }

    return 0;
}

int s_ret_t_list_n()
{
    //     <ret_t_list_n> -> , <type> <ret_t_list_n>
    // <ret_t_list_n> -> e

    // e
    get_token(&curr_token);
    if (TOKEN_IS_NOT(TOKEN_TYPE_OPENING_CLASSIC_BRACKET))
    {
        return_token(curr_token);
        return 0;
    }

    if (TOKEN_IS_NOT(TOKEN_TYPE_COMMA))
    {
        return ERR_FUNC_DEF_RET_UNEXPECTED;
    }

    // <type>
    int r_code = s_type();
    if (r_code)
    {
        return r_code;
    }

    // <ret_t_list_n>
    r_code = s_ret_t_list_n();
    if (r_code)
    {
        return r_code;
    }

    return 0;
}

int s_stat()
{
    get_token(&curr_token);
    // <stat> -> e
    if (TOKEN_IS(TOKEN_TYPE_CLOSING_CURVY_BRACKET))
    {
        return_token(curr_token);
        return 0;
    }
    // <stat> -> id <id_n>
    if (TOKEN_IS(TOKEN_TYPE_IDENTIFIER))
    {
        return s_id_n();
    }
    if (TOKEN_IS_NOT(TOKEN_TYPE_RESERVED_KEYWORD))
    {
        return ERR_STAT_UNEXPECTED;
    }

    switch (curr_token.keyword)
    {
    // <if>
    case KEYWORD_IF:
        return_token(curr_token);
        return s_if();
        break;

    // <stat> -> <for>
    case KEYWORD_FOR:
        return_token(curr_token);
        return s_for();
        break;

    // <stat> -> <return>
    case KEYWORD_RETURN:
        return_token(curr_token);
        return s_return();
        break;

    default:
        return ERR_STAT_UNEXPECTED;
        break;
    }
}

int s_stat_list()
{
    // e
    get_token(&curr_token);
    return_token(curr_token);
    if (TOKEN_IS(TOKEN_TYPE_CLOSING_CURVY_BRACKET))
    {
        return 0;
    }

    // <stat>
    int r_code = s_stat();
    if (r_code)
    {
        return r_code;
    }

    // eols
    if (!eol_required())
    {
        return ERR_EOL_EXPECTED;
    }

    s_eols();

    // <stat-list>
    r_code = s_stat_list();

    return r_code;
}

int s_if()
{
    //if
    get_token(&curr_token);
    if (TOKEN_IS_NOT(TOKEN_TYPE_RESERVED_KEYWORD) || curr_token.keyword != KEYWORD_IF)
    {
        return ERR_IF_EXPECTED;
    }
    int r_code = s_expr();
    if (r_code)
    {
        return r_code;
    }

    // <body>
    r_code = s_body();
    if (r_code)
    {
        return r_code;
    }

    // <else>
    r_code = s_else();
    if (r_code)
    {
        return r_code;
    }

    return 0;
}

int s_else()
{
    get_token(&curr_token);
    if (TOKEN_IS(TOKEN_TYPE_RESERVED_KEYWORD) && curr_token.keyword == KEYWORD_ELSE)
    {
        return s_body();
    }
    return_token(curr_token);
    return 0;
}

int s_for()
{
    // for
    get_token(&curr_token);
    if (TOKEN_IS_NOT(TOKEN_TYPE_RESERVED_KEYWORD) || curr_token.keyword != KEYWORD_FOR)
    {
        return ERR_FOR_EXPECTED;
    }

    // <id_def_v>
    int r_code = s_id_def_v();
    if (r_code)
    {
        return r_code;
    }

    // ;
    if (TOKEN_IS_NOT(TOKEN_TYPE_SEMICOLON))
    {
        return ERR_SEMICOLON_EXPECTED;
    }

    // <expr>
    r_code = s_expr();
    if (r_code)
    {
        return r_code;
    }

    // ;
    if (TOKEN_IS_NOT(TOKEN_TYPE_SEMICOLON))
    {
        return ERR_SEMICOLON_EXPECTED;
    }

    // <id_assign_v>
    r_code = s_id_assign_v();
    if (r_code)
    {
        return r_code;
    }

    // <body>
    return s_body();
}

int s_return()
{
    get_token(&curr_token);
    if (TOKEN_IS_NOT(TOKEN_TYPE_RESERVED_KEYWORD) || curr_token.keyword != KEYWORD_RETURN)
    {
        return ERR_RETURN_EXPECTED;
    }

    return s_expr_list();
}

int s_expr_list()
{
    // e
    get_token(&curr_token);
    return_token(curr_token);

    if (TOKEN_IS(TOKEN_TYPE_EOL))
    {
        return 0;
    }

    // <expr>
    int r_code = s_expr();
    if (r_code)
    {
        return r_code;
    }

    // <expr_list_n
    return s_expr_list_n();
}

int s_expr_list_n()
{
    get_token(&curr_token);

    // e
    if (TOKEN_IS(TOKEN_TYPE_EOL))
    {
        return_token(curr_token);
        return 0;
    }

    // ,
    if (TOKEN_IS_NOT(TOKEN_TYPE_COMMA))
    {
        return ERR_COMMA_EXPECTED;
    }

    // <expr>
    int r_code = s_expr();
    if (r_code)
    {
        return r_code;
    }

    // <expr_list_n>
    return s_expr_list_n();
}

int s_id_n()
{
    get_token(&curr_token);
    return_token(curr_token);
    switch (curr_token.tokentype)
    {
    // :=
    case TOKEN_TYPE_DEFINE:
        return s_id_def();
        break;

    // =
    case TOKEN_TYPE_ASSIGN:
        return s_id_assign();
        break;

    // ,
    case TOKEN_TYPE_COMMA:
        return s_id_list_assign();
        break;

    // (
    case TOKEN_TYPE_OPENING_CLASSIC_BRACKET:
        return s_f_call();
        break;

    default:
        return ERR_UNKNOWN_ID_OPERATION;
        break;
    }
}

int s_id_def()
{
    get_token(&curr_token);
    if (TOKEN_IS_NOT(TOKEN_TYPE_DEFINE))
    {
        return ERR_ID_DEF_EXPECTED;
    }

    return s_expr();
}

int s_id_def_v()
{
    get_token(&curr_token);
    if (TOKEN_IS(TOKEN_TYPE_IDENTIFIER))
    {
        get_token(&curr_token);
        return s_id_def();
    }

    return_token(curr_token);
    return 0;
}

int s_id_list()
{
    get_token(&curr_token);
    if (TOKEN_IS_NOT(TOKEN_TYPE_IDENTIFIER) && TOKEN_IS_NOT(TOKEN_TYPE_UNDERSCORE))
    {
        return ERR_ID_EXPECTED;
    }

    return s_id_list_n();
}

int s_id_list_n()
{
    get_token(&curr_token);
    if (TOKEN_IS(TOKEN_TYPE_ASSIGN))
    {
        return_token(curr_token);
        return 0;
    }

    if (TOKEN_IS(TOKEN_TYPE_COMMA))
    {
        assign_list_id_n++;

        get_token(&curr_token);
        if (TOKEN_IS(TOKEN_TYPE_IDENTIFIER))
        {
            return s_id_list_n();
        }

        return ERR_ID_EXPECTED;
    }

    return ERR_UNEXPECTED_TOKEN;
}

int s_id_assign()
{
    get_token(&curr_token);
    if (TOKEN_IS_NOT(TOKEN_TYPE_ASSIGN))
    {
        return ERR_ID_ASSIGN_EXPECTED;
    }

    return s_expr();
}

int s_id_assign_v()
{
    get_token(&curr_token);
    if (TOKEN_IS(TOKEN_TYPE_OPENING_CURVY_BRACKET))
    {
        return 0;
    }

    if (TOKEN_IS_NOT(TOKEN_TYPE_IDENTIFIER))
    {
        return ERR_ID_ASSIGN_EXPECTED;
    }

    return s_id_assign();
}

int s_id_list_assign()
{
    get_token(&curr_token);
    if (TOKEN_IS_NOT(TOKEN_TYPE_COMMA))
    {
        return ERR_UNEXPECTED_TOKEN;
    }

    int r_code = s_id_list();
    if (r_code)
    {
        return r_code;
    }

    get_token(&curr_token);
    if (TOKEN_IS_NOT(TOKEN_TYPE_ASSIGN))
    {
        return ERR_UNEXPECTED_TOKEN;
    }

    return s_expr_list();
}

int s_param_list()
{
    int r_code = s_expr();
    if (r_code)
    {
        return r_code;
    }

    return s_param_list_n();
}

int s_param_list_n()
{
    get_token(&curr_token);
    if (TOKEN_IS(TOKEN_TYPE_CLOSING_CLASSIC_BRACKET))
    {
        return 0;
    }

    if (TOKEN_IS_NOT(TOKEN_TYPE_COMMA))
    {
        return ERR_UNEXPECTED_TOKEN;
    }

    int r_code = s_expr();
    if (r_code)
    {
        return r_code;
    }

    return s_param_list_n();
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
    return 0;
}

int s_type()
{
    get_token(&curr_token);

    if (TOKEN_IS_NOT(TOKEN_TYPE_RESERVED_KEYWORD))
    {
        return ERR_TYPE_EXPECTED;
    }
    switch (curr_token.keyword)
    {
    case KEYWORD_STRING:
        return 0;
        break;

    case KEYWORD_INTEGER:
        return 0;
        break;

    case KEYWORD_FLOAT64:
        return 0;
        break;

    default:
        return ERR_TYPE_EXPECTED;
        break;
    }
}

// ############################# STATES END ###############################
