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
        if(isdigit(c)){
            char*digitword;
            digitword = (char*)malloc(MAXLEN * (sizeof(char)));
            if(!digitword){
                return 99;
            }
            int lenght = 0;
            int multiplierdig;
            while(isdigit(c)){
                digitword[lenght] = c;
                if(MAXLEN % lenght  == 0){                
                    char *digitword = realloc(digitword, multiplierdig*MAXLEN*sizeof(char));
                    if(!digitword){
                        return 99;
                    }
                    multiplierdig++;
                }
                c=getchar();
                if(isalpha(c)){             //if it starts with number it cant continue with letter
                    return 51;          
                }
                lenght++;
            }
                Word2Token(digitword,current);
                free(digitword);
        }
        
        if (isalpha(c)){
            //set string for words
            char *wordstring;
            wordstring = (char*)malloc(MAXLEN * (sizeof(char)));
            if(!wordstring){
                return 99;
            }
            memset(wordstring, 0, MAXLEN*(sizeof(char)));       //set 0 to our string
            int wordlenght = 0;                                 
            int multiplier = 2;                                    //for growing sizeof malloc
            while(isalpha(c) || isdigit(c) || c == '_'){
                wordstring[wordlenght] = c;
                if(MAXLEN % wordlenght  == 0){                      //if the string is full
                    char *wordstring = realloc(wordstring, multiplier*MAXLEN*sizeof(char));
                    if(!wordstring){
                        return 99;
                    }
                    multiplier++;
                }
                c = gechar();
                wordlenght++;
            }
                Word2Token(wordstring,current);
                free(wordstring);
      }

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
        } else {

        }
        // TODO

    } while (c != EOF);

    return retval;
}

list *word2token(char *word, list *current) {
    if (strcmp(word, "") == 0)
        return NULL;

    token_type_t t;
    if(CheckKeyword(word, current) == true){
        t=keyword;
    }
    //check if digits only

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
//NUTNO DODĚLAT PRO KAŽDÝ KEYWORD  
bool CheckKeyword(char *word, list *current){
    if(strcmp(word, 'if')){
        current->token_p->Keyword_type=WORD_IF;
        return true;
    }
    else if(strcmp(word, 'else')){
        current->token_p->Keyword_type=WORD_ELSE;
        return true;
    }
    else if(strcmp(word, 'func')){
        current->token_p->Keyword_type=WORD_FUNC;
        return true;
    }
    else if(strcmp(word, 'float64')){
        current->token_p->Keyword_type=WORD_FLOAT64;
        return true;
    }
    else if(strcmp(word, 'int')){
        current->token_p->Keyword_type=WORD_INT;
        return true;
    }
    else if(strcmp(word, 'return')){
        current->token_p->Keyword_type=WORD_RETURN;
        return true;
    }
    else if(strcmp(word, 'print')){
        current->token_p->Keyword_type=WORD_PRINT;
        return true;
    }
    else return false;


}

bool DigitOnly(char *word, list* current){

}
