%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symboltable.h"

extern int yylex();
void yyerror(const char* s) { fprintf(stderr, "Error: %s\n", s); }
%}

%union {
    int int_val;
    char* str_val;
    int* param_list;
}

%token <str_val> IDENTIFIER
%token INT FLOAT VOID

%type <int_val> type_specifier parameter
%type <param_list> parameter_list parameter_list_opt

%%

program:
    function_definition_list
    ;

function_definition_list:
    function_definition
    | function_definition_list function_definition
    ;


function_definition:
    type_specifier IDENTIFIER '(' parameter_list_opt ')' compound_statement
    {
        entry_t* new_fun = insert_symbol($2, $1, 0);
        if (new_fun != NULL && $4 != NULL) {
            int count = 0;
            while ($4[count] != -1) count++;
            fill_parameter_list(new_fun, $4, count);
        }
        free($2);
        create_new_scope();
    }
    ;

parameter_list_opt:
    parameter_list { $$ = $1; }
    |              { $$ = NULL; }
    ;

parameter_list:
    parameter
    {
        $$ = malloc(sizeof(int) * 2);
        $$[0] = $1;
        $$[1] = -1;
    }
    | parameter_list ',' parameter
    {
        int count = 0;
        while ($1[count] != -1) count++;
        $$ = realloc($1, sizeof(int) * (count + 2));
        $$[count] = $3;
        $$[count + 1] = -1;
    }
    ;

parameter:
    type_specifier { $$ = $1; }
    ;

type_specifier:
    INT   { $$ = 0; }
    | FLOAT { $$ = 1; }
    | VOID  { $$ = 2; }
    ;

compound_statement:
    '{' '}'
    ;

%%

int main() {
    yyparse();
    print_symbol_table();
    return 0;
}
