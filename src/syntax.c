#include "syntax.h"
#include "newscanner.h"
#include "expression.h"
#include "symtable_list.h"

// ########################## global variables #########################

TOKEN token_buffer = {TOKEN_TYPE_EMPTY, {NULL}};
TOKEN curr_token;
unsigned long int line = 1;
bool main_defined = false;
unsigned int assign_list_id_n = 1;
unsigned int assign_list_expr_n = 0;
bool first_pass = true;

// ############################# helper functions ###########################

int get_token(TOKEN *token)
{
    if (token_buffer.tokentype == TOKEN_TYPE_EMPTY)
    {
        int r_code = get_next_token(token);
        if (!r_code)
        {
            return NO_ERR;
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
        return NO_ERR;
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

Symtable_item *was_it_defined(symtableList list, char *key)
{

    symListItemPtr curr_list_item = list;
    Symtable_item *symbol = NULL;

    if (list == NULL)
    {
        return false;
    }

    while (curr_list_item != NULL)
    {
        symbol = symtable_search(curr_list_item->table, key);

        if (symbol != NULL)
        {
            return symbol;
        }

        curr_list_item = curr_list_item->next;
    }
    return symbol;
}

int initialise_predefined(Symtable *table)
{
    // func inputs() (string,int)
    if (!symtable_add_function_init(table, "inputs"))
        return INTERN_ERROR;
    if (!Symtable_add_function_outparam(table, "inputs", "string"))
        return INTERN_ERROR;
    if (!Symtable_add_function_outparam(table, "inputs", "int"))
        return INTERN_ERROR;

    // func inputi() (int,int)
    if (!symtable_add_function_init(table, "inputi"))
        return INTERN_ERROR;
    if (!Symtable_add_function_outparam(table, "inputi", "string"))
        return INTERN_ERROR;
    if (!Symtable_add_function_outparam(table, "inputi", "int"))
        return INTERN_ERROR;

    // func inputf() (float64,int)
    if (!symtable_add_function_init(table, "inputf"))
        return INTERN_ERROR;
    if (!Symtable_add_function_outparam(table, "inputf", "float64"))
        return INTERN_ERROR;
    if (!Symtable_add_function_outparam(table, "inputf", "int"))
        return INTERN_ERROR;

    // func int2float(i int) (float64)
    if (!symtable_add_function_init(table, "int2float"))
        return INTERN_ERROR;
    if (!Symtable_add_function_inparam(table, "int2float", "i", "int"))
        return INTERN_ERROR;
    if (!Symtable_add_function_outparam(table, "int2float", "float64"))
        return INTERN_ERROR;

    // func float2int(f float64) (int)
    if (!symtable_add_function_init(table, "float2int"))
        return INTERN_ERROR;
    if (!Symtable_add_function_inparam(table, "float2int", "f", "float64"))
        return INTERN_ERROR;
    if (!Symtable_add_function_outparam(table, "float2int", "int"))
        return INTERN_ERROR;

    // func len(𝑠 string) (int)
    if (!symtable_add_function_init(table, "len"))
        return INTERN_ERROR;
    if (!Symtable_add_function_inparam(table, "len", "s", "string"))
        return INTERN_ERROR;
    if (!Symtable_add_function_outparam(table, "len", "int"))
        return INTERN_ERROR;

    // func substr(s string, i int, n int) (string, int)
    if (!symtable_add_function_init(table, "substr"))
        return INTERN_ERROR;
    if (!Symtable_add_function_inparam(table, "substr", "s", "string"))
        return INTERN_ERROR;
    if (!Symtable_add_function_inparam(table, "substr", "i", "int"))
        return INTERN_ERROR;
    if (!Symtable_add_function_inparam(table, "substr", "n", "int"))
        return INTERN_ERROR;
    if (!Symtable_add_function_outparam(table, "substr", "string"))
        return INTERN_ERROR;
    if (!Symtable_add_function_outparam(table, "substr", "int"))
        return INTERN_ERROR;

    // func ord(s string, i int) (int, int)
    if (!symtable_add_function_init(table, "ord"))
        return INTERN_ERROR;
    if (!Symtable_add_function_inparam(table, "ord", "s", "string"))
        return INTERN_ERROR;
    if (!Symtable_add_function_inparam(table, "ord", "i", "int"))
        return INTERN_ERROR;
    if (!Symtable_add_function_outparam(table, "ord", "int"))
        return INTERN_ERROR;
    if (!Symtable_add_function_outparam(table, "ord", "int"))
        return INTERN_ERROR;

    // func chr(i int) (string, int)
    if (!symtable_add_function_init(table, "chr"))
        return INTERN_ERROR;
    if (!Symtable_add_function_inparam(table, "chr", "i", "int"))
        return INTERN_ERROR;
    if (!Symtable_add_function_outparam(table, "chr", "string"))
        return INTERN_ERROR;
    if (!Symtable_add_function_outparam(table, "chr", "int"))
        return INTERN_ERROR;

    // print(...)
    if (!symtable_add_function_init(table, "print"))
        return INTERN_ERROR;
    Symtable_set_var_param(table, "print");

    // underscore
    if (!symtable_add_int(table, "_", 0))
        return INTERN_ERROR;

    return 0;
}

// ################### end of helper functions #################

// ############################# STATES ##################################

int s_prolog(symtableList symlist)
{
    s_eols();
    get_token(&curr_token);

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
    int r_code = s_f_list(symlist);
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

    return NO_ERR;
} // ---------- end of s_prolog() -------------

int s_f_list(symtableList symlist)
{
    // eof
    get_token(&curr_token);
    return_token(curr_token);
    if (curr_token.tokentype == TOKEN_TYPE_EOF)
    {

        return NO_ERR;
    }

    // func
    get_token(&curr_token);
    if (curr_token.tokentype != TOKEN_TYPE_RESERVED_KEYWORD ||
        curr_token.keyword != KEYWORD_FUNC)
    {
        return ERR_FUNC_EXPECTED;
    }

    // <func>
    int r_code = s_func(symlist);
    if (r_code)
    {
        return r_code;
    }

    // <eols>
    s_eols();

    // f_list
    return s_f_list(symlist);
}

int s_func(symtableList symlist)
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

            if (first_pass == true)
            {
                symtable_add_function_init(symlist->table, "main");
            }
        }
    }
    else if (curr_token.tokentype == TOKEN_TYPE_IDENTIFIER)
    {
        if (first_pass == true)
        {
            symtable_add_function_init(symlist->table, curr_token.string->string);
        }
    }
    else
    {
        return ERR_FUNC_ID_EXPECTED;
    }

    int r_code = s_f_init(symlist, curr_token.string->string);
    if (r_code)
    {
        return r_code;
    }

    if (first_pass)
    {
        return NO_ERR;
    }

    r_code = s_body(symlist);

    return r_code;
}

