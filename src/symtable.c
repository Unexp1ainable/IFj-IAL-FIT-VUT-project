/**
 * @file symtable.c
 * 
 * @author Timotej Kamenský, xkamen24, +421 944 687 328
 * @author Michal Řezník,  xrezni28, +420731748122
 * @author Jiří Hofírek, xhofir06, +420774883191
 * @author Samuel Repka, xrepka07, +421907587090
 * 
 * @version 0.1
 * @note first draft of symtable implementation
 * @brief implementation of symtable
 * @date 2020-11-20
 */

#include <string.h>
#include <stdlib.h>
#include "symtable.h"
#include <stdio.h>
unsigned long hash(char *str)
{
    unsigned long hash = 0;
    unsigned long x = 0;
    for (char letter = *str; letter != '\0'; letter = *(++str))
    {
        hash = (hash << 4) + letter;
    }
    return hash % MAX_SIZE_OF_SYM_TABLE;
}

void symtable_init(Symtable *symtable)
{
    if (symtable == NULL)
    {
        return;
    }
    for (int i = 0; i < MAX_SIZE_OF_SYM_TABLE; i++)
    { //NULL ALL
        (*symtable)[i] = NULL;
    }
}

Symtable_item *symtable_add(Symtable *table, char *key, bool *noerror)
{
    if (table == NULL || key == NULL)
    { //initial check
        *noerror = false;
        return NULL;
    }
    //make sure symbol is not in the table already
    unsigned long hash_index = hash(key);
    Symtable_item *tmp = (*table)[hash_index];
    while (tmp != NULL)
    {
        if (strcmp(tmp->key, key) == 0)
        {
            *noerror = true;
            return NULL;
        }
        tmp = tmp->next;
    }

    //if not found, make new one
    Symtable_item *new_item = malloc(sizeof(Symtable_item)); //MALLOC
    if (new_item == NULL)
    {
        *noerror = false;
        return NULL;
    }

    new_item->key = (char *)malloc(sizeof(char) * (strlen(key) + 1)); //MALLOC
    if (new_item->key == NULL)
    {
        free(new_item);
        *noerror = false;
        return NULL;
    }

    //TODO FILL IN DATA
    strcpy(new_item->key, key);
    //put in table
    tmp = (*table)[hash_index];
    (*table)[hash_index] = new_item;
    new_item->next = tmp;
    *noerror = true;
    return new_item;
}
Symtable_item *symtable_search(Symtable *table, char *key)
{
    unsigned long hash_index = hash(key);
    Symtable_item *tmp = (*table)[hash_index];
    while (tmp != NULL)
    {
        if (strcmp(tmp->key, key) == 0)
        {
            return tmp;
        }
        tmp = tmp->next;
    }
    return NULL;
}
void symtable_remove(Symtable *table, char *key)
{
    unsigned long hash_index = hash(key);
    Symtable_item *current = (*table)[hash_index];
    Symtable_item *previous = NULL;
    bool found = false;
    while (current != NULL)
    {
        if (strcmp(key, current->key) == 0)
        {
            found = true;
            break;
        }
        previous = current;
        current = current->next;
    }
    if (found == false)
    {
        return;
    }

    //link the other items in table
    if (previous == NULL)
    {                                         //if item was first on hash
        (*table)[hash_index] = current->next; //works even if only one in list on the hash
    }
    else
    { //item was not first in list
        previous->next = current->next;
    }
    //free item
    free(current->key); //FREE
    free(current);      //FREE
    return;
}

void symtable_free(Symtable *table)
{
    Symtable_item *current;
    Symtable_item *next;
    for (int i = 0; i < MAX_SIZE_OF_SYM_TABLE; i++)
    {
        current = (*table)[i];
        while (current != NULL)
        {
            next = current->next; 
            free(current->key);
            free(current);
            current = next;
        }
        (*table)[i] = NULL; //is it really needed?
    }
}
void printouttable(Symtable * table){
    Symtable_item * item;
    for (int i = 0;i<MAX_SIZE_OF_SYM_TABLE;i++){
        int count = 0;
        item = (*table)[i];
        while(item!= NULL){
            printf("->found on hash %d, index %d : %s ...",i,count,item->key);
            item = item->next;
            count++;
        }
        if (count == 0){
            printf("NOTHING at hash %d",i);
        }
        item == NULL;
        printf("\n");
    } 
}