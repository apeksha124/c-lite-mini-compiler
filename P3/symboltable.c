#include "symboltable.h"
#include <stdio.h>
#include <string.h>

entry_t symbol_table[MAX_SYMBOLS];
int symbol_count = 0;
int current_scope = 0;

entry_t* insert_symbol(char* name, int data_type, int is_function) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].lexeme, name) == 0) {
            return NULL; // already exists
        }
    }
    entry_t* new_entry = &symbol_table[symbol_count++];
    new_entry->lexeme = strdup(name);
    new_entry->data_type = data_type;
    new_entry->num_params = 0;
    new_entry->parameter_list = NULL;
    return new_entry;
}

void fill_parameter_list(entry_t* entry, int* list, int n) {
    entry->num_params = n;
    entry->parameter_list = list;
}

void print_symbol_table(void) {
    printf("\nSymbol Table:\n");
    printf("%-20s %-10s %-10s\n", "Name", "Type", "#Params");
    printf("-------------------------------------------\n");
    for (int i = 0; i < symbol_count; i++) {
        printf("%-20s %-10d %-10d\n", symbol_table[i].lexeme, symbol_table[i].data_type, symbol_table[i].num_params);
    }
}

int create_new_scope(void) {
    return 0; // dummy scope id
}
