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
// ################### end of global variables #################

// ################### error codes #################
typedef enum
{
    ERR_PROLOG      // prolog wrong or missing
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
 * Return token
 *
 * param token Token
 */
void return_token(TOKEN token);

/**
 * Print error description on stderr 
 * 
 * param err Error code
 */
void describe_error(ERR_CODE_SYN err);
// ################### end of helper functions #################


// ############################# STATES ##################################
/*
 * Beginning of the program
 * 
 * <prolog> -> package main <eol> <f_list> <eols> <eof>
 * 
 * return int 
 */
int s_prolog();

/*
 * List of functions
 * 
 * <f_list> -> <func> <f_list> ||
 * <f_list> -> e
 * 
 * return int 
 */
int s_f_list();

/*
 * Function
 * 
 * <func> -> func id <f_init> <body>
 * <func> -> func main <f_init> <body>
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
 * <f_call> -> id (<param_list>)
 * 
 * return int 
 */
int s_f_call();

/*
 * Body of the function/loop/condition
 * 
 * <body> -> {<stat_list>} <eol> <eols>
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
 * <stat> -> <id_n>
 * <stat> -> e
 * 
 * return int 
 */
int s_stat();

/*
 * List of statements
 * 
 * <stat_list> -> <eol> <eols> <stat> <stat_list>
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
 * <id_n> -> <f_call>
 * <id_n> -> <id_def>
 * <id_n> -> <id_assign>
 * 
 * return int 
 */
int s_id_n();

/*
 * Variable definition
 * 
 * <id_def> -> id := <expr>
 * 
 * return int 
 */
int s_id_def();

/*
 * List of IDs, must be assignment
 * 
 * <id_list> -> id,id <id_list_n>
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
 * <id_assign> -> id = <expr>
 * <id_assign> -> <id_list> = <expr_list>
 * 
 * return int 
 */
int s_id_assign();

/*
 * Parameter of function call
 * 
 * <param> -> <expr>
 * 
 * return int 
 */
int s_param();

/*
 * List of parameters of function call
 * 
 * <param_list> -> <param> <param_list_n>
 * 
 * return int 
 */
int s_param_list();

/*
 * Continuation of function call parameters
 * 
 * <param_list_n> -> <param>, <param_list_n>
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

// ############################# STATES END ###############################


#endif /* SYNTAX_H */
