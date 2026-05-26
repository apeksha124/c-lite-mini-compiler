/*
* Compiler Design Project 1 : Lexical Analyser
*
* File        : symboltable.h
* Description : This file contains functions related to a hash organised symbol table.
*               The functions implemented are:
*               create_table(), insert(), search, display()
*
*/



#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <stdint.h>

#define HASH_TABLE_SIZE 100

typedef struct entry_s {
    char* lexeme;
    double value;
    int data_type;
    struct entry_s* successor;
} entry_t;

entry_t** create_table(void);

uint32_t hash(char* lexeme);

entry_t* create_entry(char* lexeme, double value, int data_type);

entry_t* search(entry_t** hash_table_ptr, char* lexeme);

entry_t* insert(entry_t** hash_table_ptr, char* lexeme, double value, int data_type);

void display(entry_t** hash_table_ptr);

#endif

