%{
#include <stdio.h>
#include "y.tab.h"

int
yywrap(void)
{
    return 1;
}
%}

%%
"+"             return ADD;
"-"             return SUB;
"*"             return MUL;
"/"             return DIV;
"^"             return POW;
"="             return EQUAL;
";"             return SEMI;
"\n"            return CR;
"quit"          return QUIT;
"clear"         return CLEAR;
"help"          return HELP;
[(]             return LPAREN;
[)]             return RPAREN;
"class"         return CLASS;

(0(\.[0-9]+)?)|([1-9][0-9]*(\.[0-9]+)?)  {
    yylval.dvl = strtod(yytext, 0);
    return NUMBER;
}

[a-zA-Z] {
    yylval.name = *yytext;
    return NAME; 
}
[ \t ] ;
. {
    fprintf(stderr, "lexical error.\n");
}
%%

