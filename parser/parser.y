%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include "p_type.h"

#define YYDEBUG 1

double var_val[26] = { 0 }; 

extern int yyerror(char const *str);
extern int yylex (void);
%}

%union {
    double    dvl;
    char      name;
}

%token <dvl>      NUMBER
%token <name>     NAME
%token ADD
%token SUB
%token MUL
%token DIV
%token POW
%token SEMI
%token CR
%token CLEAR
%token HELP
%token QUIT
%token EQUAL
%token LPARAN
%token RPARAN
%token CLASS
%left ADD SUB
%left MUL DIV
%left POW
%right LPAREN RPAREN
%type <dvl> expr
/* %type <name> */

%%

statement:
    | statement CR
    { 
        printf(">> ");
    }
    | statement op_cmd CR
    | statement expr CR
    {
        if ($2)
            printf( ">> result:%g\n" , $2);
        printf(">> ");
    }
    | CLASS CR
    {
        printf("struct");
    }
    ;

expr:
    NUMBER 
    { 
        $$ = $1; 
    }
    | NAME
    {
        $$ = var_val[tolower($1)-'a'];
    }
    | NAME EQUAL expr SEMI
    {
       var_val[tolower($1)-'a'] = $3;
       $$ = 0;
    }
    | LPAREN expr RPAREN
    {
        $$ = $2;
    }
    | expr ADD expr
    {
        $$ = $1 + $3;
    }
    | expr SUB expr
    {
        $$ = $1 - $3;
    }
    | expr MUL expr
    {
        $$ = $1 * $3;
    }
    | expr DIV expr
    {
        $$ = $1 / $3;
    }
    | expr POW expr
    {
        $$ = pow($1, $3);
    }
    ;

op_cmd:
    QUIT
    {
        exit(0);
    }
    | CLEAR
    {
        if (system("clear")) {}
        printf(">> ");
    }
    ;

%%

int
yyerror(char const *str)
{
    extern char *yytext;
    printf("parser error near %s\n", yytext);
    return 0;

}

