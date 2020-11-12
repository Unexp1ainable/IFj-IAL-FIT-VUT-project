//
// Created by jb on 12.11.20.
//

#include "scanner.h"
#include <string.h>


// scanns code forom stdin and parses it to tocens
list_t *scan() {
    list_t *retval = (list_t *) malloc(sizeof(list_t));
    if (retval == NULL)
        return NULL;
    ll_init(retval);

    int c; // scanned char's asci code (or EOF constant)
    char word[64]; // scanned word containing series of scaned c
    int i = 0; // index in the word
    do {
        c = getchar();

        if (c == ' ' || c == '\n' || c == EOF) {
            word[i] = '\0';

            token_t *new_token_p = word2token(word);
            int      error       = ll_add(retval, new_token_p);
            if (new_token_p == NULL || error) {
                ll_free(retval);
                return NULL;
            }

            if (c == '\n') {
                new_token_p = new_token(eol, 0);
                error       = ll_add(retval, new_token_p);
                if (new_token_p == NULL || error) {
                    ll_free(retval);
                    return NULL;
                }
            }

        } //else if ()*/
            // TODO
        else
            word[i] = c;

    } while (c != EOF);

    return retval;
}

token_t *word2token(char *word) {
    if (strcmp(word, "") == 0)
        return NULL;

    enum token_type_t type;
    /* recognize toke */
    // works with hash table
    // TODD - create hahs table

    /* test */
    type = identificator;
    static int id = 1;

    return new_token(type, id++);
}