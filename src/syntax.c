#include "syntax.h"
#include "mocks.h"

// ########################## global variables #########################

TOKEN token_buffer = {TOKEN_TYPE_EMPTY};
TOKEN curr_token;
unsigned long int line = 0;
main_defined = false;

// ############################# helper functions ###########################

int get_token(TOKEN *token)
{
    if (token_buffer.tokentype == TOKEN_TYPE_EMPTY)
    {
        return get_next_tokenm(token); //TODO mock function is here
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

    // possible eols
    s_eols();

    // f_list
    int r_code = s_f_list();
    if (r_code)
    {
        return r_code;
    }

    // possible eols
    s_eols();

    // eof
    get_token(&curr_token);
    if (curr_token.tokentype != TOKEN_TYPE_EOF)
    {
        return ERR_EOF_EXPECTED;
    }
} // ---------- end of s_prolog() -------------

int s_f_list()
{
    // eols
    s_eols();

    // eof
    get_token(&curr_token);
    if (curr_token.tokentype == TOKEN_TYPE_EOF)
    {
        return_token(curr_token);
        return 0;
    }
    return_token(curr_token);

    // func
    int r_code = s_func();
    if (!r_code)
    {
        return r_code;
    }

    // f_list
    int r_code = s_f_list();
    if (!r_code)
    {
        return r_code;
    }
}

int s_func()
{
    //func
    get_token(&curr_token);
    if (curr_token.tokentype != TOKEN_TYPE_RESERVED_KEYWORD ||
        curr_token.keyword != KEYWORD_FUNC)
    {
        return ERR_FUNC_EXPECTED;
    }

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
            main_defined == true;
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
    if (!r_code)
    {
        return r_code;
    }

    r_code = s_body();
    if (!r_code)
    {
        return r_code;
    }
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
    if (!r_code)
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
    int r_code = s_ret_t_list();
    if (!r_code)
    {
        return r_code;
    }
}

int s_f_call(){
    get_token(&curr_token);
    if (curr_token.tokentype == TOKEN_TYPE_IDENTIFIER)
    {
        // TODO symtable work
    }
    else
    {
        return ERR_FUNC_ID_EXPECTED;
    }

    // (
    get_token(&curr_token);
    if (curr_token.tokentype != TOKEN_TYPE_OPENING_CLASSIC_BRACKET)
    {
        return ERR_F_PAR_L_BRACKET;
    }

    // param_def_list
    int r_code = s_param_def_list();
    if (!r_code)
    {
        return r_code;
    }

    // )
    get_token(&curr_token);
    if (curr_token.tokentype != TOKEN_TYPE_CLOSING_CLASSIC_BRACKET)
    {
        return ERR_F_PAR_R_BRACKET;
    }

}

int s_body();

int s_param_def_list();

int s_param_def_list_n();

int s_ret_t_list();

int s_ret_t_list_n();

int s_stat();

int s_stat_list();

int s_if();

int s_else();

int s_for();

int s_return();

int s_expr_list();

int s_expr_list_n();

int s_id_n();

int s_id_def();

int s_id_list();

int s_id_list_n();

int s_id_assign();

int s_param();

int s_param_list();

int s_param_list_n();

int s_eols();

// ############################# STATES END ###############################
