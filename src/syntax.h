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
#include "symtable_list.h"
#include "symtable.h"
#include "syntax_common.h"
// ########################## global variables #########################

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

/**
 * @brief Temporary holding space for IDs on the left side of assignment.
 * 
 */
extern TOKEN id_list[MAX_N_RET_VAL];

/**
 * @brief Number of elements in id_list
 * 
 */
extern int id_list_n;
// ################### end of global variables #################

// ############################# helper functions ###########################

/**
 * @brief Check if next token is eol and increment line
 * 
 */
bool eol_required();

/**
 * @brief Put built in functions to the table
 * 
 * @param table Table for variables to be put in
 * @return int Error code, 0 if ok
 */
int initialise_predefined(Symtable *table);

// ################### end of helper functions #################

// ################### macros #################

/**
 * @brief Condition for checking tokentype, ==
 * 
 */
#define TOKEN_IS(tt) (curr_token->tokentype == tt)

/**
 * @brief Condition for checking tokentype, !=
 * 
 */
#define TOKEN_IS_NOT(tt) (curr_token->tokentype != tt)

// ################### end of macros #################

// ############################# STATES ##################################
/**
 * @brief Beginning of the program
 * 
 * <prolog> -> package main <eol> <eols> <f_list> <eols> <eof>
 * 
 * @return int 
 */
int s_prolog(symtableList symlist);

/**
 * @brief List of functions
 * 
 * <f_list> -> <eols> func <func> <eols> <f_list>
 * <f_list> -> e
 * 
 * @return int 
 */
int s_f_list(symtableList symlist);

/**
 * @brief Function
 * 
 * <func> -> id <f_init> <body>
 * <func> -> main <f_init> <body>
 * 
 * @return int 
 */
int s_func(symtableList symlist);

/**
 * @brief Lists defining function parameters and return types
 * 
 * <f_init> -> (<param_def_list>) <ret_t_list>
 * 
 * @return int 
 */
int s_f_init(symtableList symlist, char *func_id);

/**
 * @brief Function call
 * 
 * <f_call> -> <param_list>
 * 
 * @return int 
 */
int s_f_call(symtableList symlist, Symtable_item *func_def);

/**
 * @brief Body of the function/loop/condition
 * 
 * <body> -> {<eol> <eols> <stat_list>} <eol> <eols>
 * 
 * @return int 
 */
int s_body(symtableList symlist);

/**
 * @brief List of function parameters - defining
 * 
 * <param_def_list> -> id <type> <param_def_list_n>
 * <param_def_list> -> e
 * 
 * @return int
 */
int s_param_def_list(symtableList symlist, char *func_id);

/**
 * @brief Multi-parameter list continuation - definition
 * 
 * <param_def_list_n> -> , id <type> <eols> <param_def_list_n>
 * <param_def_list_n> -> e
 *
 * @return int 
 */
int s_param_def_list_n(symtableList symlist, char *func_id);

/**
 * @brief List of function return parameters
 * 
 * <ret_t_list> -> (<type> <ret_t_list_n>)
 * <ret_t_list> -> e
 * 
 * @return int 
 */
int s_ret_t_list(symtableList symlist, char *func_id);

/**
 * @brief Multi-return values list continuation - definition
 * 
 * <ret_t_list_n> -> , <type> <ret_t_list_n>
 * <ret_t_list_n> -> e
 *
 * @return int 
 */
int s_ret_t_list_n(symtableList symlist, char *func_id);

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
int s_stat(symtableList symlist);

/**
 * @brief List of statements
 * 
 * <stat_list> -> <stat> <eol> <eols> <stat_list>
 * <stat_list> -> e
 * 
 * @return int 
 */
int s_stat_list(symtableList symlist);

/**
 * @brief Condition
 * 
 * <if> -> if <expr><body><else>
 * 
 * @return int 
 */
int s_if(symtableList symlist);

/**
 * @brief Else statement after condition
 * 
 * <else> -> else <body>
 * <else> -> e
 * 
 * @return int 
 */
int s_else(symtableList symlist);

/**
 * @brief For loop
 * 
 * <for> -> for <id_def_v>; <expr>; <id_assign_v> <body>
 * 
 * @return int 
 */
int s_for(symtableList symlist);

/**
 * @brief Return statement
 * 
 * <return> -> return <expr_list>
 * 
 * @return int 
 */
int s_return(symtableList symlist);

/**
 * @brief List of expressions
 * 
 * <expr_list> -> <expr> <expr_list_n>
 * <expr_list> -> e
 * 
 * @return int 
 */
int s_expr_list(symtableList symlist);

/**
 * @brief Continuation of expression list
 * 
 * <expr_list_n> -> ,<expr> <expr_list_n>
 * <expr_list_n> -> e
 * 
 * @return int 
 */
int s_expr_list_n(symtableList symlist);

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
int s_id_n(symtableList symlist, char *id);

/**
 * @brief Variable definition
 * 
 * <id_def> -> <expr>
 * 
 * @return int 
 */
int s_id_def(symtableList symlist, char *id);

/**
 * @brief Voluntary variable definition
 * 
 * <id_def_v> -> <id_def>
 * <id_def_v> -> e
 * 
 * @return int 
 */
int s_id_def_v(symtableList symlist);

/**
 * @brief List of IDs, must be assignment
 * 
 * <id_list> -> id <id_list_n>
 * 
 * @return int 
 */
int s_id_list(symtableList symlist);

/**
 * @brief Continuation of ID list
 * 
 * <id_list_n> -> ,id <id_list_n>
 * <id_list_n> -> e
 * 
 * @return int 
 */
int s_id_list_n(symtableList symlist);

/**
 * @brief Assignment of the ID/s
 * 
 * <id_assign> -> <expr>
 * 
 * @return int 
 */
int s_id_assign(symtableList symlist);

/**
 * @brief Voluntary assignment of the ID
 * 
 * <id_assign_v> -> <assign>
 * <id_assign_v> -> e
 * 
 * @return int 
 */
int s_id_assign_v(symtableList symlist);

/**
 * @brief Assignment of list to list
 *
 * <id_list_assign> -> <id_list> = <expr_list>
 * 
 * @return int
 */
int s_id_list_assign(symtableList symlist);


/**
 * @brief Assigning values to id/s from function.
 * 
 * @param symlist List of symtables
 * @param func_def Function, from which we are assigning
 * @return int Return code (see syntax_common.h error_codes)
 */
int s_func_assign(symtableList symlist, Symtable_item *func_def);

/**
 * @brief List of parameters of function call
 * 
 * <param_list> -> <expr> <param_list_n>
 * 
 * @return int 
 */
int s_param_list(symtableList symlist, Symtable_item *func_def);

/**
 * @brief Continuation of function call parameters
 * 
 * <param_list_n> -> , <expr> <param_list_n>
 * <param_list_n> -> e
 * 
 * @return int 
 */
int s_param_list_n(symtableList symlist, Symtable_item *func_def, int n);

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
int s_type(DataType *type);

//TODO
int s_expr(symtableList TableList, TermType *type); // compatibility
// ############################# STATES END ###############################

#endif /* SYNTAX_H */
