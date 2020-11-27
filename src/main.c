#include "newscanner.h"
#include "dynamic_string.h"

int main() {
    int retval;
    TOKEN t;
    TOKEN *tp = &t;
    while ((retval=get_next_token(tp)) != WTF)
        printf("token: %i\n", (int) tp->tokentype);

    if (retval == WTF)
        return WTF;

    return 0;
}
