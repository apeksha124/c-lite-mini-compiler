%code requires {
  #include "symboltable.h"
}
%{
#include <stdio.h>
#include <stdlib.h>
#include "symboltable.h"

extern int yylex();
extern void yyerror(const char* s);

entry_t** symbol_table;

%}

%union {
    int ival;
    entry_t* entry;
}

%token <ival> INT_CONSTANT
%token <entry> IDENTIFIER
%token INT LONG_LONG LONG SHORT SIGNED UNSIGNED
%token FOR WHILE BREAK CONTINUE IF ELSE RETURN
%token EQ NEQ LE GE AND OR

%left '+' '-'
%left '*' '/'
%right '='

/* Precedence for dangling else fix */
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%type <ival> expression

%%

program:
    program statement
    | statement
    ;

statement:
    declaration_statement
    | assignment_statement
    | expression_statement
    | control_statement
    ;

declaration_statement:
    INT IDENTIFIER ';' {
        $2->data_type = 1;
        printf("Declared int variable %s\n", $2->lexeme);
    }
    ;

assignment_statement:
    IDENTIFIER '=' expression ';' {
        $1->value = $3;
        printf("Assigned %d to variable %s\n", $3, $1->lexeme);
    }
    ;

expression_statement:
    expression ';' { /* just evaluate expression */ }
    ;

/* Dangling else resolution: first IF without ELSE has LOWER_THAN_ELSE precedence */
control_statement:
      IF '(' expression ')' statement %prec LOWER_THAN_ELSE
    | IF '(' expression ')' statement ELSE statement
    | WHILE '(' expression ')' statement
    | FOR '(' expression_statement expression_statement expression ')' statement
    ;

expression:
    INT_CONSTANT {
        $$ = $1;
    }
    | IDENTIFIER {
        $$ = $1->value;
    }
    | expression '+' expression {
        $$ = $1 + $3;
    }
    | expression '-' expression {
        $$ = $1 - $3;
    }
    | expression '*' expression {
        $$ = $1 * $3;
    }
    | expression '/' expression {
        if($3 == 0) {
            yyerror("Division by zero");
            $$ = 0;
        } else {
            $$ = $1 / $3;
        }
    }
    | '(' expression ')' {
        $$ = $2;
    }
    ;

%%

void yyerror(const char* s)
{
    fprintf(stderr, "Error: %s\n", s);
}

int main()
{
    symbol_table = create_table();
    printf("Starting parsing...\n");
    yyparse();
    printf("Parsing finished.\n");
    display(symbol_table);
    return 0;
}
