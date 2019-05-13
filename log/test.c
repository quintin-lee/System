#include <stdio.h>
#include <stdlib.h>
#include "log.h"

int main(int argv,char**argc)
{
    if (argv < 1) 
        return -1;
    printf("%s\n",argc[0]);
    LogWrite(INFO,"%s","Hello World!");
    LogWrite(DEBUG,"%s","H.e.l.l.o W.o.r.l.d!");
    LogWrite(WARN,"%s","H e l l o W o r l d!");
    LogWrite(ERROR,"%s","Hallo World!");
    return 0;

}
