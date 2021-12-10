SHELL=/bin/sh

sources = lex.yy.c y.tab.c ast.c
headers = y.tab.h ast.h

main: $(sources) $(headers)
	gcc -o $@ $(sources)

y.tab.c:	grammar.y
	bison -dy $<

y.tab.h:	grammar.y
	bison -dy $<

lex.yy.c: lexinput.l $(headers)
	flex $<


