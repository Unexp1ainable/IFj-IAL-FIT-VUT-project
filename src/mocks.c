#include "newscanner.h"

int get_next_token(TOKEN *token){
    token->tokentype = TOKEN_TYPE_RESERVED_KEYWORD;
    token->keyword = KEYWORD_STRING;
    return 0;
}
