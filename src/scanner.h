//
// Created by jb on 12.11.20.
//

#ifndef IFJPROJ_SCANNER_H
#define IFJPROJ_SCANNER_H
    #include <stdbool.h>
    #include <stdio.h>
    #include <stdlib.h>

    #include "types_definition.h"


    #define MAXLEN 64 // maximal lenght of indentificator


    // scanns code forom stdin and parses it to tocens
    // if error scanning (not anought heep memory, ...) then returns NULL
    // if no tokens then returns empty list
    list_t *scan();

    // calls addToken
    token_t *word2token(char *word);
#endif //IFJPROJ_SCANNER_H
