/**
 * @file newscanner.c
 * @author Timotej Kamensky, xkamen24,+421 944 687 328
 * @brief implementation of scanner for the compiler
 * @date 25.11.2020
 * */
#include "newscanner.h"
#include <stdio.h>
#define WTF 69
#define exitus 12
/***********************************************************************************************************
 * *********************************************************************************************************
 * *********    these variables are used globally inside the function get_next_token    ********************
************************************************************************************************************/
static int eol_value;//EOL char is: 0 = forbidden,1 = optional,2 = required
static int eol_valid;//do i care about EOL? 0 = false, 1 = true
static int fsm_state;//state of the finite state machine
static bool isbuff;//is letter in the buff?
static int buffedchar;//what letter is in the buff?
FILE * fileptr;//TODO assign stdin or something
Dynamic_string stringbuffer;
//line,nextline, loadedchar, line_pos?
/****************************    variable end    ***********************************************************/
void set_fsm_state(FSM_STATES input){
    fsm_state = input;
}

int get_next_token(TOKEN * tokenptr){
    int c = 0;

    if (tokenptr == NULL){
        return WTF;
    }
    //dokoncenie predosleho cyklu+podmienky
    //proper case
    while(1){
        c = fgetc(fileptr);
        switch(fsm_state){
            case FSM_START:
                if(isspace(c) && c != '\n')                                         {continue;}/*isspace covers a lot more*/
                else if (c == '\n')             {if(eol_value == 0){return WTF;}else{if (eol_value == 2){eol_value = 1;}}/*else ignore*/}
                else if (c == '/')              {/*poss.comm. after curly bracket*/ {fsm_state = FSM_SLASH;}}
                else if (c == "\"")             {if(eol_value == 2){return WTF;}else{fsm_state = FSM_STRING;}}
                else if (isalpha(c) || c == '_'){if(eol_value == 2){return WTF;}else{fsm_state = FSM_ID;dynamic_string_add_char(&stringbuffer,c);}}
                else if (c == '=')              {if(eol_value == 2){return WTF;}else{fsm_state = FSM_FIRST_EQUAL;}}
                else if (c == '+')              {if(eol_value == 2){return WTF;}else{maketoken();/*+*/}}
                else if (c == ':')              {if(eol_value == 2){return WTF;}else{fsm_state = FSM_COLON;}}
                else if (c == ';')              {if(eol_value == 2){return WTF;}else{maketoken();}}
                else if (c == '-')              {if(eol_value == 2){return WTF;}else{maketoken();}}
                else if (c == '*')              {if(eol_value == 2){return WTF;}else{maketoken();}}
                else if (c == '(')              {if(eol_value == 2){return WTF;}else{return maketoken();/*opening bracket*/}}
                else if (c == ')')              {if(eol_value == 2){return WTF;}else{return maketoken();/*closing bracket*/}}
                else if (c == '{')              {if(eol_value == 2){return WTF;}else{eol_value = 2; return maketoken();}}
                else if (c == '}')              {if(eol_value == 2){return WTF;}else{               return maketoken();}}
                else if (c == '!')              {if(eol_value == 2){return WTF;}else{fsm_state = FSM_EXCLAMATION;}}
                else if (isdigit(c))            {if(eol_value == 2){return WTF;}else{fsm_state = FSM_NUMBER;dynamic_string_add_char(&stringbuffer,c);}}
                else if (c == '<')              {if(eol_value == 2){return WTF;}else{fsm_state = FSM_SMALLERTHAN;}}
                else if (c == '>')              {if(eol_value == 2){return WTF;}else{fsm_state = FSM_GREATERTHAN;}}
                else if (c == ',')              {if(eol_value == 2){return WTF;}else{maketoken();}}
                else if (c == EOF)              {if(eol_value == 2){return WTF;}else{maketoken();}}
                else                            {if(eol_value == 2){return WTF;}else{return WTF ;}}

                break;


            case FSM_SLASH:
                if( c == '/')                   {fsm_state = FSM_LINE_COMMENT_PROPER;}
                else if (c == '*')              {fsm_state = FSM_BLOCK_COMMENT_PROPER;}
                else{//did not go to any of the 
                    if (eol_value == 2){return WTF;}//cuz comments after opening curly brackets are possible
                    else{
                        buffedchar = c;
                        isbuff = true;
                        maketoken();/*TODO division token*/
                        return exitus;
                    }
                }
                break;
            case FSM_LINE_COMMENT_PROPER:
                if (c == EOF || c == '\n'){
                    buffedchar = c;//TODO? naozaj vratit newline token aj ked je komentar na konci riadku?
                    isbuff = true;
                    fsm_state = FSM_START;
                }
                break;
            case FSM_BLOCK_COMMENT_PROPER:
                if (c == '*'){
                    fsm_state = FSM_BLOCK_COMMENT_END;
                }
                if ()
                break;
            case FSM_BLOCK_COMMENT_END:
                if (c == '/'){
                    fsm_state = FSM_START;
                }
                else{
                    fsm_state = FSM_BLOCK_COMMENT_PROPER;
                }
                break;



            case FSM_ID:
                if (isalnum(c) || c == '_'){
                    dynamic_string_add_char(&stringbuffer, c);
                }
                else{
                    isbuff = true;
                    buffedchar = c;
                    int tmp = maketoken();//id
                    dynamic_string_delete(&stringbuffer);
                    return tmp; 
                }
                break;

            case FSM_STRING:
                if (c != '\"'){
                    dynamic_string_add_char(&stringbuffer,c);
                }
                else{
                    return maketoken();//string
                }
                break;



            case FSM_FIRST_EQUAL:
                if (c == '='){
                    return maketoken();//is equal
                }
                else{
                    isbuff = true;
                    buffedchar = c;
                    fsm_state = FSM_START;
                }
                break;
            case FSM_FIRST_MINUS:
                if (c == '-'){
                    return maketoken();//sub1
                }
                else{
                    isbuff = true;
                    buffedchar = c;
                    fsm_state = FSM_START;
                }
                break;

            case FSM_FIRST_PLUS:
                if (c == '+'){
                    return maketoken();//add1
                }
                else {
                    isbuff = true;
                    buffedchar = c;
                    fsm_state = FSM_START;
                }
                break;
            case FSM_COLON:
                if (c == '='){
                    return maketoken();//define and assign
                }
                else{
                    return WTF;
                }
                break;
        }
        
        
    }



    return 0;
}
/*
####################################################################################################################
###########################   Additional functions   ###############################################################
####################################################################################################################
*/
