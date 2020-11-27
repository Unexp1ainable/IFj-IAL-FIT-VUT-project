#include "scanner.h"
#include "dynamic_string.h"

int main() {
    TOKEN *tp;
    int retval;
    while ((retval = get_next_token()) != WTF)
        printf("%i\n", tp->tokentype);

    if (retval == WTF)
        return WTF;

    return 0;
}
