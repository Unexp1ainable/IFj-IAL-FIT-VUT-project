/**
 * @brief file syntax.h
 * author Samuel Repka
 * Header file for syntax checker
 * version 0.1
 * date 2020-11-26
 * 
 */
#ifndef SYNTAX_H
#define SYNTAX_H
// ---------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdbool.h>
#include "newscanner.h"
// ########################## global variables #########################
/**
 * @brief Buffer for returned token
 * 
 */
extern TOKEN token_buffer;

/**
 * @brief Token being processed
 * 
 */
extern TOKEN curr_token;

/**
 * @brief Line number
 * 
 */
extern unsigned long line;

/**
 * @brief For checking if main was already defined.
 *
 */

extern bool main_defined;

/**
 * @brief Used for counting how many items are on the left of the list assignment
 * Initialised on 1 because id needs to pass 1 item before entering required state.
 */
extern unsigned int assign_list_id_n;

/**
 * @brief Used for counting how many items are on the right of the list assignment
 * 
 */
extern unsigned int assign_list_expr_n;
// ################### end of global variables #################

// ################### error codes #################

/**
 * @brief Error codes
 * 
 */
typedef enum
{
    ERR_PROLOG,       // prolog wrong or missing
    ERR_EOL_EXPECTED, // required eol missing
    ERR_EOF_EXPECTED,
    ERR_FUNC_EXPECTED,
    ERR_FUNC_ID_EXPECTED,
    ERR_MULTIPLE_MAIN_FUNC,
    ERR_F_PAR_L_BRACKET,
    ERR_F_PAR_R_BRACKET,
    ERR_BODY_START,
    ERR_BODY_END,
    ERR_FUNC_DEF_UNEXPECTED,
    ERR_FUNC_DEF_RET_UNEXPECTED,
    ERR_STAT_UNEXPECTED,
    ERR_IF_EXPECTED,
    ERR_FOR_EXPECTED,
    ERR_SEMICOLON_EXPECTED,
    ERR_RETURN_EXPECTED,
    ERR_COMMA_EXPECTED,
    ERR_UNKNOWN_ID_OPERATION,
    ERR_ID_DEF_EXPECTED,
    ERR_ID_EXPECTED,
    ERR_UNEXPECTED_TOKEN,
    ERR_ID_ASSIGN_EXPECTED,
    ERR_TYPE_EXPECTED

} ERR_CODE_SYN;
// ################### end of error codes #################

// ############################# helper functions ###########################
/**
 * @brief Get the token object
 * 
 * param token Pointer to output token
 * @return int Return code. 0 if good
 */
int get_token(TOKEN *token);

/**
 * @briefPut token to temp holding space
 *
 * param token Token
 */
void return_token(TOKEN token);

/**
 * @briefPrint error description on stderr  
 * 
 * 
 * param err Error code
 */
void describe_error(ERR_CODE_SYN err);

/**
 * @brief Check if next token is eol and increment line
 * 
 */
bool eol_required();

/**
 * @brief Check if id is already in symtable
 * 
 * @param list List of symtables
 * @param key_t Id which to look for
 * 
 * @return true If it is in symtable
 * @return false If it is not in symtable
 */
bool was_it_defined(symtableList list, char* key);

// ################### end of helper functions #################

// ################### macros #################

/**
 * @brief Condition for checking tokentype, ==
 * 
 */
#define TOKEN_IS(tt) (curr_token.tokentype == tt)

/**
 * @brief Condition for checking tokentype, !=
 * 
 */
#define TOKEN_IS_NOT(tt) (curr_token.tokentype != tt)


// ################### end of macros #################

// ############################# STATES ##################################
/**
 * @brief Beginning of the program
 * 
 * <prolog> -> package main <eol> <eols> <f_list> <eols> <eof>
 * 
 * @return int 
 */
int s_prolog();

/**
 * @brief List of functions
 * 
 * <f_list> -> <eols> func <func> <eols> <f_list>
 * <f_list> -> e
 * 
 * @return int 
 */
int s_f_list();

/**
 * @brief Function
 * 
 * <func> -> id <f_init> <body>
 * <func> -> main <f_init> <body>
 * 
 * @return int 
 */
int s_func();

/**
 * @brief Lists defining function parameters and return types
 * 
 * <f_init> -> (<param_def_list>) <ret_t_list>
 * 
 * @return int 
 */
int s_f_init();

/**
 * @brief Function call
 * 
 * <f_call> -> <param_list>
 * 
 * @return int 
 */
int s_f_call();

/**
 * @brief Body of the function/loop/condition
 * 
 * <body> -> {<eol> <eols> <stat_list>} <eol> <eols>
 * 
 * @return int 
 */
