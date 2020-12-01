/**
 * @file newscanner.h
 * @brief header file for scanner. IFJ project 2020
 * 
 * @author Timotej Kamensky, xkamen24,+421 944 687 328
 * @date 25.11.2020
 * */

#ifndef _NEWSCANNER_H
#define _NEWSCANNER_H

#include "dynamic_string.h"
#define OK  0
#define WTF 1
#define exitus 12
#define memoryerror 42
/**
 * @enum enumeration of reserved keywords by golang
 * @note zadanie, strana 3
 * */
typedef enum
{
    KEYWORD_ELSE,
    KEYWORD_FOR,
    KEYWORD_FUNC,
    KEYWORD_IF,
    KEYWORD_PACKAGE,
    KEYWORD_RETURN,
    KEYWORD_NONE
} RESERVED_KEYWORDS;

/**
 * @enum all tokens
 * */
typedef enum
{
    //Operands
    TOKEN_TYPE_ADD,
    TOKEN_TYPE_SUBTRACT,
    TOKEN_TYPE_MULTIPLY,
    TOKEN_TYPE_DIVIDE,
    //Znamienka
    TOKEN_TYPE_EQUAL,
    TOKEN_TYPE_NOT_EQUAL,
    TOKEN_TYPE_LOWER_THAN,
    TOKEN_TYPE_GREATER_THAN,
    TOKEN_TYPE_LOWER_EQUAL,
    TOKEN_TYPE_GREATER_EQUAL,
    TOKEN_TYPE_SUB1,
    TOKEN_TYPE_ADD1,
    //Others
    TOKEN_TYPE_OPENING_CLASSIC_BRACKET,
    TOKEN_TYPE_CLOSING_CLASSIC_BRACKET,
    TOKEN_TYPE_OPENING_CURVY_BRACKET,
    TOKEN_TYPE_CLOSING_CURVY_BRACKET,
    TOKEN_TYPE_COMMA,
    TOKEN_TYPE_SEMICOLON,
    TOKEN_TYPE_UNDERSCORE,
    TOKEN_TYPE_EOF,
    TOKEN_TYPE_EOL,
    TOKEN_TYPE_IDENTIFIER,

    TOKEN_TYPE_RESERVED_KEYWORD,
    TOKEN_TYPE_STRING,
    TOKEN_TYPE_INTEGER,
    TOKEN_TYPE_FLOAT64,
    TOKEN_TYPE_DEFINE,
    TOKEN_TYPE_ASSIGN,
    TOKEN_TYPE_MAIN,
    TOKEN_TYPE_EMPTY

} TOKEN_TYPES;

typedef struct
{
    TOKEN_TYPES tokentype;

    union
    {
        Dynamic_string *string;
        int64_t integer;
        double floater;
        RESERVED_KEYWORDS keyword;
    };
} TOKEN;

/**
 * @enum states of the scanner
 * */
typedef enum{
FSM_START,                  //state used at the beginning of the scan
FSM_BACKSLASH,
FSM_SLASH,                  // there has been a backslash, expect * or another backslash
FSM_LINE_COMMENT_PROPER,    // ignore all till EOL
FSM_BLOCK_COMMENT_PROPER,   //ignore all till *backslash
FSM_BLOCK_COMMENT_END,      //block comment, got *, expect backslash
FSM_STRING,                 //there has been ", reading the whole string till another "
FSM_COLON,
FSM_ID,
FSM_UNDERLINE,
FSM_EXCLAMATION,
FSM_SMALLERTHAN,
FSM_GREATERTHAN,
FSM_EQUALSIGN,
FSM_DECNUMBER,
FSM_DECNUMBER_ZERO,
FSM_DECNUMBER_EXPONENT_OR_SIGN,
FSM_DECNUMBER_EXPONENT_AND_SIGN,
FSM_DECNUMBER_EXPONENT_NUMBER,
FSM_DECNUMBER_FLOAT,
FSM_DECNUMBER_EXP_DEC,
FSM_HEXNUMBER_1,
FSM_HEXNUMBER_2,

}FSM_STATES;

/*
####################################################################################################################
###########################   Function headers   ###################################################################################
####################################################################################################################
*/
int get_next_token(TOKEN * tokenptr);

void set_fsm_state(FSM_STATES input);
void make_token_float(TOKEN *t,char* num);
int maketoken(); //TODO

bool dynamic_string_copy(TOKEN *token, Dynamic_string *dynamicstring);

int get_next_token(TOKEN *tokenptr);

// variables
extern FILE *fileptr;
extern Dynamic_string stringbuffer;
extern bool end;

#endif
