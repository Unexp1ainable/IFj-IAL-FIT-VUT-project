#include "expression.h"
#include "symtable.h"
#include "syntax.h"
#include "newscanner.h"

int main()
{
    // initialisation of the structures
    dynamic_string_init(&stringbuffer);

    SymtableStack symlist;
    stackInit(&symlist);

    Symtable global;
    symtable_init(&global);
    initialise_predefined(&global);
    stackPush(&symlist, global);

    init_token_list(&tokens);

    load_tokens(&tokens);   // can fail

    TermType type;
    int r_code = StartExpr(symlist, &type);

    printf("Expression evaluation ended, return code: %i\n", r_code);

    // free all resources
    free(stringbuffer.string);
    sym_list_dispose(&symlist);
    free_token_list(&tokens);

    return 0;
}
