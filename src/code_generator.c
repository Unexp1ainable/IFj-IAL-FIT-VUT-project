/**
 * @file code_generator.c
 * 
 * @brief implementaiton of functions for code generation
 * 
 * @date 9.12.2020
 * 
 * @copyright Brno University of Technology, Czech Republic
 * 
 * @author Samuel Repka,     xrepka07, +421 907 587 090
 * @author Michal Reznik,    xrezni28, +420 731 748 122
 * @author Jiri Hofirek,     xhofir06, +420 774 883 191
 * @author Timotej Kamensky  xkamen24, +421 944 687 328
 * 
 * */


//##################################################################################################################################
//##########    BUILT-IN FUNCTIONS    ##############################################################################################
//##################################################################################################################################

/*##########    built in funcitons for string operations    ###########################*/
/**
 * @brief built-in strlen function. Page 10
*/
#define BUILT_IN_FUNC_LEN                       \
"\n\n"                                          \
"\n# built-in function for lenght of a string"  \
"\n LABEL $len"                                 \
"\n PUSHFRAME"                                  \
"\n DEFVAR LF@returnvalue"                      \
"\n STRLEN LF@%returnvalue LF@s"                \
"\n POPFRAME"                                   \
"\n RETURN"


/**
 * @brief built-int function to return ASCII (ordinal)
 * value on a given index in string, else false. Page 10
 * */
#define BUILT_IN_FUNC_ORD                                           \
"\n\n"                                                              \
"\n #built-in function for ASCII value of the given sign in string" \
"\n LABEL $ord"                                                     \
"\n PUSHFRAME"                                                      \
"\n DEFVAR   LF@strlen"                                             \
"\n STRLEN   LF@strlen"                                             \
"\n JUMPIFEQ $ord_end LF@strlen int@0"                              \
"\n DEFVAR   LF@bool1"                                              \
"\n GT       LF@bool1 LF@i LF@strlen "                              \
"\n JUMPIFEQ $ord_end LF@bool1 bool@true"                           \
"\n LT       LF@bool1 LF@i int@0"                                   \
"\n JUMPIFEQ $ord_end LF@bool1 bool@true"                           \
"\n DEFVAR   LF@$return"                                            \
"\n STR2INT  LF@$return LF@return LF@s LF@i"                        \
"\nLABEL     $ord_end "                                             \
"\n POPFRAME"                                                       \
"\n RETURN"

/**
 * @brief built-in function to return single-char
 * @note TODO v tejto funkcii aj v dalsich mam vratit hodnotu 1 ako chybu. AKO?
 * */
#define BUILT_IN_FUNC_CHR                                           \
"\n\n"                                                              \
"\n #built-in function to return sign of the given ascii value"     \
"\n LABEL   $chr"                                                   \
"\n PUSHFRAME"                                                      \
"\n DEFVAR   LF@returnvalue"                                        \
"\n MOVE     LF@returnvalue string@"                                \
"\n DEFVAR   LF@bool1"                                              \
"\n DEFVAR   LF@bool2"                                              \
"\n LT       LF@bool1 LF@i  int@256"                                \
"\n GT       LF@bool2 LF@i  int@-1"                                 \
"\n AND      LF@bool1 LF@bool1 LF@bool2"                            \
"\n JUMPIFNEQ $chr_end LF@bool1 bool@true"                          \
"\n INT2CHAR LF@returnvalue LF@i"                                   \
"\n LABEL    $chr_end"                                              \
"\n POPFRAME"                                                       \
"\n RETURN"

/**
 * @brief built in substr function. Page 10
 * */
#define BUILT_IN_FUNC_SUBSTR                                                    \
"\nLABEL   $substr "                                                            \
"\n PUSHFRAME "                                                                 \
"\n DEFVAR LF@str_len"                                                          \
"\n DEFVAR LF@str "                                                             \
"\n DEFVAR LF@newstring"                                                        \
"\n MOVE   LF@str         string@ "                                             \
"\n STRLEN LF@str_len     lf@s"                                                 \
"\n DEFVAR LF@bool1  "                                                          \
"\n JUMPIFEQ $substr_end LF@strlen int@0"     /*nullchecks*/                    \
"\n JUMPIFEQ $substr_end LF@n int@0"                                            \
"\n "                                                                           \
"\n GT     LF@i           LF@str_len "                                          \
"\n JUMPIFEQ $substr_end  LF@bool1 bool@true "   /*if i>strlen   */             \
"\n DEFVAR LF@total_len"                                                        \
"\n MOVE   LF@total_len   int@0 "                                               \
"\n ADD    LF@total_len   LF@s LF@i"                                            \
"\n GT     LF@bool1       LF@total_len   LF@str_len "                           \
"\n JUMPIFEQ $substr_long LF@bool1 bool@true" /*if i+n>strlen */                \
"\n JUMP   $substr_short"                                                       \
"\nLABEL   $substr_long"                                                        \
"\n DEFVAR LF@newstr_len "                                                      \
"\n MOVE   LF@newstr_len  LF@s "                                                \
"\n SUB    LF@total_len   LF@total_len  LF@str_len "                            \
"\n SUB    LF@newstr_len  LF@newstr_len LF@total_len "/*new remaining lenght*/  \
"\n JUMP   $substr_long_short "                                                 \
"\nLABEL   $substr_short  "                                                     \
"\n DEFVAR LF@newstr_len "                                                      \
"\n MOVE   LF@newstr_len  LF@s "                    /*or original lenght*/      \
"\nLABEL   $substr_long_short  "                   /*prepare while cycle*/      \
"\n DEFVAR LF@letter "                                                          \
"\n DEFVAR LF@index "                                                           \
"\n MOVE   LF@index LF@i"                                                       \
"\nLABEL   $substr_while_start  "                    /*actual while cycle*/     \
"\n GETCHAR LF@letter LF@s LF@index "                                           \
"\n CONCAT LF@newstring LF@newstring LF@letter "                                \
"\n ADD    LF@index LF@index int@1 "                                            \
"\n SUB    LF@newstr_len LF@newstr_len int@1 "                                  \
"\n JUMPIFNEQ $substr_while_start LF@newstr_len int@1"                          \
"\nLABEL   $substr_end "                                                        \
"\n POPFRAME "                                                                  \
"\n RETURN "




/*####    built in functions for reading of literals and print of terms    ###########################*/
#define BUILT_IN_FUNC_INPUTS /*TODO*/
#define BUILT_IN_FUNC_INPUTI /*TODO*/
#define BUILT_IN_FUNC_INPUTF /*TODO*/

/*built-in functions for conversion of number types*/
#define BUILT_IN_FUNC_INT_2_FLOAT /*TODO*/
#define BUILT_IN_FUNC_FLOAT_2_INT /*TODO*/

/*other built-in functions*/
#define BUILT_IN_FUNC_PRINT /*TODO*/
