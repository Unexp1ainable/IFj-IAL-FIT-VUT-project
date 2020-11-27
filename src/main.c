<<<<<<< HEAD
#include "scanner.h"
#include "dynamic_string.h"

int main() {
    list *l = scann();


    freeList(l);
=======
#include "syntax.h"

int main()
{
    TOKEN t;
    int i = get_token(&t);

    printf("%i, %li\n", t.tokentype, t.integer);

    return_token(t);
    i = get_token(&t);
    printf("%i, %li\n", t.tokentype, t.integer);

    i = get_token(&t);
    printf("%i, %li\n", t.tokentype, t.integer);

    i = get_token(&t);
    printf("%i, %li\n", t.tokentype, t.integer);
    return_token(t);

    i = get_token(&t);
    printf("%i, %li\n", t.tokentype, t.integer);

>>>>>>> syntax
    return 0;
}
