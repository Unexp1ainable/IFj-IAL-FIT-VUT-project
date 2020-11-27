
#include "newscanner.h"

int get_next_tokenm(TOKEN *token){
    static int a = 0;
    token->tokentype = TOKEN_TYPE_INTEGER;
    token->integer = a;
    a++;
    return 0;
}
