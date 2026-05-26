#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define HASH_TABLE_SIZE 100
#define NUM_TABLES 10

typedef struct entry_t {
    char* lexeme;
    int value;
    int data_type;
    struct entry_t* successor;
} entry_t;

typedef struct {
    entry_t** symbol_table;
    int parent;
} table_t;

typedef struct {
    entry_t* entry;
    int value;
    char* addr;
    char* code;
    int data_type;
} content_t;

extern table_t symbol_table_list[NUM_TABLES];
extern int table_index;
extern int current_scope;

entry_t** create_table(void);
void init_symbol_tables(void);
entry_t* insert(entry_t** table, char* lexeme, int value, int data_type);
entry_t* search_recursive(char* lexeme);
entry_t* search(entry_t** table, char* lexeme);

content_t create_num_content(int val);
content_t lookup_or_create_symbol(char* name);
content_t assign_symbol(char* name, content_t value);
content_t create_binop(int op, content_t left, content_t right);

#endif
