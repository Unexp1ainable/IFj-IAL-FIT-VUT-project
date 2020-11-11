#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLEN 64 // maximal lenght of indentificator

enum token_type_t {operator_, identificator, keyword, eol}
        typedef token_type_t;

struct token {
    enum token_type_t type;
    int id;
} token;

// linced list of tocents
struct list {
    struct token *token_p;
    struct list  *next_p;
} typedef list;

// scanns code forom stdin and parses it to tocens
// if error scanning (not anought heep memory, ...) then returns NULL
// if no tokens then returns empty list
list *scann();

// calls addToken
list *word2token(char *word, list *current);

// calls _add2list
list *addToken(list *current, token_type_t type, int id);

list *add2list(list *current);


void freeList(list *start);
