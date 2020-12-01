#include "syntax.h"

int main()
{
    dynamic_string_init(&stringbuffer);

    TOKEN t = {TOKEN_TYPE_EMPTY, {0}};

    int r_code = s_prolog();
    describe_error(r_code);

    free(stringbuffer.string);

    return 0;
}
