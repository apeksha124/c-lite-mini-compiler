/*
* Compiler Design Project 3 : Semantic Analyser
*
* File        : symboltable.h
*/


#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <stdlib.h>

#define MAX_SYMBOLS 100

typedef struct entry_t {
    char* lexeme;
    int data_type;
    int* parameter_list;
    int num_params;
} entry_t;

extern entry_t symbol_table[MAX_SYMBOLS];
extern int symbol_count;
extern int current_scope;

entry_t* insert_symbol(char* name, int data_type, int is_function);
void fill_parameter_list(entry_t* entry, int* list, int n);
void print_symbol_table(void);
int create_new_scope(void);

#endif

