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
//works
/**
 * @brief built-in strlen function. Page 10
*/
#define BUILT_IN_FUNC_LEN                          \
    "\n\n"                                         \
    "\n# built-in function for lenght of a string" \
    "\nLABEL len"                                 \
    "\nPUSHFRAME"                                  \
    "\nDEFVAR LF@__ret_000__"                      \
    "\nSTRLEN LF@__ret_000__ LF@s"                \
    "\nPOPFRAME"                                   \
    "\nRETURN"

/**
 * @brief built-int function to return ASCII (ordinal)
 * value on a given index in string, else false. Page 10
 * */
#define BUILT_IN_FUNC_ORD                                              \
    "\n\n"                                                             \
    "\n#built-in function for ASCII value of the given sign in string" \
    "\nLABEL $ord"                                                     \
    "\nPUSHFRAME"                                                      \
    "\nDEFVAR   LF@strlen"                                             \
    "\nSTRLEN   LF@strlen"                                             \
    "\nJUMPIFEQ $ord_end LF@strlen int@0"                              \
    "\nDEFVAR   LF@bool1"                                              \
    "\nGT       LF@bool1 LF@i LF@strlen "                              \
    "\nJUMPIFEQ $ord_end LF@bool1 bool@true"                           \
    "\nLT       LF@bool1 LF@i int@0"                                   \
    "\nJUMPIFEQ $ord_end LF@bool1 bool@true"                           \
    "\nDEFVAR   LF@$return"                                            \
    "\nSTR2INT  LF@$return LF@return LF@s LF@i"                        \
    "\nLABEL     $ord_end "                                            \
    "\nPOPFRAME"                                                       \
    "\nRETURN"

/**
 * @brief built-in function to return single-char
 * @note TODO v tejto funkcii aj v dalsich mam vratit hodnotu 1 ako chybu. AKO?
 * */
#define BUILT_IN_FUNC_CHR                                          \
    "\n\n"                                                         \
    "\n#built-in function to return sign of the given ascii value" \
    "\nLABEL   $chr"                                               \
    "\nPUSHFRAME"                                                  \
    "\nDEFVAR   LF@returnvalue"                                    \
    "\nMOVE     LF@returnvalue string@"                            \
    "\nDEFVAR   LF@bool1"                                          \
    "\nDEFVAR   LF@bool2"                                          \
    "\nLT       LF@bool1 LF@i  int@256"                            \
    "\nGT       LF@bool2 LF@i  int@-1"                             \
    "\nAND      LF@bool1 LF@bool1 LF@bool2"                        \
    "\nJUMPIFNEQ $chr_end LF@bool1 bool@true"                      \
    "\nINT2CHAR LF@returnvalue LF@i"                               \
    "\nLABEL    $chr_end"                                          \
    "\nPOPFRAME"                                                   \
    "\nRETURN"

/**
 * @brief built in substr function. Page 10
 * */
#define BUILT_IN_FUNC_SUBSTR                                                       \
    "\nLABEL   $substr "                                                           \
    "\nPUSHFRAME "                                                                 \
    "\nDEFVAR LF@str_len"                                                          \
    "\nDEFVAR LF@str "                                                             \
    "\nDEFVAR LF@newstring"                                                        \
    "\nMOVE   LF@str         string@ "                                             \
    "\nSTRLEN LF@str_len     lf@s"                                                 \
    "\nDEFVAR LF@bool1  "                                                          \
    "\nJUMPIFEQ $substr_end LF@strlen int@0" /*nullchecks*/                        \
    "\nJUMPIFEQ $substr_end LF@n int@0"                                            \
    "\n"                                                                           \
    "\nGT     LF@i           LF@str_len "                                          \
    "\nJUMPIFEQ $substr_end  LF@bool1 bool@true " /*if i>strlen   */               \
    "\nDEFVAR LF@total_len"                                                        \
    "\nMOVE   LF@total_len   int@0 "                                               \
    "\nADD    LF@total_len   LF@s LF@i"                                            \
    "\nGT     LF@bool1       LF@total_len   LF@str_len "                           \
    "\nJUMPIFEQ $substr_long LF@bool1 bool@true" /*if i+n>strlen */                \
    "\nJUMP   $substr_short"                                                       \
    "\nLABEL   $substr_long"                                                       \
    "\nDEFVAR LF@newstr_len "                                                      \
    "\nMOVE   LF@newstr_len  LF@s "                                                \
    "\nSUB    LF@total_len   LF@total_len  LF@str_len "                            \
    "\nSUB    LF@newstr_len  LF@newstr_len LF@total_len " /*new remaining lenght*/ \
    "\nJUMP   $substr_long_short "                                                 \
    "\nLABEL   $substr_short  "                                                    \
    "\nDEFVAR LF@newstr_len "                                                      \
    "\nMOVE   LF@newstr_len  LF@s "  /*or original lenght*/                        \
    "\nLABEL   $substr_long_short  " /*prepare while cycle*/                       \
    "\nDEFVAR LF@letter "                                                          \
    "\nDEFVAR LF@index "                                                           \
    "\nMOVE   LF@index LF@i"                                                       \
    "\nLABEL   $substr_while_start  " /*actual while cycle*/                       \
    "\nGETCHAR LF@letter LF@s LF@index "                                           \
    "\nCONCAT LF@newstring LF@newstring LF@letter "                                \
    "\nADD    LF@index LF@index int@1 "                                            \
    "\nSUB    LF@newstr_len LF@newstr_len int@1 "                                  \
    "\nJUMPIFNEQ $substr_while_start LF@newstr_len int@1"                          \
    "\nLABEL   $substr_end "                                                       \
    "\nPOPFRAME "                                                                  \
    "\nRETURN "

