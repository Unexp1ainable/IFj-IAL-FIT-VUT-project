#include <stdio.h>
#include "../src/table_hierarchy.h"
#include "../src/symtable.h"

void printoutstack(SymtableStack * stack){
    printf("started printing the whole stack.\n");
    if (stack->top == -1){
        printf("no table in the stack!.\n");
    }
    for (int i = 0;i< stack->top+1;i++){
        printf("\nTABLE %d: \n",i);
        Symtable * table = stack->table[i];
        printouttable(table);
    }
}
void printoutstackdata(SymtableStack * stack){
    printf("\n\ndata of the stack:\n");
    printf("top  => %d\n",stack->top);
    printf("size => %d\n\n",stack->size);
}

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
        else{
            printf("correct pointer in stack. ");
        }
    }
    printf("\nexpected print.\n");
    printoutstack(&thestack);
    Symtable table1;
    Symtable table2;
    Symtable table3;
    symtable_init(&table3);
    symtable_init(&table1);
    symtable_init(&table2);
    bool noerror;
    symtable_add(&table1,"timotej",&noerror);
    symtable_add(&table1,"Matej"  ,&noerror);
    symtable_add(&table1,"Lukas"  ,&noerror);
    symtable_add(&table1,"Samo"   ,&noerror);
    symtable_add(&table2,"Jirka"  ,&noerror);
    symtable_add(&table2,"Michal" ,&noerror);
    symtable_add(&table2,"Anton"  ,&noerror);
    symtable_add(&table2,"Maria"  ,&noerror);
    symtable_add(&table3,"Lukas"  ,&noerror);
    symtable_add(&table3,"Samo"   ,&noerror);
    symtable_add(&table3,"Jirka"  ,&noerror);
    symtable_add(&table3,"Michal" ,&noerror);



    stackPush(&thestack, &table1);
    printoutstack(&thestack);
    stackPush(&thestack,&table2);
    printoutstack(&thestack);


    printf("\n\nwill pop a table, expect only one table to be there.\n");
    Symtable * popped1 = stackPop(&thestack);
    printoutstack(&thestack);

    printf("\n\n stack should be empty.\n");
    Symtable * popped2 = stackPop(&thestack);
    printoutstack(&thestack);
    printoutstackdata(&thestack);
    Symtable * popped3 = stackPop(&thestack);//popped empty, to be sure
    printoutstack(&thestack);
    printoutstackdata(&thestack);

    printf("\n\n now, there should be both of the tables again, in the same order.\n");
    stackPush(&thestack,popped1);
    stackPush(&thestack, popped2);
    printoutstack(&thestack);
    printoutstackdata(&thestack);
    printf("inserting third table, which does not fit automatically.\n");
    stackPush(&thestack, table3);
    printoutstack(&thestack);
    printoutstackdata(&thestack);
    stackFree(&thestack);
    printf("#######################    DONE    ##############################################################################################################################################################################\n");

    return 0;
}

