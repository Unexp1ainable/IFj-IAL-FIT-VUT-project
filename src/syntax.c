#include "syntax.h"
#include "mocks.h"

// ########################## global variables #########################

TOKEN token_buffer = {TOKEN_TYPE_EMPTY};
TOKEN curr_token;

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
        fputs("Prolog invalid or missing.\n", stderr);
        break;

    default:
        fputs("Something is wrong, I can feel it.\n", stderr);
        break;
    }
}

// ################### end of helper functions #################

// ############################# STATES ##################################

int s_prolog()
{
    get_token(&curr_token); // TODO failure check - possibly implement in get_token?

    if (curr_token.tokentype != TOKEN_TYPE_RESERVED_KEYWORD ||
        curr_token.keyword != KEYWORD_PACKAGE)
    {
        return ERR_PROLOG;
    }

    get_token(&curr_token);
    if (curr_token.tokentype != TOKEN_TYPE_MAIN)
    {
        return ERR_PROLOG;
    }

    get_token(&curr_token);
    if (curr_token.tokentype != TOKEN_TYPE_EOL)
    {
        return ERR_PROLOG;
    }
}

int s_f_list();

int s_func();

int s_f_init();

int s_f_call();

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
