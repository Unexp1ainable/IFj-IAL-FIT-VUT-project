/**
 * @file syntax.c
 * 
 * @brief implementation of syntax analysis
 * 
 * @date 26.11.2020
 * 
 * @copyright Brno University of Technology, Czech Republic
 * 
 * @author Samuel Repka,     xrepka07, +421 907 587 090
 * 
 * @author Michal Reznik,    xrezni28, +420 731 748 122
 * @author Jiri Hofirek,     xhofir06, +420 774 883 191
 * @author Timotej Kamensky  xkamen24, +421 944 687 328
 * 
 * */
#include "syntax.h"

// ########################## global variables #########################
unsigned int assign_list_id_n = 1;
unsigned int assign_list_expr_n = 0;
Symtable_item id_list[MAX_N_RET_VAL];
int id_list_n = 0;
char *curr_func = NULL;

// ############################# helper functions ###########################

bool eol_required()
{
    get_token(&curr_token);
    if (curr_token->tokentype != TOKEN_TYPE_EOL)
    {
        return_token(curr_token);
        return false;
    }

    line++;
    return true;
}

int initialise_predefined(Symtable *table)
{
    // func inputs() (string,int)
    if (!symtable_add_function_init(table, "inputs"))
        return INTERN_ERROR;
    if (!Symtable_add_function_outparam(table, "inputs", T_STRING))
        return INTERN_ERROR;
    if (!Symtable_add_function_outparam(table, "inputs", T_INT))
        return INTERN_ERROR;

    // func inputi() (int,int)
    if (!symtable_add_function_init(table, "inputi"))
        return INTERN_ERROR;
    if (!Symtable_add_function_outparam(table, "inputi", T_INT))
        return INTERN_ERROR;
    if (!Symtable_add_function_outparam(table, "inputi", T_INT))
        return INTERN_ERROR;

    // func inputf() (float64,int)
    if (!symtable_add_function_init(table, "inputf"))
        return INTERN_ERROR;
    if (!Symtable_add_function_outparam(table, "inputf", T_FLOAT))
        return INTERN_ERROR;
    if (!Symtable_add_function_outparam(table, "inputf", T_INT))
        return INTERN_ERROR;

    // func int2float(i int) (float64)
    if (!symtable_add_function_init(table, "int2float"))
        return INTERN_ERROR;
    if (!Symtable_add_function_inparam(table, "int2float", "i", T_INT))
        return INTERN_ERROR;
    if (!Symtable_add_function_outparam(table, "int2float", T_FLOAT))
        return INTERN_ERROR;

    // func float2int(f float64) (int)
    if (!symtable_add_function_init(table, "float2int"))
        return INTERN_ERROR;
    if (!Symtable_add_function_inparam(table, "float2int", "f", T_FLOAT))
        return INTERN_ERROR;
    if (!Symtable_add_function_outparam(table, "float2int", T_INT))
        return INTERN_ERROR;

    // func len(𝑠 string) (int)
    if (!symtable_add_function_init(table, "len"))
        return INTERN_ERROR;
    if (!Symtable_add_function_inparam(table, "len", "s", T_STRING))
        return INTERN_ERROR;
    if (!Symtable_add_function_outparam(table, "len", T_INT))
        return INTERN_ERROR;

    // func substr(s string, i int, n int) (string, int)
    if (!symtable_add_function_init(table, "substr"))
        return INTERN_ERROR;
    if (!Symtable_add_function_inparam(table, "substr", "s", T_STRING))
        return INTERN_ERROR;
    if (!Symtable_add_function_inparam(table, "substr", "i", T_INT))
        return INTERN_ERROR;
    if (!Symtable_add_function_inparam(table, "substr", "n", T_INT))
        return INTERN_ERROR;
    if (!Symtable_add_function_outparam(table, "substr", T_STRING))
        return INTERN_ERROR;
    if (!Symtable_add_function_outparam(table, "substr", T_INT))
        return INTERN_ERROR;

    // func ord(s string, i int) (int, int)
    if (!symtable_add_function_init(table, "ord"))
        return INTERN_ERROR;
    if (!Symtable_add_function_inparam(table, "ord", "s", T_STRING))
        return INTERN_ERROR;
    if (!Symtable_add_function_inparam(table, "ord", "i", T_INT))
        return INTERN_ERROR;
    if (!Symtable_add_function_outparam(table, "ord", T_INT))
        return INTERN_ERROR;
    if (!Symtable_add_function_outparam(table, "ord", T_INT))
        return INTERN_ERROR;

    // func chr(i int) (string, int)
    if (!symtable_add_function_init(table, "chr"))
        return INTERN_ERROR;
    if (!Symtable_add_function_inparam(table, "chr", "i", T_INT))
        return INTERN_ERROR;
    if (!Symtable_add_function_outparam(table, "chr", T_STRING))
        return INTERN_ERROR;
    if (!Symtable_add_function_outparam(table, "chr", T_INT))
        return INTERN_ERROR;

    // print(...)
    if (!symtable_add_function_init(table, "print"))
        return INTERN_ERROR;
    Symtable_set_var_param(table, "print");

    // underscore
    if (!symtable_add_int(table, "_", 0)) // TODO Not sure about the type
        return INTERN_ERROR;

    return 0;
}

