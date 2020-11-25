/**
 * @file newscanner.h
 * @brief header file for scanner. IFJ project 2020
 * 
 * @author Timotej Kamensky, xkamen24,+421 944 687 328
 * */

#ifndef _NEWSCANNER_H
#define _NEWSCANNER_H
/**
 * @enum enumeration of reserved keywords by golang
 * @note zadanie, strana 3
 * */
typedef enum{
    KEYWORD_ELSE,
    KEYWORD_FLOAT64,
    KEYWORD_FOR,
    KEYWORD_FUNC,
    KEYWORD_IF,
    KEYWORD_INT,
    KEYWORD_PACKAGE,
    KEYWORD_RETURN,
    KEYWORD_STRING,
}RESERVED_KEYWORDS;


/**
 * @enum all tokens
 * */
typedef enum{
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
    //Others
    TOKEN_TYPE_OPENING_CLASSIC_BRACKET,
    TOKEN_TYPE_CLOSING_CLASSIC_BRACKET,
    TOKEN_TYPE_OPENING_CURVY_BRACKET,
    TOKEN_TYPE_CLOSING_CURVY_BRACKET,//TODO possible boxy bracket
    TOKEN_TYPE_COMMA,
    TOKEN_TYPE_SEMICOLON,

    TOKEN_TYPE_EOF,
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_STRING,
    TOKEN_TYPE_INTEGER,
    TOKEN_TYPE_FLOAT64,
    TOKEN_TYPE_RESERVED_KEYWORD,
}TOKEN_TYPES;


/**
 * @enum states of the scanner
 * */
typedef enum{

}FSM_STATES;

/*
####################################################################################################################
###########################   Function headers   ###################################################################################
####################################################################################################################
*/





#endif