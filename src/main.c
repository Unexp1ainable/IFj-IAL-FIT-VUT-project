#include "scanner.h"
#include "dynamic_string.h"

int main() {
    list *l = scann();


    freeList(l);
    return 0;
}