int copy_to_id(SymtableStack *symlist)
{
    Symtable_item *func = was_it_defined(symlist, curr_func);
    if (func == NULL || func->dataType != T_FUNC)
    {
        return ERR_SYMTABLE;
    }
    for (int i = 0; i < func->itemData.funcitemptr->used_return; i++)
    {
        id_list[i].dataType = func->itemData.funcitemptr->return_types[i]; // not ideal but whatever
        id_list_n++;
    }

    return 0;
}

// ################### end of helper functions #################

// ############################# STATES ##################################

int s_prolog(SymtableStack *symlist)
{
    s_eols();
    get_token(&curr_token);

    // package
    if (curr_token->tokentype != TOKEN_TYPE_RESERVED_KEYWORD ||
        curr_token->keyword != KEYWORD_PACKAGE)
    {
        return ERR_PROLOG;
    }

    // main
    get_token(&curr_token);
    if (curr_token->tokentype != TOKEN_TYPE_MAIN)
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

    if (first_pass)
        return NO_ERR;

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
    if (curr_token->tokentype != TOKEN_TYPE_EOF)
    {
        return ERR_EOF_EXPECTED;
    }

    return NO_ERR;
} // ---------- end of s_prolog() -------------

int s_f_list(SymtableStack *symlist)
{
    // eof
    get_token(&curr_token);
    return_token(curr_token); // TODO this is weird
    if (curr_token->tokentype == TOKEN_TYPE_EOF)
    {

        return NO_ERR;
    }

    // func
    get_token(&curr_token);
    if (curr_token->tokentype != TOKEN_TYPE_RESERVED_KEYWORD ||
        curr_token->keyword != KEYWORD_FUNC)
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

int s_func(SymtableStack *symlist)
{
    Symtable_item *new_func = NULL;

    //main || id
    get_token(&curr_token);
    if (curr_token->tokentype == TOKEN_TYPE_MAIN)
    {
        curr_func = "main";
        if (first_pass == true)
        {
            if (was_it_defined(symlist, "main"))
            {
                return ERR_FUNC_REDEFINITION;
            }

            new_func = symtable_add_function_init(stackBottom(symlist), curr_func);
            if (!new_func)
            {
                return ERR_SYMTABLE;
            }
        }
    }
    else if (curr_token->tokentype == TOKEN_TYPE_IDENTIFIER)
    {
        curr_func = curr_token->string->string;
        if (first_pass == true)
        {
            if (was_it_defined(symlist, curr_func))
            {
                return ERR_FUNC_REDEFINITION;
            }

            new_func = symtable_add_function_init(stackBottom(symlist), curr_token->string->string);
            if (!new_func)
            {
                return ERR_SYMTABLE;
            }
        }
    }
    else
    {
        return ERR_FUNC_ID_EXPECTED;
    }

    int r_code = s_f_init(symlist, curr_func);
    if (r_code)
    {
        return r_code;
    }

    if (first_pass)
    {
        return NO_ERR;
    }

    new_func = was_it_defined(symlist, curr_func);

    if (new_func)
    {
        r_code = s_body(symlist, new_func->itemData.funcitemptr);
    }
    else
    {
        return ERR_SYMTABLE;
    }

    return r_code;
}

int s_f_init(SymtableStack *symlist, char *func_id)
{
    // (
    get_token(&curr_token);
    if (curr_token->tokentype != TOKEN_TYPE_OPENING_CLASSIC_BRACKET)
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
    if (curr_token->tokentype != TOKEN_TYPE_CLOSING_CLASSIC_BRACKET)
    {
        return ERR_F_PAR_R_BRACKET;
    }

    // ret_t_list
    r_code = s_ret_t_list(symlist, func_id);

    return r_code;
}

int s_f_call(SymtableStack *symlist, Symtable_item *func_def)
{
    // param_def_list
    get_token(&curr_token);
    return_token(curr_token);
    if (TOKEN_IS_NOT(TOKEN_TYPE_OPENING_CLASSIC_BRACKET))
    {
        return ERR_UNEXPECTED_TOKEN;
    }
    return s_param_list(symlist, func_def);
}

int s_body(SymtableStack *symlist, FuncItemData *func_ptr)
{
    Symtable new_table;
    symtable_init(&new_table);
    stackPush(symlist, &new_table);

    // in case the body is body of the function, inject func params to symtable
    if (func_ptr != NULL)
    {
        for (int i = 0; i < func_ptr->used_param; i++)
        {
            sym_list_add_to_last(symlist, func_ptr->param_names[i], func_ptr->param_types[i]);
        }
    }

    // {
    get_token(&curr_token);
    if (curr_token->tokentype != TOKEN_TYPE_OPENING_CURVY_BRACKET)
    {
        symtable_free(stackPop(symlist));
        return ERR_BODY_START;
    }

    // eol
    if (!eol_required())
    {
        symtable_free(stackPop(symlist));
        return ERR_EOL_EXPECTED;
    }

    // <eols>
    s_eols();

    // <stat_list>
    int r_code = s_stat_list(symlist);
    if (r_code)
    {
        symtable_free(stackPop(symlist));
        return r_code;
    }

    // }
    get_token(&curr_token);
    if (curr_token->tokentype != TOKEN_TYPE_CLOSING_CURVY_BRACKET)
    {
        symtable_free(stackPop(symlist));
        return ERR_BODY_START;
    }

    symtable_free(stackPop(symlist));

    return NO_ERR;
}

int s_param_def_list(SymtableStack *symlist, char *func_id)
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
    char *id = curr_token->string->string;

    // <type>
    DataType type;
    int r_code = s_type(&type);
    if (r_code)
    {
        return r_code;
    }

    if (first_pass == true)
    {
        Symtable_add_function_inparam(stackBottom(symlist), func_id, id, type);
    }

    // <param_def_list_n>
    r_code = s_param_def_list_n(symlist, func_id);

    return r_code;
}