/*####    built in functions for reading of literals and print of terms    ###########################*/
#define BUILT_IN_FUNC_INPUTS "" /*TODO*/
#define BUILT_IN_FUNC_INPUTI "" /*TODO*/
#define BUILT_IN_FUNC_INPUTF "" /*TODO*/

/*built-in functions for conversion of number types*/
#define BUILT_IN_FUNC_INT_2_FLOAT \
    "\nLABEL int2float"           \
    "\nPUSH FRAME"                \
    "\nDEFVAR LF@__ret_000__"     \
    "\nPUSH LF@i"                 \
    "\nINT2FLOATS"                \
    "\nPOP LF@$__ret_000__"       \
    "\nPOPFRAME"                  \
    "\nRETURN"

#define BUILT_IN_FUNC_FLOAT_2_INT \
    "\nLABEL float2int"           \
    "\nPUSH FRAME"                \
    "\nDEFVAR LF@__ret_000__"     \
    "\nPUSH LF@f"                 \
    "\nFLOAT2INTS"                \
    "\nPOP LF@$__ret_000__"       \
    "\nPOPFRAME"                  \
    "\nRETURN"

/*other built-in functions*/
#define BUILT_IN_FUNC_PRINT                                \
    "\nLABEL print # SUPPORTS UP TO 15 VALUES!!!!!!!!!!!!" \
    "\nPUSHFRAME"                                          \
    "\nDEFVAR LF@__counter__"                              \
    "\nMOVE LF@__counter__ LF@__param_n__"                 \
    "\nJUMPIFEQ __endprint__ int@0 LF@__counter__"         \
    "\nWRITE LF@__par000__"                                \
    "\nSUB LF@__counter__ LF@__counter__ int@1"            \
    "\nJUMPIFEQ __endprint__ int@0 LF@__counter__"         \
    "\nWRITE LF@__par001__"                                \
    "\nSUB LF@__counter__ LF@__counter__ int@1"            \
    "\nJUMPIFEQ __endprint__ int@0 LF@__counter__"         \
    "\nWRITE LF@__par002__"                                \
    "\nSUB LF@__counter__ LF@__counter__ int@1"            \
    "\nJUMPIFEQ __endprint__ int@0 LF@__counter__"         \
    "\nWRITE LF@__par003__"                                \
    "\nSUB LF@__counter__ LF@__counter__ int@1"            \
    "\nJUMPIFEQ __endprint__ int@0 LF@__counter__"         \
    "\nWRITE LF@__par004__"                                \
    "\nSUB LF@__counter__ LF@__counter__ int@1"            \
    "\nJUMPIFEQ __endprint__ int@0 LF@__counter__"         \
    "\nWRITE LF@__par005__"                                \
    "\nSUB LF@__counter__ LF@__counter__ int@1"            \
    "\nJUMPIFEQ __endprint__ int@0 LF@__counter__"         \
    "\nWRITE LF@__par006__"                                \
    "\nSUB LF@__counter__ LF@__counter__ int@1"            \
    "\nJUMPIFEQ __endprint__ int@0 LF@__counter__"         \
    "\nWRITE LF@__par007__"                                \
    "\nSUB LF@__counter__ LF@__counter__ int@1"            \
    "\nJUMPIFEQ __endprint__ int@0 LF@__counter__"         \
    "\nWRITE LF@__par008__"                                \
    "\nSUB LF@__counter__ LF@__counter__ int@1"            \
    "\nJUMPIFEQ __endprint__ int@0 LF@__counter__"         \
    "\nWRITE LF@__par009__"                                \
    "\nSUB LF@__counter__ LF@__counter__ int@1"            \
    "\nJUMPIFEQ __endprint__ int@0 LF@__counter__"         \
    "\nWRITE LF@__par010__"                                \
    "\nSUB LF@__counter__ LF@__counter__ int@1"            \
    "\nJUMPIFEQ __endprint__ int@0 LF@__counter__"         \
    "\nWRITE LF@__par011__"                                \
    "\nSUB LF@__counter__ LF@__counter__ int@1"            \
    "\nJUMPIFEQ __endprint__ int@0 LF@__counter__"         \
    "\nWRITE LF@__par012__"                                \
    "\nSUB LF@__counter__ LF@__counter__ int@1"            \
    "\nJUMPIFEQ __endprint__ int@0 LF@__counter__"         \
    "\nWRITE LF@__par013__"                                \
    "\nSUB LF@__counter__ LF@__counter__ int@1"            \
    "\nJUMPIFEQ __endprint__ int@0 LF@__counter__"         \
    "\nWRITE LF@__par014__"                                \
    "\nSUB LF@__counter__ LF@__counter__ int@1"            \
    "\nLABEL __endprint__"                                 \
    "\nPOPFRAME"                                           \
    "\nRETURN"