int s_body();

/**
 * @brief List of function parameters - defining
 * 
 * <param_def_list> -> id <type> <param_def_list_n>
 * <param_def_list> -> e
 * 
 * @return int
 */
int s_param_def_list();

/**
 * @brief Multi-parameter list continuation - definition
 * 
 * <param_def_list_n> -> , id <type> <eols> <param_def_list_n>
 * <param_def_list_n> -> e
 *
 * @return int 
 */
int s_param_def_list_n();

/**
 * @brief List of function return parameters
 * 
 * <ret_t_list> -> (<type> <ret_t_list_n>)
 * <ret_t_list> -> e
 * 
 * @return int 
 */
int s_ret_t_list();

/**
 * @brief Multi-return values list continuation - definition
 * 
 * <ret_t_list_n> -> , <type> <ret_t_list_n>
 * <ret_t_list_n> -> e
 *
 * @return int 
 */
int s_ret_t_list_n();

/**
 * @brief Statement: f_call/id_n/for/if
 * 
 * <stat> -> <if>
 * <stat> -> <for>
 * <stat> -> <return>
 * <stat> -> id <id_n>
 * <stat> -> e
 * 
 * @return int 
 */
int s_stat();

/**
 * @brief List of statements
 * 
 * <stat_list> -> <stat> <eol> <eols> <stat_list>
 * <stat_list> -> e
 * 
 * @return int 
 */
int s_stat_list();

/**
 * @brief Condition
 * 
 * <if> -> if <expr><body><else>
 * 
 * @return int 
 */
int s_if();

/**
 * @brief Else statement after condition
 * 
 * <else> -> else <body>
 * <else> -> e
 * 
 * @return int 
 */
int s_else();

/**
 * @brief For loop
 * 
 * <for> -> for <id_def_v>; <expr>; <id_assign_v> <body>
 * 
 * @return int 
 */
int s_for();

/**
 * @brief Return statement
 * 
 * <return> -> return <expr_list>
 * 
 * @return int 
 */
int s_return();

/**
 * @brief List of expressions
 * 
 * <expr_list> -> <expr> <expr_list_n>
 * <expr_list> -> e
 * 
 * @return int 
 */
int s_expr_list();

/**
 * @brief Continuation of expression list
 * 
 * <expr_list_n> -> ,<expr> <expr_list_n>
 * <expr_list_n> -> e
 * 
 * @return int 
 */
int s_expr_list_n();

/**
 * @brief Id was found in the statement - crossroads
 * 
 * <id_n> -> := <id_def>
 * <id_n> -> = <id_assign>
 * <id_n> -> , <id_list_assign>
 * <id_n> -> ( <f_call> )
 * 
 * @return int 
 */
int s_id_n();

/**
 * @brief Variable definition
 * 
 * <id_def> -> <expr>
 * 
 * @return int 
 */
int s_id_def();

/**
 * @brief Voluntary variable definition
 * 
 * <id_def_v> -> <id_def>
 * <id_def_v> -> e
 * 
 * @return int 
 */
int s_id_def_v();

/**
 * @brief List of IDs, must be assignment
 * 
 * <id_list> -> id <id_list_n>
 * 
 * @return int 
 */
int s_id_list();

/**
 * @brief Continuation of ID list
 * 
 * <id_list_n> -> ,id <id_list_n>
 * <id_list_n> -> e
 * 
 * @return int 
 */
int s_id_list_n();

/**
 * @brief Assignment of the ID/s
 * 
 * <id_assign> -> <expr>
 * 
 * @return int 
 */
int s_id_assign();

/**
 * @brief Voluntary assignment of the ID
 * 
 * <id_assign_v> -> <assign>
 * <id_assign_v> -> e
 * 
 * @return int 
 */
int s_id_assign_v();

/**
 * @brief Assignment of list to list
 *
 * <id_list_assign> -> <id_list> = <expr_list>
 * 
 * @return int
 */
int s_id_list_assign();

/**
 * @brief List of parameters of function call
 * 
 * <param_list> -> <expr> <param_list_n>
 * 
 * @return int 
 */
int s_param_list();

/**
 * @brief Continuation of function call parameters
 * 
 * <param_list_n> -> , <expr> <param_list_n>
 * <param_list_n> -> e
 * 
 * @return int 
 */
int s_param_list_n();

/**
 * @brief List of eols
 * 
 * <eols> -> eol <eols>
 * <eols> -> e
 * 
 * @return int 
 */
int s_eols();

/**
 * @brief Variable type
 * 
 * <type> -> int
 * <type> -> float64
 * <type> -> string
 * 
 * @return int 
 */
int s_type();
// ############################# STATES END ###############################

#endif /* SYNTAX_H */
