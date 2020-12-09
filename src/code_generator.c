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
 * @brief built-int function to return ordinal value on a given index in string, else false. Page 10
 * */
#define BUILT_IN_FUNC_ORD                       \
""\

/**
 * @brief built-in function to return single-char string of the position
 * */
#define BUILT_IN_FUNC_CHR


/**
 * @brief built in substr function. Page 10
 * */
#define BUILT_IN_FUNC_SUBSTR
/*####    built in functions for reading of literals and print of terms    ###########################*/
#define BUILT_IN_FUNC_INPUTS
#define BUILT_IN_FUNC_INPUTI
#define BUILT_IN_FUNC_INPUTF

/*built-in functions for conversion of number types*/
#define BUILT_IN_FUNC_INT_2_FLOAT
#define BUILT_IN_FUNC_FLOAT_2_INT

/*other built-in functions*/
#define BUILT_IN_FUNC_PRINT
