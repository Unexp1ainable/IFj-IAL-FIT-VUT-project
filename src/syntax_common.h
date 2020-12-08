#ifndef SYNTAX_COMMON
#define SYNTAX_COMMON

#include "newscanner.h"
#include "symtable_list.h"
#include "error.h"

// ################### error codes #################

/**
 * @brief Error codes
 * 
 */
typedef enum
{
    NO_ERR, // 0
    // =================================
    LEXICAL_ERRORS, // 1
    //----------------------------
    ERR_LEX,                     // lexical error

    // =================================
    SYNTAX_ERRORS, // 2
    //----------------------------
    ERR_PROLOG,                  // prolog wrong or missing
    ERR_EOL_EXPECTED,            // required eol missing
    ERR_EOF_EXPECTED,            // eof was not found somehow
    ERR_FUNC_EXPECTED,           // expected "func" token
    ERR_FUNC_ID_EXPECTED,        // expected id
    ERR_MULTIPLE_MAIN_FUNC,      // multiple definitions of main function
    ERR_F_PAR_L_BRACKET,         // missing ( in function parameter definition
    ERR_F_PAR_R_BRACKET,         // missing ) in function parameter definition
    ERR_BODY_START,              // missing {
    ERR_BODY_END,                // missing }
    ERR_FUNC_DEF_UNEXPECTED,     // unexpected token in function definition
    ERR_FUNC_DEF_RET_UNEXPECTED, // unexpected token in function definition return values
    ERR_STAT_UNEXPECTED,         // unexpected token, expected statement
    ERR_IF_EXPECTED,             // missing if
    ERR_FOR_EXPECTED,            // missing for
    ERR_SEMICOLON_EXPECTED,      // missing ;
    ERR_RETURN_EXPECTED,         // missing "return"
    ERR_COMMA_EXPECTED,          // missing comma
    ERR_UNKNOWN_ID_OPERATION,    // found id, but operation could not be determined
    ERR_ID_DEF_EXPECTED,         // expected :=
    ERR_ID_EXPECTED,             // identifier expected
    ERR_UNEXPECTED_TOKEN,        // unexpected token
    ERR_ID_ASSIGN_EXPECTED,      // expected =
    ERR_TYPE_EXPECTED,           // expected type identifier
    ERR_MAIN_MISSING,            // main() function was not defined
    ERR_EMPTY_EXP,               // expression was empty
    ERR_MISSING_ELSE,            // if is without the else
    ERR_INVALID_EXPRESSION,      // expected evaluation result to be bool

    // =================================
    SEM_ERR_UNDEFS, // 3
    //----------------------------
    ERR_FUNC_REDEFINITION,        // function was already defined
    ERR_ID_REDEFINITION,         // id was already defined
    ERR_ID_UNDEFINED,            // id was not defined
    ERR_TYPE_MISMATCH,           // trying to assign different type to variable than is defined

    // =================================
    SEM_ERR_NEW_VARS, // 4
    //----------------------------
    ERR_TYPE_UNDETERMINED,       // failed to determine type of the new variable

    // =================================
    SEM_ERR_TYPE_COMPATS, // 5
    //----------------------------
    ERR_TYPE_COMB,               // invalid expression
    ERR_EXP_ORDER,               // TODO idk invalid expression order 

    // =================================
    SEM_ERR_PAR_NUMS, // 6
    //----------------------------
    ERR_WRONG_FUNC_PARAM,        // given parameters are not of the type that is defined in function
    ERR_WRONG_NUMBER_LVALUES,    // too many ids on the left side of the assignment

    // =================================
    SEM_ERR_OTHERS, // 7
    //----------------------------
    ERR_INVALID_MAIN_DEFINITION, // main() function has defined parameters or return values

    // =================================
    SEM_ERR_DIV_ZEROS, // 9
    //----------------------------
    ERR_ZERO_DIVISION,           // division by zero

    // =================================
    INTERN_ERRORS,  // 99
    //----------------------------
    ERR_ALLOC_M,                 // memory allocation failure
    ERR_SYMTABLE                 // error in table of symbols

} ERR_CODE_SYN;
// ################### end of error codes #################

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

/**
 * @brief Maximum number of return values that can be returned from a function
 * 
 */
#define MAX_N_RET_VAL 100

// ################### end of macros #################

// ################### global variables #################

/**
 * @brief Buffer for returned token
 * 
 */
extern TOKEN *token_buffer;

/**
 * @brief Token being processed
 * 
 */
extern TOKEN *curr_token;

/**
 * @brief Line number
 * 
 */
extern unsigned long line;

/**
 * @brief List of all loaded tokens
 * 
 */
extern TokenList tokens;

/**
 * @brief Marks whether we are doing first pass through code or second.
 * 
 */
extern bool first_pass;

// ################### end of global variables #################

// ################### functions #################

/**
 * @brief Maps returned code to the required program return code
 * 
 * @param code Return code from analyser
 * @return int Program return code
 */
int map_err_code(int code);

/**
 * @brief Get the token object
 * 
 * param token Pointer to output token
 * @return int Return code. 0 if good
 */
void get_token(TOKEN **token);

/**
 * @brief Put token to temp holding space
 *
 * param token Token
 */
void return_token(TOKEN *token);

/**
 * @brief Print error description on stderr  
 * 
 * 
 * param err Error code
 */
void describe_error(ERR_CODE_SYN err);

/**
 * @brief Check if id is already in symtable
 * 
 * @param list List of symtables
 * @param key_t Id which to look for
 * 
 * @return true If it is in symtable
 * @return false If it is not in symtable
 */
Symtable_item *was_it_defined(symtableList list, char *key);

/**
 * @brief Processes all sucessive eols
 * 
 * 
 * @return int 
 */
int s_eols();

// ################### end of functions #################


#endif /* SYNTAX_COMMON */
