#include <stdio.h>

#include "scanner.h"

int main() {
    list_t *lp = scan();
    token_t *tp;
    while ((tp = ll_read(lp)) != NULL) {
        printf("%i, %i/n", (int) tp->type, tp->id);
    }

    ll_free(lp);
    return 0;
}