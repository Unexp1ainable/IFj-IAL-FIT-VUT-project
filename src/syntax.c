#include "syntax.h"
#include "mocks.h"

TOKEN token_buffer = {TOKEN_TYPE_EMPTY};

int get_token(TOKEN *token)
{
    if (token_buffer.tokentype == TOKEN_TYPE_EMPTY)
    {
        return get_next_tokenm(token);//TODO mock function is here
    }
    else
    {
        memcpy(token, &token_buffer, sizeof(TOKEN));
        token_buffer.tokentype = TOKEN_TYPE_EMPTY;
        return 0;
    }
}

void return_token(TOKEN token)
{
    memcpy(&token_buffer, &token, sizeof(TOKEN));
}
