/**
 * @file syntax.h
 * @author Samuel Repka
 * @brief Header file for syntax checker
 * @version 0.1
 * @date 2020-11-26
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
/**
 * @brief Buffer for returned token
 * 
 */
extern TOKEN token_buffer;

// ############################# helper functions ###########################
/**
 * @brief Get the token object
 * 
 * @param token Pointer to output token
 * @return int Return code. 0 if good
 */
int get_token(TOKEN *token);

/**
 * @brief Return token
 * 
 * @param token Token
 */
void return_token(TOKEN token);
// ################### end of helper functions #################


// ############################# STATES ##################################
/**
 * @brief Beginning of the program
 * 
 * <prolog> -> package main <f_list> <eol>
 * 
 * @return int 
 */
int s_prolog();

/**
 * @brief List of functions
 * 
 * <f_list> -> <func> <f_list>
 * <f_list> -> e
 * 
 * @return int 
 */
int s_f_list();

/**
 * @brief Function
 * 
 * <func> -> func id <f_init> <body>
 * <func> -> func main <f_init> <body>
 * 
 * @return int 
 */
int s_func();

/**
 * @brief Lists defining function parameters and return types
 * 
 * <f_init> -> (<param_def_list>) (<ret_t_list>)
 * 
 * @return int 
 */
int s_f_init();

/**
 * @brief Function call
 * 
 * <f_call> -> id (<param_list>)
 * 
 * @return int 
 */
int s_f_call();

/**
 * @brief Body of the function/loop/condition
 * 
 * <body> -> {<stat_list>}
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
 * <param_def_list_n> -> , id <type> <param_def_list_n>
 * <param_def_list_n> -> e
 *
 * @return int 
 */
int s_param_def_list_n();

/**
 * @brief List of function return parameters
 * 
 * <ret_t_list> -> <type> <ret_t_list_n>
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
 * <stat> -> <id_n>
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
 * <id_n> -> <f_call>
 * <id_n> -> <id_def>
 * <id_n> -> <id_assign>
 * 
 * @return int 
 */
int s_id_n();

/**
 * @brief Variable definition
 * 
 * <id_def> -> id := <expr>
 * 
 * @return int 
 */
int s_id_def();

/**
 * @brief List of IDs, must be assignment
 * 
 * <id_list> -> id,id <id_list_n>
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
 * <id_assign> -> id = <expr>
 * <id_assign> -> <id_list> = <expr_list>
 * 
 * @return int 
 */
int s_id_assign();

/**
 * @brief Parameter of function call
 * 
 * <param> -> <expr>
 * 
 * @return int 
 */
int s_param();

/**
 * @brief List of parameters of function call
 * 
 * <param_list> -> <param> <param_list_n>
 * 
 * @return int 
 */
int s_param_list();

/**
 * @brief Continuation of function call parameters
 * 
 * <param_list_n> -> <param>, <param_list_n>
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

// ############################# STATES END ###############################


#endif /* SYNTAX_H */
