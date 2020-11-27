/**
 * @file newscanner.c
 * @author Timotej Kamensky, xkamen24,+421 944 687 328
 * @brief implementation of scanner for the compiler
 * @date 25.11.2020
 * */
#include "newscanner.h"
#include <stdio.h>
#include <ctype.h>
#define OK  0
#define WTF 69
#define exitus 12
#define memoryerror 42
/***********************************************************************************************************
 * *********************************************************************************************************
 * *********    these variables are used globally inside the function get_next_token    ********************
************************************************************************************************************/
static int fsm_state;//state of the finite state machine
static bool isbuff;//is letter in the buff?
static int buffedchar;//what letter is in the buff?
FILE * fileptr;//TODO assign stdin or something
Dynamic_string stringbuffer;
//line,nextline, loadedchar, line_pos?
/****************************    variable end    ***********************************************************/


bool dynamic_string_copy(dynamic_string *dynamicstring, TOKEN *token) {
    token->string = (char *) malloc(dynamicstring->actual_size + 1);
    if (token->string == NULL)
        return false;

    strcpy(token->string, dynamicstring->string);
    return true;
}

int get_next_token(TOKEN * tokenptr){
    int c = 0;
    fsm_state = FSM_START;
    if (tokenptr == NULL){
        return WTF;
    }
    dynamic_string_delete(&stringbuffer);//can be done on the start of the function, or at EVERY EXIT CONDITION
    //dokoncenie predosleho cyklu+podmienky
    //proper case
    while(1){
        if (isbuff){
            isbuff = false;
            c = buffedchar;
        }
        else{
            c = fgetc(fileptr);
        }
        switch(fsm_state){
            case FSM_START:
                if(isspace(c) && c != '\n')     {continue;}/*isspace covers a lot more*/
                else if (c == '\n')             {tokenptr->tokentype = TOKEN_TYPE_EOL; return OK;}/*else ignore*/
                else if (c == '/')              {fsm_state = FSM_SLASH;}
                else if (c == '\"')             {fsm_state = FSM_STRING;}
                else if (isalpha(c) || c == '_'){fsm_state = FSM_ID;dynamic_string_add_char(&stringbuffer,c);}
                else if (c == '=')              {fsm_state = FSM_EQUALSIGN;}
                else if (c == '+')              {tokenptr->tokentype = TOKEN_TYPE_ADD; return OK;}
                else if (c == ':')              {fsm_state = FSM_COLON;}
                else if (c == ';')              {tokenptr->tokentype = TOKEN_TYPE_SEMICOLON; return OK;}
                else if (c == '-')              {tokenptr->tokentype = TOKEN_TYPE_SUBTRACT; return OK;}
                else if (c == '*')              {tokenptr->tokentype = TOKEN_TYPE_MULTIPLY; return OK;}
                else if (c == '(')              {tokenptr->tokentype = TOKEN_TYPE_OPENING_CLASSIC_BRACKET; return OK;}
                else if (c == ')')              {tokenptr->tokentype = TOKEN_TYPE_CLOSING_CLASSIC_BRACKET; return OK;}
                else if (c == '{')              {tokenptr->tokentype = TOKEN_TYPE_OPENING_CURVY_BRACKET; return OK;}
                else if (c == '}')              {tokenptr->tokentype = TOKEN_TYPE_CLOSING_CURVY_BRACKET; return OK;}
                else if (c == '!')              {fsm_state = FSM_EXCLAMATION;}
                else if (isdigit(c))            {fsm_state = FSM_DECNUMBER;   dynamic_string_add_char(&stringbuffer,c);}
                else if (c == '<')              {fsm_state = FSM_SMALLERTHAN;}
                else if (c == '>')              {fsm_state = FSM_GREATERTHAN;}
                else if (c == ',')              {tokenptr->tokentype = TOKEN_TYPE_COMMA; return OK;}
                else if (c == '_')              {tokenptr->tokentype = TOKEN_TYPE_UNDERSCORE;return OK;}
                else if (c == EOF)              {tokenptr->tokentype = TOKEN_TYPE_EOF; return OK;}
                else                            {return WTF;}

                break;


            case FSM_SLASH:
                if( c == '/')                   {fsm_state = FSM_LINE_COMMENT_PROPER;}
                else if (c == '*')              {fsm_state = FSM_BLOCK_COMMENT_PROPER;}
                else{//did not go to any of the 
                    buffedchar = c;
                    isbuff = true;
                    tokenptr->tokentype = TOKEN_TYPE_DIVIDE;
                    return OK;
                }
                break;


            case FSM_LINE_COMMENT_PROPER:
                if (c == EOF || c == '\n'){
                    buffedchar = c;
                    isbuff = true;
                    fsm_state = FSM_START;
                }
                break;


            case FSM_BLOCK_COMMENT_PROPER:
                if (c == '*'){
                    fsm_state = FSM_BLOCK_COMMENT_END;
                }
                else if (c == EOF){
                    return WTF;
                }
                break;


            case FSM_BLOCK_COMMENT_END:
                if (c == '/'){
                    fsm_state = FSM_START;
                }
                else if (c == '*'){
                    fsm_state = FSM_BLOCK_COMMENT_END;
                }
                else if (c == EOF){
                    return WTF;
                }
                else{
                    fsm_state = FSM_BLOCK_COMMENT_PROPER;
                }
                break;



            case FSM_ID:
                if (isalnum(c) || c == '_'){
                    if(!dynamic_string_add_char(&stringbuffer, c)){return memoryerror;}
                }
                else{
                    isbuff = true;
                    buffedchar = c;
                    if      (!strcmp(stringbuffer.string,"else"))   {tokenptr->keyword = KEYWORD_ELSE;      tokenptr->tokentype = TOKEN_TYPE_RESERVED_KEYWORD;}
                    else if (!strcmp(stringbuffer.string,"float64")){tokenptr->keyword = KEYWORD_FLOAT64;   tokenptr->tokentype = TOKEN_TYPE_RESERVED_KEYWORD;}
                    else if (!strcmp(stringbuffer.string,"for"))    {tokenptr->keyword = KEYWORD_FOR;       tokenptr->tokentype = TOKEN_TYPE_RESERVED_KEYWORD;}
                    else if (!strcmp(stringbuffer.string,"func"))   {tokenptr->keyword = KEYWORD_FUNC;      tokenptr->tokentype = TOKEN_TYPE_RESERVED_KEYWORD;}
                    else if (!strcmp(stringbuffer.string,"if"))     {tokenptr->keyword = KEYWORD_IF;        tokenptr->tokentype = TOKEN_TYPE_RESERVED_KEYWORD;}
                    else if (!strcmp(stringbuffer.string,"int"))    {tokenptr->keyword = KEYWORD_INT;       tokenptr->tokentype = TOKEN_TYPE_RESERVED_KEYWORD;}
                    else if (!strcmp(stringbuffer.string,"package")){tokenptr->keyword = KEYWORD_PACKAGE;   tokenptr->tokentype = TOKEN_TYPE_RESERVED_KEYWORD;}
                    else if (!strcmp(stringbuffer.string,"return")) {tokenptr->keyword = KEYWORD_RETURN;    tokenptr->tokentype = TOKEN_TYPE_RESERVED_KEYWORD;}
                    else if (!strcmp(stringbuffer.string,"string")) {tokenptr->keyword = KEYWORD_STRING;    tokenptr->tokentype = TOKEN_TYPE_RESERVED_KEYWORD;}
                    else if (!strcmp(stringbuffer.string,"main"))   {tokenptr->tokentype = TOKEN_TYPE_MAIN; tokenptr->tokentype = TOKEN_TYPE_RESERVED_KEYWORD;}
                    else{
                        tokenptr->tokentype =TOKEN_TYPE_IDENTIFIER;
                        dynamic_string_copy(tokenptr->string,stringbuffer);
                    }
                    dynamic_string_delete(&stringbuffer);
                    return OK;
                }
                break;


            case FSM_STRING://TODO cely case, nieco chcel robit jirka, a toto je uplne ze nedokoncene
                if (c == '\\'){
                    fsm_state = FSM_BACKSLASH;
                }
                else if (c != '\"'){
                    dynamic_string_add_char(&stringbuffer,c);
                }
                else{
                    tokenptr->tokentype = TOKEN_TYPE_STRING;
                    tokenptr->string    = &stringbuffer;
                    return OK;
                }
                break;


            case FSM_COLON:
                if (c == '='){
                    tokenptr->tokentype = TOKEN_TYPE_DEFINE_AND_ASSIGN;
                    return OK;
                }
                else{
                    return WTF;
                }
                break;


            case FSM_EXCLAMATION:
                if (c== '='){
                    tokenptr->tokentype = TOKEN_TYPE_NOT_EQUAL; return OK;
                }
                else{
                    return WTF;
                }
                break;


            case FSM_EQUALSIGN:
                if (c == '='){
                   tokenptr->tokentype = TOKEN_TYPE_EQUAL; return OK;}
                else{
                    isbuff = true; buffedchar = c;
                    tokenptr->tokentype = TOKEN_TYPE_ASSIGN; return OK;}
                break;


            case FSM_GREATERTHAN:
                if (c == '='){
                    tokenptr->tokentype = TOKEN_TYPE_GREATER_EQUAL; return OK;
                }
                else{
                    isbuff = true; buffedchar = c;
                    tokenptr->tokentype = TOKEN_TYPE_GREATER_THAN; return OK;
                }   
                break;


            case FSM_SMALLERTHAN:
                if (c == '='){
                    tokenptr->tokentype = TOKEN_TYPE_LOWER_EQUAL;return OK;
                }
                else{
                    isbuff = true;
                    buffedchar = c;
                    tokenptr->tokentype = TOKEN_TYPE_LOWER_THAN;return OK;
                }
                break;


            case FSM_BACKSLASH://note:sorted by ascii
                if      (c == 'a') {if(!dynamic_string_add_char(&stringbuffer,'\a')){return memoryerror;}fsm_state = FSM_STRING;}
                else if (c == 'b') {if(!dynamic_string_add_char(&stringbuffer,'\b')){return memoryerror;}fsm_state = FSM_STRING;}
                else if (c == 't') {if(!dynamic_string_add_char(&stringbuffer,'\t')){return memoryerror;}fsm_state = FSM_STRING;}
                else if (c == 'n') {if(!dynamic_string_add_char(&stringbuffer,'\n')){return memoryerror;}fsm_state = FSM_STRING;}
                else if (c == 'v') {if(!dynamic_string_add_char(&stringbuffer,'\v')){return memoryerror;}fsm_state = FSM_STRING;}
                else if (c == 'f') {if(!dynamic_string_add_char(&stringbuffer,'\f')){return memoryerror;}fsm_state = FSM_STRING;}
                else if (c == 'r') {if(!dynamic_string_add_char(&stringbuffer,'\r')){return memoryerror;}fsm_state = FSM_STRING;}
                else if (c == '\\'){if(!dynamic_string_add_char(&stringbuffer,'\\')){return memoryerror;}fsm_state = FSM_STRING;}
                else if (c == '\''){if(!dynamic_string_add_char(&stringbuffer,'\'')){return memoryerror;}fsm_state = FSM_STRING;}
                else if (c == '\"'){if(!dynamic_string_add_char(&stringbuffer,'\"')){return memoryerror;}fsm_state = FSM_STRING;}
                else if (c == 'x') {fsm_state = FSM_HEXNUMBER_1;}
                else{return WTF;}
                break;

                
            case FSM_DECNUMBER://any other first number than zero
                if     (isdigit(c))     {if(!dynamic_string_add_char(&stringbuffer,c)){return memoryerror;}}
                else if(c=='E'||c=='e') {if(!dynamic_string_add_char(&stringbuffer,c)){return memoryerror;}
                                        fsm_state = FSM_DECNUMBER_EXPONENT_OR_SIGN;}
                else if(c == '.')       {if(!dynamic_string_add_char(&stringbuffer,c)){return memoryerror;}
                                        fsm_state = FSM_DECNUMBER_EXP_DEC;}
                else                    {isbuff = true; buffedchar = c;maketoken();/*number processing*/}
                break;


            case FSM_DECNUMBER_ZERO://only point or exponent after first zero allowed
                if     (isdigit(c))     {return WTF;}
                else if(c=='E'||c=='e') {if(!dynamic_string_add_char(&stringbuffer,c)){return memoryerror;}
                                        fsm_state = FSM_DECNUMBER_EXPONENT_OR_SIGN;}
                else if(c == '.')       {if(!dynamic_string_add_char(&stringbuffer,c)){return memoryerror;}
                                        fsm_state = FSM_DECNUMBER_EXP_DEC;}
                else                    {isbuff = true; buffedchar = c;maketoken();/*number processing*/}
                break;


            case FSM_DECNUMBER_EXPONENT_OR_SIGN://got letter E, can get +,-,nums
                if (isdigit(c))         {if(!dynamic_string_add_char(&stringbuffer,c)){return memoryerror;}
                                        fsm_state = FSM_DECNUMBER_EXPONENT_NUMBER;}
                else if (c == '+')      {if(!dynamic_string_add_char(&stringbuffer,c)){return memoryerror;}
                                        fsm_state = FSM_DECNUMBER_EXPONENT_AND_SIGN;}
                else if (c == '-')      {if(!dynamic_string_add_char(&stringbuffer,c)){return memoryerror;}
                                        fsm_state = FSM_DECNUMBER_EXPONENT_AND_SIGN;}
                else                    {return WTF;}
                break;


            case FSM_DECNUMBER_EXPONENT_AND_SIGN://already got e, passed  where +- was possible, so number is required
                if (isdigit(c))         {if(!dynamic_string_add_char(&stringbuffer,c)){return memoryerror;}
                                        fsm_state = FSM_DECNUMBER_EXPONENT_NUMBER;}
                else                    {return WTF;}
                break;


            case FSM_DECNUMBER_EXPONENT_NUMBER://only number in exponent now
                if (isdigit(c))         {if(!dynamic_string_add_char(&stringbuffer,c)){return memoryerror;}}
                else                    {isbuff = true;buffedchar = c; maketoken();}
                break;

            case FSM_DECNUMBER_FLOAT://already well in decimal places, possible exponent or more numbers
                if (isdigit(c))         {if(!dynamic_string_add_char(&stringbuffer,c)){return memoryerror;}}
                else if(c =='e'||c=='E'){if(!dynamic_string_add_char(&stringbuffer,c)){return memoryerror;}
                                        fsm_state = FSM_DECNUMBER_EXPONENT_OR_SIGN;}
                else                    {isbuff = true;buffedchar = c; maketoken();}
                break;


            case FSM_DECNUMBER_EXP_DEC://got point, need at least one number
                if (isdigit(c))         {if(!dynamic_string_add_char(&stringbuffer,c)){return memoryerror;}
                                        fsm_state = FSM_DECNUMBER_FLOAT;}
                else                    {return WTF;}
                break;


            case FSM_HEXNUMBER_1://got \x
                if (isxdigit(c)){fsm_state = FSM_HEXNUMBER_2;}
                buffedchar = c;//shouldnt bother the actual buffer cuz im not setting isbuff to true
                break;


            case FSM_HEXNUMBER_2:
                if (isxdigit(c)){
                    char hexnumber[4];hexnumber[0]='0';hexnumber[1] = 'x';hexnumber[2]=buffedchar;hexnumber[3] = c;
                    char * endptr;
                    long c = strtol(hexnumber,&endptr,16);
                    if (*endptr){return WTF;/*fail*/}
                    else{/*success*//*TODO is this correct format? ma to vyrobit token ci nie?*/
                        {if(!dynamic_string_add_char(&stringbuffer,'0'          )){return memoryerror;}}
                        {if(!dynamic_string_add_char(&stringbuffer,'x'          )){return memoryerror;}}
                        {if(!dynamic_string_add_char(&stringbuffer,buffedchar   )){return memoryerror;}}
                        {if(!dynamic_string_add_char(&stringbuffer,c            )){return memoryerror;}}
                    }
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
void set_fsm_state(FSM_STATES input){
    fsm_state = input;
}