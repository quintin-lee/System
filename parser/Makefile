
all:mycalc y.tab.c lex.yy.c

mycalc:y.tab.c lex.yy.c test.c
	gcc -o mycalc -O2 -g y.tab.c lex.yy.c test.c -lm

y.tab.c:parser.y
	bison --yacc -dv parser.y -o y.tab.c

lex.yy.c:lex.l
	flex lex.l

clean:
	rm -f mycalc y.tab.c y.tab.h y.output lex.yy.c lex.h
