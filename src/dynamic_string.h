/**TODO
 * */
#ifndef DYNAMIC_STRING
#define DYNAMIC_STRING

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#define DEFAULT_STRING_LENGTH 30
typedef struct{
    char * string;
    unsigned int actual_size;
    unsigned int allocated_size;
    int error;
}Dynamic_string;

bool dynamic_string_init        (Dynamic_string * dynamicstring);
bool dynamic_string_add_char    (Dynamic_string * dynamicstring, char c);
bool dynamic_string_add_string  (Dynamic_string * dynamicstring, char * word);
bool dynamic_string_double      (Dynamic_string * dynamicstring);
void dynamic_string_free        (Dynamic_string * dynamicstring);
void dynamic_string_delete      (Dynamic_string * dynamicstring);
bool dynamic_string_full        (Dynamic_string * dynamicstring);
void dynamic_string_backspace   (Dynamic_string * dynamicstring);
void dynamic_string_print       (Dynamic_string dynamicstring);

#endif