int s_param_def_list_n(SymtableStack *symlist, char *func_id)
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
    char *id = curr_token->string->string;

    // <type>
    DataType type;
    int r_code = s_type(&type);
    if (r_code)
    {
        return r_code;
    }

    if (first_pass == true)
    {
        Symtable_add_function_inparam(stackBottom(symlist), func_id, id, type);
    }

    // <param_def_list_n>
    r_code = s_param_def_list_n(symlist, func_id);

    return r_code;
}

int s_ret_t_list(SymtableStack *symlist, char *func_id)
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

    get_token(&curr_token);
    if (TOKEN_IS(TOKEN_TYPE_CLOSING_CLASSIC_BRACKET))
    {
        return NO_ERR;
    }

    return_token(curr_token);

    // <type>
    DataType type;
    int r_code = s_type(&type);
    if (r_code)
    {
        return r_code;
    }

    if (first_pass == true)
    {
        Symtable_add_function_outparam(stackBottom(symlist), func_id, type);
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

int s_ret_t_list_n(SymtableStack *symlist, char *func_id)
{
    //     <ret_t_list_n> -> , <type> <ret_t_list_n>
    // <ret_t_list_n> -> e

    // e
    get_token(&curr_token);
    if (TOKEN_IS(TOKEN_TYPE_CLOSING_CLASSIC_BRACKET))
    {
        return_token(curr_token);
        return NO_ERR;
    }

    if (TOKEN_IS_NOT(TOKEN_TYPE_COMMA))
    {
        return ERR_FUNC_DEF_RET_UNEXPECTED;
    }

    // <type>
    DataType type;
    int r_code = s_type(&type);
    if (r_code)
    {
        return r_code;
    }

    if (first_pass == true)
    {
        Symtable_add_function_outparam(stackBottom(symlist), func_id, type);
    }

    // <ret_t_list_n>
    r_code = s_ret_t_list_n(symlist, func_id);
    if (r_code)
    {
        return r_code;
    }

    return NO_ERR;
}

int s_stat(SymtableStack *symlist)
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
        return s_id_n(symlist, curr_token->string->string);
    }
    if (TOKEN_IS(TOKEN_TYPE_UNDERSCORE))
    {
        return s_id_n(symlist, "_");
    }
    if (TOKEN_IS_NOT(TOKEN_TYPE_RESERVED_KEYWORD))
    {
        return ERR_STAT_UNEXPECTED;
    }

    switch (curr_token->keyword)
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
        int r_code = s_return(symlist);
        if (r_code == ERR_TYPE_MISMATCH)
        {
            return ERR_INVALID_RETURN;
        }
        return r_code;
        break;

    default:
        return ERR_STAT_UNEXPECTED;
        break;
    }
}

