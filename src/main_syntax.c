#include "syntax.h"
#include "symtable.h"
#include "symtable_list.h"

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
    // first pass - register all function fingerprints

    do
    {
        get_token(&curr_token);
        if (curr_token.tokentype == TOKEN_TYPE_RESERVED_KEYWORD)
        {
            if (curr_token.keyword == KEYWORD_FUNC)
            {
                int r_code = s_func(symlist);
                if (r_code)
                {
                    return r_code;
                }
            }
        }
    } while (curr_token.tokentype != TOKEN_TYPE_EOF);

    first_pass = false;

    // check if main() was defined
    // TODO main_defined is redundant
    if (!main_defined)
    {
        return ERR_MAIN_MISSING;
    }
    else
    {
        Symtable_item *main_f = was_it_defined(symlist, "main");
        if (main_f->itemData.funcitemptr->used_param != 0 || main_f->itemData.funcitemptr->used_return != 0)
        {
            return ERR_INVALID_MAIN_DEFINITION;
        }
    }

    // second pass
    int r_code = s_prolog(symlist);

    describe_error(r_code);

    // free all resources
    free(stringbuffer.string);
    sym_list_dispose(&symlist);
    free_token_list(&tokens);

    return 0;
}
