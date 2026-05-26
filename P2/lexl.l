%option noyywrap
%option yylineno

%{
#include "parser.tab.h"
#include "symboltable.h"

extern entry_t** symbol_table;

void yyerror(const char *s);
%}

letter      [a-zA-Z]
digit       [0-9]
ws          [ \t\r\f\v]+
identifier  (_|{letter})({letter}|{digit}|_)*

%%

"int"         { return INT; }
"long long"   { return LONG_LONG; }
"long"        { return LONG; }
"short"       { return SHORT; }
"signed"      { return SIGNED; }
"unsigned"    { return UNSIGNED; }
"for"         { return FOR; }
"while"       { return WHILE; }
"break"       { return BREAK; }
"continue"    { return CONTINUE; }
"if"          { return IF; }
"else"        { return ELSE; }
"return"      { return RETURN; }

{identifier} {
    yylval.entry = insert(symbol_table, yytext, 0.0, 0); // datatype 0 = unknown
    return IDENTIFIER;
}

{digit}+      {
    yylval.ival = atoi(yytext);
    return INT_CONSTANT;
}

"=="          { return EQ; }
"!="          { return NEQ; }
"<="          { return LE; }
">="          { return GE; }
"&&"          { return AND; }
"||"          { return OR; }

"+"           { return '+'; }
"-"           { return '-'; }
"*"           { return '*'; }
"/"           { return '/'; }
"%"           { return '%'; }
"="           { return '='; }
";"           { return ';'; }
","           { return ','; }
"("           { return '('; }
")"           { return ')'; }
"{"           { return '{'; }
"}"           { return '}'; }
"<"           { return '<'; }
">"           { return '>'; }

\n            { /* ignore newlines, but update line count */ }
[ \t]+        { /* ignore whitespace */ }

.             { printf("Unknown character: %s\n", yytext); }

%%