int s_stat_list(SymtableStack *symlist)
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

int s_if(SymtableStack *symlist)
{
    //if
    get_token(&curr_token);
    if (TOKEN_IS_NOT(TOKEN_TYPE_RESERVED_KEYWORD) || curr_token->keyword != KEYWORD_IF)
    {
        return ERR_IF_EXPECTED;
    }

    // <expr>
    TermType type;
    int r_code = s_expr(symlist, &type);

    if (r_code)
    {
        if (r_code == ERR_EMPTY_EXP)
            return ERR_NO_EXPR;
        return r_code;
    }

    if (type != T_BOOL)
    {
        return ERR_INVALID_EXPRESSION_IF;
    }


    // <body>
    r_code = s_body(symlist, NULL);
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

int s_else(SymtableStack *symlist)
{
    get_token(&curr_token);
    if (TOKEN_IS(TOKEN_TYPE_RESERVED_KEYWORD) && curr_token->keyword == KEYWORD_ELSE)
    {
        return s_body(symlist, NULL);
    }
    return ERR_MISSING_ELSE;
}

int s_for(SymtableStack *symlist)
{
    // for
    get_token(&curr_token);
    if (TOKEN_IS_NOT(TOKEN_TYPE_RESERVED_KEYWORD) || curr_token->keyword != KEYWORD_FOR)
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
    get_token(&curr_token);
    if (TOKEN_IS_NOT(TOKEN_TYPE_SEMICOLON))
    {
        return ERR_SEMICOLON_EXPECTED;
    }

    // <expr>
    TermType type;
    r_code = s_expr(symlist, &type);

    if (type != T_BOOL)
    {
        return ERR_INVALID_EXPRESSION;
    }

    if (r_code)
    {
        if (r_code == ERR_EMPTY_EXP)
            return ERR_NO_EXPR;
        return r_code;
    }

    // ;
    get_token(&curr_token);
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
    return s_body(symlist, NULL);
}

int s_return(SymtableStack *symlist)
{
    get_token(&curr_token);
    if (TOKEN_IS_NOT(TOKEN_TYPE_RESERVED_KEYWORD) || curr_token->keyword != KEYWORD_RETURN)
    {
        return ERR_RETURN_EXPECTED;
    }

    copy_to_id(symlist);

    return s_expr_list(symlist);
}

int s_expr_list(SymtableStack *symlist)
{
    // e
    get_token(&curr_token);
    return_token(curr_token);

    if (TOKEN_IS(TOKEN_TYPE_EOL))
    {
        id_list_n = 0;
        return NO_ERR;
    }

    // <expr>
    TermType type;
    int r_code = s_expr(symlist, &type);
    if (r_code)
    {
        return r_code;
    }

    if (type != id_list[0].dataType)
    {
        return ERR_TYPE_MISMATCH;
    }

    // <expr_list_n
    return s_expr_list_n(symlist, 1);
}

int s_expr_list_n(SymtableStack *symlist, int n)
{
    get_token(&curr_token);

    // e
    if (TOKEN_IS(TOKEN_TYPE_EOL))
    {
        if (n != id_list_n)
        {
            return ERR_NOT_ENOUGH_RVALUES;
        }
        return_token(curr_token);
        id_list_n = 0;
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

    if (n == id_list_n)
    {
        return ERR_TOO_MANY_RVALUES;
    }

    if (type != id_list[n].dataType)
    {
        return ERR_TYPE_MISMATCH;
    }

    // <expr_list_n>
    return s_expr_list_n(symlist, n + 1);
}

int s_id_n(SymtableStack *symlist, char *id)
{
    get_token(&curr_token);
    return_token(curr_token);
    int r_code;

    Symtable_item *already_defined = was_it_defined(symlist, id);

    switch (curr_token->tokentype)
    {
    // :=
    case TOKEN_TYPE_DEFINE:
        if (was_it_defined_top(symlist, id))
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
        // assignment copy to first value
        memcpy(id_list, already_defined, sizeof(Symtable_item));
        id_list_n++;

        r_code = s_id_assign(symlist);
        id_list_n = 0;
        return r_code;
        break;

    // ,
    case TOKEN_TYPE_COMMA:
        if (!already_defined)
        {
            return ERR_ID_UNDEFINED;
        }
        // assignment copy to first value
        memcpy(id_list, already_defined, sizeof(Symtable_item));
        id_list_n++;

        r_code = s_id_list_assign(symlist);
        id_list_n = 0;
        return r_code;
        break;

    // (
    case TOKEN_TYPE_OPENING_CLASSIC_BRACKET:
        if (!already_defined)
        {
            return ERR_ID_UNDEFINED;
        }
        else if (already_defined->dataType != T_FUNC)
        {
            return ERR_ID_IS_NOT_FUNC;
        }
        return s_f_call(symlist, already_defined);
        break;

    default:
        return ERR_UNKNOWN_ID_OPERATION;
        break;
    }
}

int s_id_def(SymtableStack *symlist, char *id)
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
        if (r_code == ERR_EMPTY_EXP)
            return ERR_NO_EXPR;
        return r_code;
    }

    if (type == T_STRING || type == T_INT || type == T_FLOAT)
    {
        sym_list_add_to_last(symlist, id, type);
        return r_code;
    }
    return ERR_TYPE_UNDETERMINED;
}

