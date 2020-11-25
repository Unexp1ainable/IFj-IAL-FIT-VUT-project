/**TODO
 * */
#include "dynamic_string.h"
/**
 * actual size means index of the dynamic array
 * */

bool dynamic_string_init(Dynamic_string * dynamicstring){
    dynamicstring->string = calloc(DEFAULT_STRING_LENGTH,sizeof(char));
    if (dynamicstring->string == NULL){
        return false;
    }
    dynamicstring->actual_size = -1;
    dynamicstring->allocated_size = DEFAULT_STRING_LENGTH;
    return true;
}
bool dynamic_string_add_char(Dynamic_string * dynamicstring, char c){
    if (dynamic_string_full(dynamicstring)){
        bool tmpboolean = dynamic_string_double(dynamicstring);
        if (tmpboolean == false){
            return false;
        }
    }
    dynamicstring->string[dynamicstring->actual_size + 1] = c;
    return true;
}
bool dynamic_string_add_string(Dynamic_string * dynamicstring, char * word){
    while(strlen(dynamicstring->string)+ strlen(word) + 1 > dynamicstring->allocated_size){
        bool tmpboolean = dynamic_string_double(dynamicstring);
        if (tmpboolean == false){
            return false;
        }
    }
}
bool dynamic_string_double(Dynamic_string * dynamicstring){
    char * tmp = realloc(dynamicstring->string,dynamicstring->allocated_size * 2 );
    if (tmp != NULL){
        dynamicstring->string = tmp;
        dynamicstring->allocated_size = dynamicstring->allocated_size * 2;
        return true;
    }
    else{
        return false;
    }
}
bool dynamic_string_free(Dynamic_string * dynamicstring){
    free(dynamicstring->string);
    dynamicstring->allocated_size = 0;
    dynamicstring->actual_size = -1;
}
bool dynamic_string_full(Dynamic_string * dynamicstring){
    if (dynamicstring->allocated_size == dynamicstring->actual_size + 1){
        return true;
    }
    else{
        return false;
    }
}
void dynamic_string_backspace   (Dynamic_string * dynamicstring){
    dynamicstring->string[dynamicstring->actual_size] = '\0';
    dynamicstring->actual_size--;
    return;
}