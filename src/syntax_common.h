#ifndef SYNTAX_COMMON
#define SYNTAX_COMMON

#include "newscanner.h"
#include "symtable_list.h"


// ################### error codes #################

/**
 * @brief Error codes
 * 
 */
typedef enum
{
    NO_ERR,
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
    ERR_TYPE_EXPECTED,
    ERR_ID_REDEFINITION,
    ERR_ID_UNDEFINED,
    ERR_WRONG_FUNC_PARAM,
    ERR_MAIN_MISSING,
    ERR_INVALID_MAIN_DEFINITION,
    ERR_INVALID_EXPRESSION,
    ERR_WRONG_NUMBER_LVALUES,
    ERR_WRONG_LVALUE_TYPE,
    ERR_SYMTABLE,
    ERR_TYPE_MISMATCH,
    ERR_TYPE_COMB,
    ERR_EXP_ORDER,
    ERR_UNDEF_ID,
    ERR_EMPTY_EXP,
    ERR_ALLOC_M

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

// ################### end of macros #################

/**
 * @brief Maximum number of return values that can be returned from a function
 * 
 */
#define MAX_N_RET_VAL 100

/**
 * @brief Buffer for returned token
 * 
 */
extern TOKEN* token_buffer;

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

#endif /* SYNTAX_COMMON */