int s_id_def_v(SymtableStack *symlist)
{
    get_token(&curr_token);
    if (TOKEN_IS(TOKEN_TYPE_IDENTIFIER))
    {
        char *id = curr_token->string->string;
        return s_id_def(symlist, id);
    }

    return_token(curr_token);
    return NO_ERR;
}

int s_id_list(SymtableStack *symlist)
{
    get_token(&curr_token);
    Symtable_item *sym_item;
    if (TOKEN_IS_NOT(TOKEN_TYPE_IDENTIFIER))
    {
        if (TOKEN_IS_NOT(TOKEN_TYPE_UNDERSCORE))
        {
            return ERR_ID_EXPECTED;
        }
        else
        {
            sym_item = was_it_defined(symlist, "_");
            if (!sym_item)
            {
                return ERR_ID_UNDEFINED;
            }
        }
    }
    else
    {
        sym_item = was_it_defined(symlist, curr_token->string->string);
        if (!sym_item)
        {
            return ERR_ID_UNDEFINED;
        }
    }

    memcpy(id_list + 1, sym_item, sizeof(Symtable_item));
    id_list_n++;

    return s_id_list_n(symlist);
}

int s_id_list_n(SymtableStack *symlist)
{
    get_token(&curr_token);
    if (TOKEN_IS(TOKEN_TYPE_ASSIGN))
    {
        return_token(curr_token);
        return NO_ERR;
    }

    if (TOKEN_IS(TOKEN_TYPE_COMMA))
    {
        Symtable_item *sym_item;

        get_token(&curr_token);
        if (TOKEN_IS_NOT(TOKEN_TYPE_IDENTIFIER))
        {
            if (TOKEN_IS_NOT(TOKEN_TYPE_UNDERSCORE))
            {
                return ERR_ID_EXPECTED;
            }
            else
            {
                sym_item = was_it_defined(symlist, "_");
                if (!sym_item)
                {
                    return ERR_ID_UNDEFINED;
                }
            }
        }
        else
        {
            sym_item = was_it_defined(symlist, curr_token->string->string);
            if (!sym_item)
            {
                return ERR_ID_UNDEFINED;
            }
        }

        memcpy(id_list + id_list_n, sym_item, sizeof(Symtable_item));
        id_list_n++;

        return NO_ERR;
    }

    return ERR_UNEXPECTED_TOKEN;
}

