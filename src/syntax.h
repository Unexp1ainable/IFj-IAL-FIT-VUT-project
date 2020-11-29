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
1.	<prolog> -> package main <eol> <eols> <f_list> <eols> <eof>
 * 
 * return int 
 */
int s_prolog();

/*
 * List of functions
 * 
2.	<f_list> -> func <func> <eols> <f_list>
3.	<f_list> -> e
 * 
 * return int 
 */
int s_f_list();

/*
 * Function
 * 
4.	<func> -> id <f_init> <body>
5.	<func> -> main <f_init> <body>
 * 
 * return int 
 */
int s_func();

/*
 * Lists defining function parameters and return types
 * 
6.	<f_init> -> (<param_def_list>) <ret_t_list>
 * 
 * return int 
 */
int s_f_init();

/*
 * Function call
 * 
46.	<f_call> -> <param_list>
 * 
 * return int 
 */
int s_f_call();

/*
 * Body of the function/loop/condition
 * 
7.	<body> -> {<eol> <eols> <stat_list>} <eol> <eols>
 * 
 * return int 
 */
int s_body();

/*
 * List of function parameters - defining
 * 
8.	<param_def_list> -> id <type> <param_def_list_n>
9.	<param_def_list> -> e
 * 
 * return int
 */
int s_param_def_list();

/*
 * Multi-parameter list continuation - definition
 * 
10.	<param_def_list_n> -> , id <type> <param_def_list_n>
11.	<param_def_list_n> -> e
 *
 * return int 
 */
int s_param_def_list_n();

/*
 * List of function return parameters
 * 
12.	<ret_t_list> -> (<type> <ret_t_list_n>)
13.	<ret_t_list> -> e
 * 
 * return int 
 */
int s_ret_t_list();

/*
 * Multi-return values list continuation - definition
 * 
14.	<ret_t_list_n> -> , <type> <ret_t_list_n>
15.	<ret_t_list_n> -> e
 *
 * return int 
 */
int s_ret_t_list_n();

/*
 * Statement: f_call/id_n/for/if
 * 
16.	<stat> -> <if>
17.	<stat> -> <for>
18.	<stat> -> <return>
19.	<stat> -> id <id_n>
20.	<stat> -> e
 * 
 * return int 
 */
int s_stat();

/*
 * List of statements
 * 
22.	<stat_list> -> <stat> <eol> <eols> <stat_list>
23.	<stat_list> -> e
 * 
 * return int 
 */
int s_stat_list();

/*
 * Condition
 * 
24.	<if> -> if <expr><body><else>
 * 
 * return int 
 */
int s_if();

/*
 * Else statement after condition
 * 
25.	<else> -> else <body>
26.	<else> -> e
 * 
 * return int 
 */
int s_else();

/*
 * For loop
 * 
27.	<for> -> for <id_def_v>; <expr>; <id_assign_v> <body>
 * 
 * return int 
 */
int s_for();

/*
 * Return statement
 * 
28.	<return> -> return <expr_list>
 * 
 * return int 
 */
int s_return();

/*
 * List of expressions
 * 
30.	<expr_list> -> <expr> <expr_list_n>
31.	<expr_list> -> e
 * 
 * return int 
 */
int s_expr_list();

/*
 * Continuation of expression list
 * 
32.	<expr_list_n> -> ,<expr> <expr_list_n>
33.	<expr_list_n> -> e
 * 
 * return int 
 */
int s_expr_list_n();

/*
 * Id was found in the statement - crossroads
 * 
34.	<id_n> -> := <id_def>
35.	<id_n> -> = <id_assign>
36.	<id_n> -> , <id_list_assign>
37.	<id_n> -> ( <f_call> )
 * 
 * return int 
 */
int s_id_n();

/*
 * Variable definition
 * 
38.	<id_def> -> <expr>
 * 
 * return int 
 */
int s_id_def();

/*
 * Voluntary variable definition
 * 
39.	<id_def_v> -> <id_def>
40.	<id_def_v> -> e
 * 
 * return int 
 */
int s_id_def_v();

/*
 * List of IDs, must be assignment
 * 
43.	<id_list> -> id <id_list_n>
 * 
 * return int 
 */
int s_id_list();

/*
 * Continuation of ID list
 * 
44.	<id_list_n> -> ,id <id_list_n>
45.	<id_list_n> -> e
 * 
 * return int 
 */
int s_id_list_n();

/*
 * Assignment of the ID/s
 * 
41.	<id_assign> -> <expr>
 * 
 * return int 
 */
int s_id_assign();

/*
 * Voluntary assignment of the ID
 * 
56.	<id_assign_v> -> <expr>
57.	<id_assign_v> -> e
 * 
 * return int 
 */
int s_id_assign();

/*
 * Assignment of list to list
 *
42.	<id_list_assign> -> <id_list> = <expr_list>
 * 
 * return int
 */
int s_id_list_assign();

/*
 * List of parameters of function call
 * 
47.	<param_list> -> <expr> <param_list_n>
 * 
 * return int 
 */
int s_param_list();

/*
 * Continuation of function call parameters
 * 
48.	<param_list_n> -> , <param_list_n>
49.	<param_list_n> -> e
 * 
 * return int 
 */
int s_param_list_n();

/*
 * List of eols
 * 
51.	<eols> -> eol <eols>
52.	<eols> -> e
 * 
 * return int 
 */
int s_eols();

/*
 * Variable type
 * 
53. <type> -> int
54. <type> -> float64
55. <type> -> string
 * 
 * return int 
 */
int s_type();
// ############################# STATES END ###############################

#endif /* SYNTAX_H */
