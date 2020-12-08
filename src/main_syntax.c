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

    int r_code = load_tokens(&tokens);

    if (r_code)
    {
        // LEX error
        r_code = ERR_LEX;
        goto cleanup;
    }

    // begin check
    // first pass - register all function fingerprints

    do
    {
        get_token(&curr_token);
        if (curr_token->tokentype == TOKEN_TYPE_RESERVED_KEYWORD)
        {
            if (curr_token->keyword == KEYWORD_FUNC)
            {
                r_code = s_func(symlist);
                if (r_code)
                {
                    // first pass failed
                    goto cleanup;
                }
            }
        }
    } while (curr_token->tokentype != TOKEN_TYPE_EOF);

    // prepare for second pass
    first_pass = false;
    reset_list_position(&tokens);

    Symtable_item *main_f = was_it_defined(symlist, "main");
    // check if main() was defined
    if (!main_f)
    {
        r_code = ERR_MAIN_MISSING;
        goto cleanup;
    }
    else
    {
        // and if it has required parameters and return values (NONE)
        if (main_f->itemData.funcitemptr->used_param != 0 || main_f->itemData.funcitemptr->used_return != 0)
        {
            r_code = ERR_INVALID_MAIN_DEFINITION;
            goto cleanup;
        }
    }

    // second pass
    r_code = s_prolog(symlist);

// deallocate all structures in case of an error
cleanup:

    describe_error(r_code);

    // free all resources
    symtable_free(symlist->table);
    free(symlist);
    free(stringbuffer.string);
    free_token_list(&tokens);

    return map_err_code(r_code);
}