int s_id_assign(SymtableStack *symlist)
{
    get_token(&curr_token);
    if (TOKEN_IS_NOT(TOKEN_TYPE_ASSIGN))
    {
        return ERR_ID_ASSIGN_EXPECTED;
    }
    TermType type;

    // check if next token is func id
    get_token(&curr_token);
    if (curr_token->tokentype != TOKEN_TYPE_IDENTIFIER)
    {
        return_token(curr_token);
        int r_code = s_expr(symlist, &type);
        if (r_code == ERR_EMPTY_EXP)
            return ERR_NO_EXPR;
        return r_code;
    }

    Symtable_item *item = was_it_defined(symlist, curr_token->string->string);

    if (item == NULL)
    {
        return ERR_ID_UNDEFINED;
    }
    else if (item->dataType == T_FUNC)
    {

        int r_code = s_func_assign(item);
        if (r_code)
        {
            return r_code;
        }
        return s_f_call(symlist, item);
    }

    return_token(curr_token);
    return s_expr(symlist, &type);
}

int s_id_assign_v(SymtableStack *symlist)
{
    get_token(&curr_token);
    if (TOKEN_IS(TOKEN_TYPE_OPENING_CURVY_BRACKET))
    {
        return_token(curr_token);
        return NO_ERR;
    }

    if (TOKEN_IS_NOT(TOKEN_TYPE_IDENTIFIER))
    {
        return ERR_ID_ASSIGN_EXPECTED;
    }

    return s_id_assign(symlist);
}

int s_id_list_assign(SymtableStack *symlist)
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

    // check if next token is func id
    get_token(&curr_token);
    if (curr_token->tokentype != TOKEN_TYPE_IDENTIFIER)
    {
        return_token(curr_token);
        // TODO check validity and number of the asignees
        return s_expr_list(symlist);
    }
    // it is identifier, but we don`t know if it is a function
    Symtable_item *item = was_it_defined(symlist, curr_token->string->string);

    // it does not exist
    if (item == NULL)
    {
        return ERR_ID_UNDEFINED;
    }
    // it is a function
    else if (item->dataType == T_FUNC)
    {
        r_code = s_func_assign(item);
        if (r_code)
        {
            return r_code;
        }
        return s_f_call(symlist, item);
    }
    // it is not a function
    else
    {
        return_token(curr_token);
        return s_expr_list(symlist);
    }

    return_token(curr_token);
    return expr_list_assign(symlist);
}

