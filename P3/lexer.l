%{
#include "parser.tab.h"
#include <string.h>
%}

digit       [0-9]
letter      [a-zA-Z_]

%%
"int"       { return INT; }
"float"     { return FLOAT; }
"void"      { return VOID; }

{letter}({letter}|{digit})*   {
    yylval.str_val = strdup(yytext);
    return IDENTIFIER;
}

[ \t\r\n]+  ;
.           { return yytext[0]; }
%%

int yywrap() { return 1; }
