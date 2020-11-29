/*
 * file syntax.h
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
#include "mocks.h"
// ########################## global variables #########################
/*
 * Buffer for returned token
 * 
 */
extern TOKEN token_buffer;

/*
 * Token being processed
 * 
 */
extern TOKEN curr_token;

/*
 * Line number
 * 
 */
extern unsigned long line;

/*
 * For checking if main was already defined.
 *
 */

extern bool main_defined;
// ################### end of global variables #################

// ################### error codes #################
typedef enum
{
    ERR_PROLOG,      // prolog wrong or missing
    ERR_EOL_EXPECTED, // required eol missing
    ERR_EOF_EXPECTED,
    ERR_FUNC_EXPECTED,
    ERR_FUNC_ID_EXPECTED,
    ERR_MULTIPLE_MAIN_FUNC,
    ERR_F_PAR_L_BRACKET,
    ERR_F_PAR_R_BRACKET

} ERR_CODE_SYN;
// ################### end of error codes #################

// ############################# helper functions ###########################
/*
 * Get the token object
 * 
 * param token Pointer to output token
 * return int Return code. 0 if good
 */
int get_token(TOKEN *token);

/**
 * Put token to temp holding space
 *
 * param token Token
 */
void return_token(TOKEN token);

/**
 * Print error description on stderr  
 * 
 * 
 * param err Error code
 */
void describe_error(ERR_CODE_SYN err);

/*
 * Check if next token is eol and increment line
 * 
 */
bool eol_required();
// ################### end of helper functions #################

// ############################# STATES ##################################
/*
 * Beginning of the program
 * 
 * <prolog> -> package main <eol> <eols> <f_list> <eols> <eof>
 * 
 * return int 
 */
int s_prolog();

/*
 * List of functions
 * 
 * <f_list> -> func <func> <eols> <f_list>
 * <f_list> -> e
 * 
 * return int 
 */
int s_f_list();

/*
 * Function
 * 
 * <func> -> id <f_init> <body>
 * <func> -> main <f_init> <body>
 * 
 * return int 
 */
int s_func();

/*
 * Lists defining function parameters and return types
 * 
 * <f_init> -> (<param_def_list>) <ret_t_list>
 * 
 * return int 
 */
int s_f_init();

/*
 * Function call
 * 
 * <f_call> -> <param_list>
 * 
 * return int 
 */
int s_f_call();

/*
 * Body of the function/loop/condition
 * 
 * <body> -> {<eol> <eols> <stat_list>} <eol> <eols>
 * 
 * return int 
 */
int s_body();

/*
 * List of function parameters - defining
 * 
 * <param_def_list> -> id <type> <param_def_list_n>
 * <param_def_list> -> e
 * 
 * return int
 */
int s_param_def_list();

/*
 * Multi-parameter list continuation - definition
 * 
 * <param_def_list_n> -> , id <type> <param_def_list_n>
 * <param_def_list_n> -> e
 *
 * return int 
 */
int s_param_def_list_n();

/*
 * List of function return parameters
 * 
 * <ret_t_list> -> (<type> <ret_t_list_n>)
 * <ret_t_list> -> e
 * 
 * return int 
 */
int s_ret_t_list();

/*
 * Multi-return values list continuation - definition
 * 
 * <ret_t_list_n> -> , <type> <ret_t_list_n>
 * <ret_t_list_n> -> e
 *
 * return int 
 */
int s_ret_t_list_n();

/*
 * Statement: f_call/id_n/for/if
 * 
 * <stat> -> <if>
 * <stat> -> <for>
 * <stat> -> <return>
 * <stat> -> id <id_n>
 * <stat> -> e
 * 
 * return int 
 */
int s_stat();

/*
 * List of statements
 * 
 * <stat_list> -> <stat> <eol> <eols> <stat_list>
 * <stat_list> -> e
 * 
 * return int 
 */
int s_stat_list();

/*
 * Condition
 * 
 * <if> -> if <expr><body><else>
 * 
 * return int 
 */
int s_if();

/*
 * Else statement after condition
 * 
 * <else> -> else <body>
 * <else> -> e
 * 
 * return int 
 */
int s_else();

/*
 * For loop
 * 
 * <for> -> for <id_def_v>; <expr>; <id_assign_v> <body>
 * 
 * return int 
 */
int s_for();

/*
 * Return statement
 * 
 * <return> -> return <expr_list>
 * 
 * return int 
 */
int s_return();

/*
 * List of expressions
 * 
 * <expr_list> -> <expr> <expr_list_n>
 * <expr_list> -> e
 * 
 * return int 
 */
int s_expr_list();

/*
 * Continuation of expression list
 * 
 * <expr_list_n> -> ,<expr> <expr_list_n>
 * <expr_list_n> -> e
 * 
 * return int 
 */
int s_expr_list_n();

/*
 * Id was found in the statement - crossroads
 * 
 * <id_n> -> := <id_def>
 * <id_n> -> = <id_assign>
 * <id_n> -> , <id_list_assign>
 * <id_n> -> ( <f_call> )
 * 
 * return int 
 */
int s_id_n();

/*
 * Variable definition
 * 
 * <id_def> -> <expr>
 * 
 * return int 
 */
int s_id_def();

/*
 * Voluntary variable definition
 * 
 * <id_def_v> -> <id_def>
 * <id_def_v> -> e
 * 
 * return int 
 */
int s_id_def_v();

/*
 * List of IDs, must be assignment
 * 
 * <id_list> -> id <id_list_n>
 * 
 * return int 
 */
int s_id_list();

/*
 * Continuation of ID list
 * 
 * <id_list_n> -> ,id <id_list_n>
 * <id_list_n> -> e
 * 
 * return int 
 */
int s_id_list_n();

/*
 * Assignment of the ID/s
 * 
 * <id_assign> -> <expr>
 * 
 * return int 
 */
int s_id_assign();

/*
 * Voluntary assignment of the ID
 * 
 * <id_assign_v> -> <expr>
 * <id_assign_v> -> e
 * 
 * return int 
 */
int s_id_assign();

/*
 * Assignment of list to list
 *
 * <id_list_assign> -> <id_list> = <expr_list>
 * 
 * return int
 */
int s_id_list_assign();

/*
 * List of parameters of function call
 * 
 * <param_list> -> <expr> <param_list_n>
 * 
 * return int 
 */
int s_param_list();

/*
 * Continuation of function call parameters
 * 
 * <param_list_n> -> , <param_list_n>
 * <param_list_n> -> e
 * 
 * return int 
 */
int s_param_list_n();

/*
 * List of eols
 * 
 * <eols> -> eol <eols>
 * <eols> -> e
 * 
 * return int 
 */
int s_eols();

/*
 * Variable type
 * 
 * <type> -> int
 * <type> -> float64
 * <type> -> string
 * 
 * return int 
 */
int s_type();
// ############################# STATES END ###############################

#endif /* SYNTAX_H */
