#include <stdio.h>
#include <stdlib.h>
#include "y.tab.h"

int main(void)
{
	FILE *yyin = stdin;

	printf(" _______________________________________________________________________ \n");
    printf("|                                                                       |\n");
    printf("|                           简易计算器                                  |\n");
    printf("| + - * / ^ 操作                                                        |\n");
    printf("| a=1 b=2 a+b                                                           |\n");
    printf("|_______________________________________________________________________|\n"); 
	printf(">> "); 

    while (1)
    {
		yyparse();
	}

	return 0;
}