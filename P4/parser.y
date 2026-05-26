%code requires {
    #include "symboltable.h"
}
%{
#include <stdio.h>
#include <stdlib.h>
#include "symboltable.h"

int yylex(void);
int yyerror(const char* s);
%}

%union {
    int ival;
    char* str;
    content_t cont;
}

%token <ival> NUM
%token <str> ID
%token ASSIGN PLUS MINUS MUL DIV
%token NEWLINE
%left PLUS MINUS
%left MUL DIV
%type <cont> expression

%%

program:
      /* empty */
    | program statement NEWLINE
;

statement:
    expression {
        printf("Result = %d\n", $1.data_type);
        printf("Generated IR:\n%s", $1.code);
    }
    | ID ASSIGN expression {
        content_t c = assign_symbol($1, $3); free($1);
        printf("Assigned %d\n", c.data_type);
        printf("Generated IR:\n%s%s = %s\n", $3.code ? $3.code : "", c.entry->lexeme, $3.addr);
    }
;



expression:
      NUM                     { $$ = create_num_content($1); }
    | ID                      { $$ = lookup_or_create_symbol($1); free($1); }
    | expression PLUS expression   { $$ = create_binop('+', $1, $3); }
    | expression MINUS expression  { $$ = create_binop('-', $1, $3); }
    | expression MUL expression    { $$ = create_binop('*', $1, $3); }
    | expression DIV expression    { $$ = create_binop('/', $1, $3); }
;


%%

int main() {
    init_symbol_tables();
    
    yyparse();
    return 0;
}

int yyerror(const char* s) {
    fprintf(stderr, "Parse error: %s\n", s);
    return 1;
}
