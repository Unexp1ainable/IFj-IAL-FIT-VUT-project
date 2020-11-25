#include <stdio.h>
#include "../src/table_hierarchy.h"


int main(){
    SymtableStack thestack;
    bool tmpboolean = stackInit(&thestack);
    int counttheerrors = 0;
    if (thestack.size != DEFAULT_STACK_SIZE){
        printf("invalid stack size. Instead equals %d\n",thestack.size);
        counttheerrors++;
    }
    for (int i = 0;i<DEFAULT_STACK_SIZE;i++){
        if (thestack.table[i] != NULL){
            printf("invalid stack value!on position no. %d.\n",i);
            counttheerrors++;
        }
    }
    
    stackFree(&thestack);
    
    return 0;
}