int s_param_list(SymtableStack *symlist, Symtable_item *func_def)
{
    get_token(&curr_token);
    if (TOKEN_IS_NOT(TOKEN_TYPE_OPENING_CLASSIC_BRACKET))
    {
        return ERR_UNEXPECTED_TOKEN;
    }

    get_token(&curr_token);
    if (TOKEN_IS(TOKEN_TYPE_CLOSING_CLASSIC_BRACKET))
    {
        return NO_ERR;
    }
    return_token(curr_token);

    TermType type;
    int r_code = s_expr(symlist, &type);
    if (r_code)
    {
        return r_code;
    }

    if (!func_def->itemData.funcitemptr->var_param)
    {
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
    }

    return s_param_list_n(symlist, func_def, 1);
}

int s_param_list_n(SymtableStack *symlist, Symtable_item *func_def, int n)
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
    if (!func_def->itemData.funcitemptr->var_param)
    {
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
    }
    return s_param_list_n(symlist, func_def, n);
}

int s_type(DataType *type)
{
    get_token(&curr_token);

    if (TOKEN_IS_NOT(TOKEN_TYPE_RESERVED_KEYWORD))
    {
        return ERR_TYPE_EXPECTED;
    }
    switch (curr_token->keyword)
    {
    case KEYWORD_STRING:
        *type = T_STRING;
        return NO_ERR;
        break;

    case KEYWORD_INTEGER:
        *type = T_INT;
        return NO_ERR;
        break;

    case KEYWORD_FLOAT64:
        *type = T_FLOAT;
        return NO_ERR;
        break;

    default:
        *type = T_UNKNOWN;
        return ERR_TYPE_EXPECTED;
        break;
    }
}

int s_func_assign(Symtable_item *func_def)
{
    if (id_list_n != func_def->itemData.funcitemptr->used_return)
    {
        return ERR_WRONG_NUMBER_LVALUES;
    }

    for (int i = 0; i < id_list_n; i++)
    {
        //TODO check if underscore
        // if (id_list[i].dataType == TOKEN_TYPE_UNDERSCORE)
        // {
        //     continue;
        // }

        // check type
        if (strcmp(id_list[i].key, "_") == 0)
        {
            continue;
        }
        if (func_def->itemData.funcitemptr->return_types[i] != id_list[i].dataType)
        {
            return ERR_TYPE_MISMATCH;
        }
    }

    return NO_ERR;
}

int expr_list_assign(SymtableStack *symlist)
{

    get_token(&curr_token);

    TermType type;
    int r_code;

    // <expr>
    r_code = s_expr(symlist, &type);
    if (r_code)
    {
        return r_code;
    }
    // check type
    if (id_list_n > 0)
    {
        if (id_list->dataType == type)
        {

            return expr_list_assign_n(symlist, 1);
        }
    }
    return ERR_TYPE_MISMATCH;
}
int expr_list_assign_n(SymtableStack *symlist, int n)
{
    if (n == id_list_n)
    {
        return NO_ERR;
    }

    get_token(&curr_token);
    if (TOKEN_IS_NOT(TOKEN_TYPE_COMMA))
    {
        return ERR_COMMA_EXPECTED;
    }

    TermType type;
    int r_code;

    // <expr>
    r_code = s_expr(symlist, &type);
    if (r_code)
    {
        return r_code;
    }
    // check type
    if (id_list->dataType == type)
    {

        return expr_list_assign_n(symlist, n + 1);
    }

    return ERR_TYPE_MISMATCH;
}

// ############################# STATES END ###############################
