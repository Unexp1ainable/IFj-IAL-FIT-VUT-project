#include "expression.h"
#include "symtable.h"
#include "syntax.h"
#include "symtable_list.h"
#include "newscanner.h"





int main()
{
    // initialisation of the structures
    dynamic_string_init(&stringbuffer);

    symtableList symlist;
    sym_list_init(&symlist);

    Symtable global;
    symtable_init(&global);
    initialise_predefined(&global);
    sym_list_add(&symlist, &global);

    init_token_list(&tokens);

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
    TermType type;
    int r_code = StartExpr(symlist, &type);
    describe_error(r_code);

    // free all resources
    free(stringbuffer.string);
    sym_list_dispose(&symlist);
    free_token_list(&tokens);

    return 0;
}
