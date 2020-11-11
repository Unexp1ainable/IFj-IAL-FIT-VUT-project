// this file contains types definition, data structrs and funinctions needet to work with it

#ifndef JIRKA_SRC_TYPES_DEFINITION_H
#define JIRKA_SRC_TYPES_DEFINITION_H
    enum token_type_t {operator_, identificator, keyword, eol};


    /********!!! id is id in table of symbols !!!********/
    struct token {
        enum token_type_t type;
        int id;
    } typedef token_t;

    struct ll_element {
        struct token       *token_p;
        struct ll_element *next;
    } typedef element_t;

    // the list serves as dynamic queue
    //     (we write only to the end and we read linearly from the start)
    /********!!! output of scanner will be this list !!!********/
    struct linked_list {
        element_t *first;
        element_t *last; // write index
        element_t *read_index;
    } typedef list_t;




    void ll_init(list_t *lp);

    token_t *new_token(enum token_type_t type, int id);

    void elem_free(element_t *ep);

    // adds element to the end of list
    // returns EXIT_SUCCESS on success and EXIT_FAIULURE otherways
    int ll_add(list_t *lp, token_t *tp);

    // reads token from the read_index and stores it to variable tp
    // returns 1 afrer reading the last element 0 otherways
    token_t *ll_read(list_t *lp);

    void ll_free(list_t *lp);

#endif //JIRKA_SRC_TYPES_DEFINITION_H
