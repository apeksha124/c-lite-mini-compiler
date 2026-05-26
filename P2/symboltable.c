#include "symboltable.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

entry_t** create_table()
{
    entry_t** hash_table_ptr = malloc(sizeof(entry_t*) * HASH_TABLE_SIZE);
    if (!hash_table_ptr) return NULL;

    for (int i = 0; i < HASH_TABLE_SIZE; i++)
        hash_table_ptr[i] = NULL;

    return hash_table_ptr;
}

uint32_t hash(char* lexeme)
{
    size_t len = strlen(lexeme);
    uint32_t hash = 0;

    for (size_t i = 0; i < len; i++) {
        hash += lexeme[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash % HASH_TABLE_SIZE;
}

entry_t* create_entry(char* lexeme, double value, int data_type)
{
    entry_t* newentry = malloc(sizeof(entry_t));
    if (!newentry) return NULL;

    newentry->lexeme = strdup(lexeme);
    if (!newentry->lexeme) {
        free(newentry);
        return NULL;
    }

    newentry->value = value;
    newentry->data_type = data_type;
    newentry->successor = NULL;

    return newentry;
}

entry_t* search(entry_t** hash_table_ptr, char* lexeme)
{
    uint32_t idx = hash(lexeme);
    entry_t* current = hash_table_ptr[idx];

    while (current != NULL) {
        if (strcmp(current->lexeme, lexeme) == 0)
            return current;
        current = current->successor;
    }
    return NULL;
}

entry_t* insert(entry_t** hash_table_ptr, char* lexeme, double value, int data_type)

{
    entry_t* found = search(hash_table_ptr, lexeme);
    if (found != NULL)
        return found;

    uint32_t idx = hash(lexeme);
    entry_t* newentry = create_entry(lexeme, value, data_type);

    if (!newentry) {
        fprintf(stderr, "Error: Failed to create new symbol table entry.\n");
        exit(EXIT_FAILURE);
    }

    newentry->successor = hash_table_ptr[idx];
    hash_table_ptr[idx] = newentry;

    return newentry;
}

void display(entry_t** hash_table_ptr)
{
    printf("\n================ Symbol Table ================\n");
    printf("%-20s %-20s %-20s\n", "Lexeme", "Value", "Data Type");
    printf("==============================================\n");

    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        entry_t* current = hash_table_ptr[i];
        while (current != NULL) {
            printf("%-20s %-20g %-20d\n", current->lexeme, current->value, current->data_type);
            current = current->successor;
        }
    }
    printf("==============================================\n");
}
