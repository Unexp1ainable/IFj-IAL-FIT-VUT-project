#include "pars_string.h"

Dynamic_string *pars_string() {
    Dynamic_string *sp = (Dynamic_string *) malloc(sizeof(Dynamic_string));
    if (sp == NULL)
        return NULL;
    dynamic_string_init(sp);

    while (1) {
        int c = getchar();
        switch (c) {
            case EOF:
                sp->error = 1;
                return sp;
            case (int) '"':
                return sp;
            case (int) '\\':
                backslash(sp);
                if (sp->error != 0)
                    return sp;
                break;
            default:
                dynamic_string_add_char(sp, (char) c);
                break;
        }
    }
}

void backslash(Dynamic_string *sp) {
    int c = getchar();
    switch (c) {
        case EOF:
            sp->error = 1;
            break;
        case (int) 'x': // hex ascii code
            backslash_x(sp);
            break;
        case (int) 'n':
            dynamic_string_add_char(sp, '\n');
            break;
        case (int) '\\':
            dynamic_string_add_char(sp, '\\');
            break;
        case (int) '\"':
            dynamic_string_add_char(sp, '\"');
            break;
        case (int) 't':
            dynamic_string_add_char(sp, '\t');
            break;
        default:
            sp->error = 2;
            break;
    }
}

void backslash_x(Dynamic_string *sp) {
    return;
}