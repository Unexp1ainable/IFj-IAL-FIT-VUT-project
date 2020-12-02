#include "../src/symtable.h"
#include <stdio.h>
#include <string.h>

void printboolean(bool boolean)
{
    if (boolean)
    {
        printf("boolean is true");
    }
    else
    {
        printf("boolean is false");
    }
}

int main()
{
    Symtable table;
    symtable_init(&table);
    for (int i = 0; i < MAX_SIZE_OF_SYM_TABLE; i++)
    {
        if (table[i] != NULL)
        {
            printf("NOT noice\n");
        }
    }
    bool noerror = false;

    Symtable_item *firstitem = symtable_add(&table, "Timotej", &noerror);

    if (noerror != true)
    {
        printf("error! failed memory allocation.\n");
        return 0;
    }
    if (firstitem == NULL)
    {
        printf("error! shouldnt be null pointer.\n");
    }
    printf("%s\n", firstitem->key);
    if (table[hash("Timotej")] == NULL)
    {
        printf("not saved in table.\n");
    }
    else if (table[hash("Timotej") != firstitem])
    {
        printf("incorrect save in table");
    }
    else
    {
        printf("all correct.\n");
    }
    for (unsigned long i = 0; i < MAX_SIZE_OF_SYM_TABLE; i++)
    {
        if (table[i] != NULL)
        {
            if (i == hash("Timotej"))
            {
                printf("found it.\n");
            }
            else
            {
                printf("incorrect write into table.\n");
            }
        }
    }
    symtable_remove(&table, "Timotej");
    for (int i = 0; i < MAX_SIZE_OF_SYM_TABLE; i++)
    {
        if (table[i] != NULL)
        {
            printf("NOT noice\n");
        }
    }

    Symtable_item *seconditem = symtable_add(&table, "Matej", &noerror);
    if (noerror != true)
    {
        printf("error! failed memory allocation.\n");
        return 0;
    }
    if (firstitem == NULL)
    {
        printf("error! shouldnt be null pointer.\n");
    }
    if (table[hash("Matej")] == NULL)
    {
        printf("not saved in table.\n");
    }
    else if (table[hash("Matej") != firstitem])
    {
        printf("incorrect save in table");
    }
    else
    {
        printf("all correct.\n");
    }
    for (unsigned long i = 0; i < MAX_SIZE_OF_SYM_TABLE; i++)
    {
        if (table[i] != NULL)
        {
            if (i == hash("Matej"))
            {
                printf("found it.\n");
            }
            else
            {
                printf("incorrect write into table.\n");
            }
        }
    }
    symtable_free(&table);
    for (int i = 0; i < MAX_SIZE_OF_SYM_TABLE; i++)
    {
        if (table[i] != NULL)
        {
            printf("NOT noice\n");
        }
    }
    Symtable_item *thirditem = symtable_add(&table, "Lukas", &noerror);
    Symtable_item *founditem = symtable_search(&table, "Lukas");
    if (founditem != thirditem)
    {
        printf("they do not equal");
    }
    printf("%s\n", founditem->key);
    symtable_remove(&table, "Lukas");
    //to this point, checked with valgrind, all cleared
    symtable_init(&table);
    symtable_add(&table,"timotej",&noerror);
    symtable_add(&table,"Matej"  ,&noerror);
    symtable_add(&table,"Lukas"  ,&noerror);
    symtable_add(&table,"Samo"   ,&noerror);
    symtable_add(&table,"Jirka"  ,&noerror);
    symtable_add(&table,"Michal" ,&noerror);
    symtable_add(&table,"Anton"  ,&noerror);
    symtable_add(&table,"Maria"  ,&noerror);
    symtable_add(&table,"timotej",&noerror);
    printf("\nall added:\n");
    printouttable(&table);

    symtable_remove(&table,"Anton");
    printf("\nanton removed:\n\n");
    printouttable(&table);

    printf("\nAnton added.\n\n");
    symtable_add(&table,"Anton",&noerror);
    printouttable(&table);

    symtable_remove(&table,"Matej");
    printf("Matej removed.\n");
    printouttable(&table);

    symtable_add(&table,"Matej",&noerror);
    printf("\nMatej added.\n\n");
    printouttable(&table);
    //symtable_free(&table);
    symtable_remove(&table, "timotej");
    symtable_remove(&table, "Matej");
    symtable_remove(&table, "Lukas");
    symtable_remove(&table, "Samo");
    symtable_remove(&table, "Jirka");
    symtable_remove(&table, "Michal");
    symtable_remove(&table, "Anton");
    symtable_remove(&table, "Maria");
    symtable_remove(&table, "timotej");
    printf("\nfree all worked like a charm, so tried to remove all manually:\n\n");
    printouttable(&table);
    printf("#######################    DONE    ##############################################################################################################################################################################\n");
    return 0;
}
