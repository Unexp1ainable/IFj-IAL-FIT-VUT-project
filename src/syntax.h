/**
 * @file syntax.h
 * @author Samuel Repka
 * @brief Header file for syntax checker
 * @version 0.1
 * @date 2020-11-26
 * 
 */
#include <stdio.h>
#include <stdbool.h>
#include "scanner.h"
#include "types_definition.h"


// ############################# helper functions

token_t get_token(){
    static token_t buffer = {eol, -1};

}

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
 * 13.	<ret_t_list> -> <type> <ret_t_list_n>
 * 14.	<ret_t_list> -> e
 * 
 * @return int 
 */
int s_ret_t_list();

/**
 * @brief Multi-return values list continuation - definition
 * 
 * 15.	<ret_t_list_n> -> , <type> <ret_t_list_n>
16.	<ret_t_list_n> -> e
 *
 * @return int 
 */
int s_ret_t_list_n();

/**
 * @brief Statement: f_call/id_n/for/if
 * 
 * 17.	<stat> -> <if>
18.	<stat> -> <for>
19.	<stat> -> <return>
20.	<stat> -> <id_n>
21.	<stat> -> e
 * 
 * @return int 
 */
int s_stat();

/**
 * @brief List of statements
 * 
 * 22.	<stat_list> -> <stat> <eol> <eols> <stat_list>
23.	<stat_list> -> e
 * 
 * @return int 
 */
int s_stat_list();

/**
 * @brief Condition
 * 
 * 24.	<if> -> if <expr><body><else>
 * 
 * @return int 
 */
int s_if();

/**
 * @brief Else statement after condition
 * 
 * 25.	<else> -> else <body>
26.	<else> -> e
 * 
 * @return int 
 */
int s_else();

/**
 * @brief For loop
 * 
 * 27.	<for> -> for <id_def_v>; <expr>; <id_assign_v> <body>
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
 * 30.	<expr_list> -> <expr> <expr_list_n>
31.	<expr_list> -> e
 * 
 * @return int 
 */
int s_expr_list();

/**
 * @brief Continuation of expression list
 * 
 * 50.	<expr_list_n> -> ,<expr> <expr_list_n>
51.	<expr_list_n> -> e
 * 
 * @return int 
 */
int s_expr_list_n();

/**
 * @brief Id was found in the statement - crossroads
 * 
 * 32.	<id_n> -> <f_call>
33.	<id_n> -> <id_def>
34.	<id_n> -> <id_assign>
 * 
 * @return int 
 */
int s_id_n();

/**
 * @brief Variable definition
 * 
 * 36.	<id_def> -> id := <expr>
 * 
 * @return int 
 */
int s_id_def();

/**
 * @brief List of IDs, must be assignment
 * 
 * 39.	<id_list> -> id,id <id_list_n>
 * 
 * @return int 
 */
int s_id_list();

/**
 * @brief Continuation of ID list
 * 
 * 40.	<id_list_n> -> ,id <id_list_n>
41.	<id_list_n> -> e
 * 
 * @return int 
 */
int s_id_list_n();

/**
 * @brief Assignment of the ID/s
 * 
 * 42.	<id_assign> -> id = <expr>
43.	<id_assign> -> <id_list> = <expr_list>
 * 
 * @return int 
 */
int s_id_assign();

/**
 * @brief Parameter of function call
 * 
 * 44.	<param> -> <expr>
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
 * 46.	<param_list_n> -> <param>, <param_list_n>
47.	<param_list_n> -> e
 * 
 * @return int 
 */
int s_param_list_n();

/**
 * @brief List of eols
 * 
 * 48.	<eols> -> eol <eols>
49.	<eols> -> e
 * 
 * @return int 
 */
int s_eols();

// ############################# STATES END ###############################
