#include "syntax.h"
#include "symtable.h"
#include "symtable_list.h"

int main()
{
    // initialisation of the structures
    dynamic_string_init(&stringbuffer);
    TOKEN t = {TOKEN_TYPE_EMPTY, {0}};

    symtableList symlist;
    sym_list_init(symlist);

    Symtable global;
    symtable_init(global);
    initialise_predefined(global);
    sym_list_add(symlist, global);

    // begin check
    // first pass
    do
    {
        do
        {
            get_token(&curr_token);
            if (curr_token.tokentype == TOKEN_TYPE_RESERVED_KEYWORD)
            {
                if (curr_token.keyword == KEYWORD_FUNC)
                {
                    break;
                }
            }
        } while (curr_token.tokentype != TOKEN_TYPE_EOF);

        int r_code = s_func(symlist);
        if (r_code)
        {
            return r_code;
        }

    } while (curr_token.tokentype != TOKEN_TYPE_EOF);

    first_pass = false;

    int r_code = s_prolog(symlist);
    describe_error(r_code);

    // free all resources
    free(stringbuffer.string);
    sym_list_dispose(symlist);

    return 0;
}
