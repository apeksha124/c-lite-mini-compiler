%{
#include "parser.tab.h"
#include <string.h>
#include <stdlib.h>
%}

%%

[0-9]+      { yylval.ival = atoi(yytext); return NUM; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.str = strdup(yytext); return ID; }
"="         { return ASSIGN; }
"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MUL; }
"/"         { return DIV; }
[ \t]+      ;           // ignore spaces/tabs
\n          { return NEWLINE; }
.           { return yytext[0]; }

%%

int yywrap() { return 1; }
