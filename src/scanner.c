#include "scanner.h"



// scanns code forom stdin and parses it to tocens
list *scann() {
    list *retval = (list *) malloc(sizeof(list));
    if (retval == NULL)
        return NULL;
    list *current = retval;

    int c; // scanned char's asci code (or EOF constant)
    char word[64]; // scanned word containing series of scaned c
    int i = 0; // index in the word
    do {
        c = getchar();

        if (c == ' ' || c == '\n' || c == EOF) {
            word[i] = '\0';
            current = word2token(word, current);
            if (current == NULL) {
                freeList(current);
                return NULL;
            }

            if (c == '\n') {
                current = addToken(current, eol, 0);

                if (current == NULL) {
                    freeList(current);
                    return NULL;
                }
            }

        } //else if ()
        // TODO

    } while (c != EOF);

    return retval;
}

list *word2token(char *word, list *current) {
    if (strcmp(word, "") == 0)
        return NULL;

    token_type_t t;
    /* recognize toke */
    // works with hash table
    // TODD - create hahs table

    /* test */
    t = identificator;
    static int id = 1;
    /* add token */
    current = addToken(current, t, id++);

    return current;
}

list *addToken(list *current, token_type_t type, int id) {
    current->token_p = (struct token *) malloc(sizeof(token));
    if (current->next_p == NULL)
        return NULL;

    current->token_p->type = type;
    current->token_p->id   = id;

    current = add2list(current);
    return current; // can be NULL also
}

list *add2list(list *current) {
    return current->next_p = (list *) malloc(sizeof(list));
}


void freeList(list *start) {
    list *current = start;

    while (current != NULL) {
        if (current->token_p != NULL)
            free(current->token_p);

        current = current->next_p;
        free(start);
        start = current;
    }
}