int s_f_init(symtableList symlist, char *func_id)
{
    // (
    get_token(&curr_token);
    if (curr_token.tokentype != TOKEN_TYPE_OPENING_CLASSIC_BRACKET)
    {
        return ERR_F_PAR_L_BRACKET;
    }

    // param_def_list
    int r_code = s_param_def_list(symlist, func_id);
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
    r_code = s_ret_t_list(symlist, func_id);

    return r_code;
}

int s_f_call(symtableList symlist, Symtable_item *func_def)
{
    // param_def_list
    get_token(&curr_token);
    if (TOKEN_IS_NOT(TOKEN_TYPE_OPENING_CLASSIC_BRACKET))
    {
        return ERR_UNEXPECTED_TOKEN;
    }
    return s_param_list(symlist, func_def);
}

int s_body(symtableList symlist)
{
    Symtable new_table;
    symtable_init(&new_table);
    sym_list_add(&symlist, &new_table);

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
    int r_code = s_stat_list(symlist);
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

    sym_list_remove_last(&symlist);

    // eol
    if (!eol_required())
    {
        return ERR_EOL_EXPECTED;
    }

    // <eols>
    s_eols();

    return NO_ERR;
}

int s_param_def_list(symtableList symlist, char *func_id)
{
    // e
    get_token(&curr_token);
    if (TOKEN_IS(TOKEN_TYPE_CLOSING_CLASSIC_BRACKET))
    {
        return_token(curr_token);
        return NO_ERR;
    }

    // id
    if (TOKEN_IS_NOT(TOKEN_TYPE_IDENTIFIER))
    {
        return ERR_FUNC_DEF_UNEXPECTED;
    }
    char *id = curr_token.string->string;

    // <type>
    char *type;
    int r_code = s_type( &type);
    if (r_code)
    {
        return r_code;
    }

    if (first_pass == true)
    {
        Symtable_add_function_inparam(symlist->table, func_id, id, type);
    }

    // <param_def_list_n>
    r_code = s_param_def_list_n(symlist, func_id);

    return r_code;
}

