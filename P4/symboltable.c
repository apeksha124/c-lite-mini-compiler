#include "symboltable.h"
int temp_var_counter = 0;

char* new_temp() {
    char* temp = malloc(10);
    sprintf(temp, "t%d", temp_var_counter++);
    return temp;
}


table_t symbol_table_list[NUM_TABLES];
int table_index = 0;
int current_scope = 0;

entry_t** create_table(void) {
    entry_t** table = (entry_t**)calloc(HASH_TABLE_SIZE, sizeof(entry_t*));
    return table;
}

void init_symbol_tables(void) {
    symbol_table_list[0].symbol_table = create_table();
    symbol_table_list[0].parent = -1;
    current_scope = 0;
    table_index = 1;
}

int myhash(char* lexeme) {
    unsigned int hash = 0;
    for (int i = 0; lexeme[i] != '\0'; i++) {
        hash = (hash * 31) + lexeme[i];
    }
    return hash % HASH_TABLE_SIZE;
}

entry_t* search(entry_t** table, char* lexeme) {
    int index = myhash(lexeme);
    entry_t* curr = table[index];
    while (curr) {
        if (strcmp(curr->lexeme, lexeme) == 0) return curr;
        curr = curr->successor;
    }
    return NULL;
}

entry_t* search_recursive(char* lexeme) {
    int idx = current_scope;
    while (idx != -1) {
        entry_t* found = search(symbol_table_list[idx].symbol_table, lexeme);
        if (found) return found;
        idx = symbol_table_list[idx].parent;
    }
    return NULL;
}

entry_t* insert(entry_t** table, char* lexeme, int value, int data_type) {
    int index = myhash(lexeme);
    entry_t* entry = (entry_t*)malloc(sizeof(entry_t));
    entry->lexeme = strdup(lexeme);
    entry->value = value;
    entry->data_type = data_type;
    entry->successor = table[index];
    table[index] = entry;
    return entry;
}

content_t create_num_content(int val) {
    content_t c;
    c.value = val;
    c.data_type = val;
    c.addr = new_temp();
    c.code = malloc(50);
    sprintf(c.code, "%s = %d\n", c.addr, val);
    c.entry = NULL;
    return c;
}


content_t lookup_or_create_symbol(char* name) {
    content_t c;
    entry_t* e = search_recursive(name);
    if (!e) e = insert(symbol_table_list[current_scope].symbol_table, name, 0, 0);
    c.entry = e;
    c.data_type = e->data_type;
    c.value = e->value;
    c.addr = NULL;
    c.code = NULL;
    return c;
}

content_t assign_symbol(char* name, content_t value) {
    content_t c;
    entry_t* e = search(symbol_table_list[current_scope].symbol_table, name);
    if (!e)
        e = insert(symbol_table_list[current_scope].symbol_table, name, value.value, value.data_type);
    e->value = value.value;
    c.entry = e;
    c.data_type = value.data_type;
    c.value = value.value;
    c.addr = NULL;
    c.code = NULL;
    return c;
}

content_t create_binop(int op, content_t left, content_t right) {
    content_t result;
    result.addr = new_temp();
    result.entry = NULL;
    result.value = 0; // actual runtime value not needed for IR
    result.data_type = 0;

    // Select operator symbol
    char op_char;
    switch (op) {
        case '+': op_char = '+'; break;
        case '-': op_char = '-'; break;
        case '*': op_char = '*'; break;
        case '/': op_char = '/'; break;
        default:  op_char = '?'; break;
    }

    // Format the 3-address code instruction
    char instr[100];
    sprintf(instr, "%s = %s %c %s\n",
            result.addr,
            left.addr ? left.addr : "0",
            op_char,
            right.addr ? right.addr : "0");

    // Allocate and combine code
    int len = 0;
    len += (left.code ? strlen(left.code) : 0);
    len += (right.code ? strlen(right.code) : 0);
    len += strlen(instr) + 1;

    result.code = malloc(len);
    result.code[0] = '\0';
    if (left.code) strcat(result.code, left.code);
    if (right.code) strcat(result.code, right.code);
    strcat(result.code, instr);

    return result;
}