int s_param_def_list_n(symtableList symlist, char *func_id)
{
    // e
    get_token(&curr_token);
    if (TOKEN_IS(TOKEN_TYPE_CLOSING_CLASSIC_BRACKET))
    {
        return_token(curr_token);
        return NO_ERR;
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
    char *id = curr_token.string->string;

    // <type>
    char *type;
    int r_code = s_type( &type);
    if (r_code)
    {
        return r_code;
    }

    if (first_pass == true)
    {
        Symtable_add_function_inparam(symlist->table, func_id, id, type);
    }

    // <param_def_list_n>
    r_code = s_param_def_list_n(symlist, func_id);

    return r_code;
}

int s_ret_t_list(symtableList symlist, char *func_id)
{
    // e
    get_token(&curr_token);
    if (TOKEN_IS(TOKEN_TYPE_OPENING_CURVY_BRACKET))
    {
        return_token(curr_token);
        return NO_ERR;
    }

    // (
    if (TOKEN_IS_NOT(TOKEN_TYPE_OPENING_CLASSIC_BRACKET))
    {
        return ERR_FUNC_DEF_RET_UNEXPECTED;
    }

    // <type>
    char *type;
    int r_code = s_type( &type);
    if (r_code)
    {
        return r_code;
    }

    if (first_pass == true)
    {
        Symtable_add_function_outparam(symlist->table, func_id, type);
    }

    // <ret_t_list_n>
    r_code = s_ret_t_list_n(symlist, func_id);
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

    return NO_ERR;
}

int s_ret_t_list_n(symtableList symlist, char *func_id)
{
    //     <ret_t_list_n> -> , <type> <ret_t_list_n>
    // <ret_t_list_n> -> e

    // e
    get_token(&curr_token);
    if (TOKEN_IS_NOT(TOKEN_TYPE_OPENING_CLASSIC_BRACKET))
    {
        return_token(curr_token);
        return NO_ERR;
    }

    if (TOKEN_IS_NOT(TOKEN_TYPE_COMMA))
    {
        return ERR_FUNC_DEF_RET_UNEXPECTED;
    }

    // <type>
    char *type;
    int r_code = s_type( &type);
    if (r_code)
    {
        return r_code;
    }

    if (first_pass == true)
    {
        Symtable_add_function_outparam(symlist->table, func_id, type);
    }

    // <ret_t_list_n>
    r_code = s_ret_t_list_n(symlist, func_id);
    if (r_code)
    {
        return r_code;
    }

    return NO_ERR;
}

int s_stat(symtableList symlist)
{
    get_token(&curr_token);
    // <stat> -> e
    if (TOKEN_IS(TOKEN_TYPE_CLOSING_CURVY_BRACKET))
    {
        return_token(curr_token);
        return NO_ERR;
    }
    // <stat> -> id <id_n>
    if (TOKEN_IS(TOKEN_TYPE_IDENTIFIER))
    {
        return s_id_n(symlist, curr_token.string->string);
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
        return s_if(symlist);
        break;

    // <stat> -> <for>
    case KEYWORD_FOR:
        return_token(curr_token);
        return s_for(symlist);
        break;

    // <stat> -> <return>
    case KEYWORD_RETURN:
        return_token(curr_token);
        return s_return(symlist);
        break;

    default:
        return ERR_STAT_UNEXPECTED;
        break;
    }
}

int s_stat_list(symtableList symlist)
{
    // e
    get_token(&curr_token);
    return_token(curr_token);
    if (TOKEN_IS(TOKEN_TYPE_CLOSING_CURVY_BRACKET))
    {
        return NO_ERR;
    }

    // <stat>
    int r_code = s_stat(symlist);
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
    r_code = s_stat_list(symlist);

    return r_code;
}

int s_if(symtableList symlist)
{
    //if
    get_token(&curr_token);
    if (TOKEN_IS_NOT(TOKEN_TYPE_RESERVED_KEYWORD) || curr_token.keyword != KEYWORD_IF)
    {
        return ERR_IF_EXPECTED;
    }

    // <expr>
    TermType type;
    int r_code = s_expr(symlist, &type);
    if (r_code)
    {
        return r_code;
    }

    // <body>
    r_code = s_body(symlist);
    if (r_code)
    {
        return r_code;
    }

    // <else>
    r_code = s_else(symlist);
    if (r_code)
    {
        return r_code;
    }

    return NO_ERR;
}

int s_else(symtableList symlist)
{
    get_token(&curr_token);
    if (TOKEN_IS(TOKEN_TYPE_RESERVED_KEYWORD) && curr_token.keyword == KEYWORD_ELSE)
    {
        return s_body(symlist);
    }
    return_token(curr_token);
    return NO_ERR;
}

int s_for(symtableList symlist)
{
    // for
    get_token(&curr_token);
    if (TOKEN_IS_NOT(TOKEN_TYPE_RESERVED_KEYWORD) || curr_token.keyword != KEYWORD_FOR)
    {
        return ERR_FOR_EXPECTED;
    }

    // <id_def_v>
    int r_code = s_id_def_v(symlist);
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
    TermType type;
    r_code = s_expr(symlist, &type);
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
    r_code = s_id_assign_v(symlist);
    if (r_code)
    {
        return r_code;
    }

    // <body>
    return s_body(symlist);
}

int s_return(symtableList symlist)
{
    get_token(&curr_token);
    if (TOKEN_IS_NOT(TOKEN_TYPE_RESERVED_KEYWORD) || curr_token.keyword != KEYWORD_RETURN)
    {
        return ERR_RETURN_EXPECTED;
    }

    return s_expr_list(symlist);
}

int s_expr_list(symtableList symlist)
{
    // e
    get_token(&curr_token);
    return_token(curr_token);

    if (TOKEN_IS(TOKEN_TYPE_EOL))
    {
        return NO_ERR;
    }

    // <expr>
    TermType type;
    int r_code = s_expr(symlist, &type);
    if (r_code)
    {
        return r_code;
    }

    // <expr_list_n
    return s_expr_list_n(symlist);
}

int s_expr_list_n(symtableList symlist)
{
    get_token(&curr_token);

    // e
    if (TOKEN_IS(TOKEN_TYPE_EOL))
    {
        return_token(curr_token);
        return NO_ERR;
    }

    // ,
    if (TOKEN_IS_NOT(TOKEN_TYPE_COMMA))
    {
        return ERR_COMMA_EXPECTED;
    }

    // <expr>
    TermType type;
    int r_code = s_expr(symlist, &type);
    if (r_code)
    {
        return r_code;
    }

    // <expr_list_n>
    return s_expr_list_n(symlist);
}

int s_id_n(symtableList symlist, char *id)
{
    get_token(&curr_token);
    return_token(curr_token);

    Symtable_item *already_defined = was_it_defined(symlist, id);

    switch (curr_token.tokentype)
    {
    // :=
    case TOKEN_TYPE_DEFINE:
        if (already_defined)
        {
            return ERR_ID_REDEFINITION;
        }
        return s_id_def(symlist, id);
        break;

    // =
    case TOKEN_TYPE_ASSIGN:
        if (!already_defined)
        {
            return ERR_ID_UNDEFINED;
        }
        return s_id_assign(symlist);
        break;

    // ,
    case TOKEN_TYPE_COMMA:
        if (!already_defined)
        {
            return ERR_ID_UNDEFINED;
        }
        return s_id_list_assign(symlist);
        break;

    // (
    case TOKEN_TYPE_OPENING_CLASSIC_BRACKET:
        if (!already_defined)
        {
            return ERR_ID_UNDEFINED;
        }
        return s_f_call(symlist, already_defined);
        break;

    default:
        return ERR_UNKNOWN_ID_OPERATION;
        break;
    }
}

int s_id_def(symtableList symlist, char *id)
{
    get_token(&curr_token);
    if (TOKEN_IS_NOT(TOKEN_TYPE_DEFINE))
    {
        return ERR_ID_DEF_EXPECTED;
    }

    TermType type;
    int r_code = s_expr(symlist, &type);

    if (r_code)
    {
        return r_code;
    }
    sym_list_add_to_last(symlist, id, type);

    return r_code;
}

int s_id_def_v(symtableList symlist)
{
    get_token(&curr_token);
    if (TOKEN_IS(TOKEN_TYPE_IDENTIFIER))
    {
        get_token(&curr_token);
        return s_id_def(symlist, curr_token.string->string);
    }

    return_token(curr_token);
    return NO_ERR;
}

int s_id_list(symtableList symlist)
{
    get_token(&curr_token);
    if (TOKEN_IS_NOT(TOKEN_TYPE_IDENTIFIER) && TOKEN_IS_NOT(TOKEN_TYPE_UNDERSCORE))
    {
        return ERR_ID_EXPECTED;
    }

    return s_id_list_n(symlist);
}

int s_id_list_n(symtableList symlist)
{
    get_token(&curr_token);
    if (TOKEN_IS(TOKEN_TYPE_ASSIGN))
    {
        return_token(curr_token);
        return NO_ERR;
    }

    if (TOKEN_IS(TOKEN_TYPE_COMMA))
    {
        assign_list_id_n++;

        get_token(&curr_token);
        if (TOKEN_IS(TOKEN_TYPE_IDENTIFIER))
        {
            return s_id_list_n(symlist);
        }

        return ERR_ID_EXPECTED;
    }

    return ERR_UNEXPECTED_TOKEN;
}

int s_id_assign(symtableList symlist)
{
    get_token(&curr_token);
    if (TOKEN_IS_NOT(TOKEN_TYPE_ASSIGN))
    {
        return ERR_ID_ASSIGN_EXPECTED;
    }
    TermType type;
    return s_expr(symlist, &type);
}

int s_id_assign_v(symtableList symlist)
{
    get_token(&curr_token);
    if (TOKEN_IS(TOKEN_TYPE_OPENING_CURVY_BRACKET))
    {
        return NO_ERR;
    }

    if (TOKEN_IS_NOT(TOKEN_TYPE_IDENTIFIER))
    {
        return ERR_ID_ASSIGN_EXPECTED;
    }

    return s_id_assign(symlist);
}

int s_id_list_assign(symtableList symlist)
{
    get_token(&curr_token);
    if (TOKEN_IS_NOT(TOKEN_TYPE_COMMA))
    {
        return ERR_UNEXPECTED_TOKEN;
    }

    int r_code = s_id_list(symlist);
    if (r_code)
    {
        return r_code;
    }

    get_token(&curr_token);
    if (TOKEN_IS_NOT(TOKEN_TYPE_ASSIGN))
    {
        return ERR_UNEXPECTED_TOKEN;
    }

    return s_expr_list(symlist);
}

int s_param_list(symtableList symlist, Symtable_item *func_def)
{
    TermType type;
    int r_code = s_expr(symlist, &type);
    if (r_code)
    {
        return r_code;
    }

    if (func_def->itemData.funcitemptr->used_param == 0)
    {
        if (type != T_UNKNOWN)
        {
            return ERR_WRONG_FUNC_PARAM;
        }
    }
    else if (type != func_def->itemData.funcitemptr->param_types[0])
    {
        return ERR_WRONG_FUNC_PARAM;
    }

    return s_param_list_n(symlist, func_def, 1);
}

int s_param_list_n(symtableList symlist, Symtable_item *func_def, unsigned int n)
{
    get_token(&curr_token);
    if (TOKEN_IS(TOKEN_TYPE_CLOSING_CLASSIC_BRACKET))
    {
        return NO_ERR;
    }

    if (TOKEN_IS_NOT(TOKEN_TYPE_COMMA))
    {
        return ERR_UNEXPECTED_TOKEN;
    }

    TermType type;
    int r_code = s_expr(symlist, &type);
    if (r_code)
    {
        return r_code;
    }

    if (func_def->itemData.funcitemptr->used_param <= n)
    {
        if (type != T_UNKNOWN)
        {
            return ERR_WRONG_FUNC_PARAM;
        }
    }
    else if (type != func_def->itemData.funcitemptr->param_types[n])
    {
        return ERR_WRONG_FUNC_PARAM;
    }

    return s_param_list_n(symlist, func_def, n);
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

int s_type(char **type)
{
    get_token(&curr_token);

    if (TOKEN_IS_NOT(TOKEN_TYPE_RESERVED_KEYWORD))
    {
        return ERR_TYPE_EXPECTED;
    }
    switch (curr_token.keyword)
    {
    case KEYWORD_STRING:
        return NO_ERR;
        break;

    case KEYWORD_INTEGER:
        return NO_ERR;
        break;

    case KEYWORD_FLOAT64:
        return NO_ERR;
        break;

    default:
        return ERR_TYPE_EXPECTED;
        break;
    }
}

// ############################# STATES END ###############################
