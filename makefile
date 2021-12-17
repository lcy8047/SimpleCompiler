SHELL=/bin/sh

sources = lex.yy.c y.tab.c ast.cpp
headers = y.tab.h ast.hpp

codeGen: $(sources) $(headers)
	g++ -o $@ $(sources)

y.tab.c:	grammar.y
	bison -dy $< --verbose

y.tab.h:	grammar.y
	bison -dy $< --verbose

lex.yy.c: lexinput.l $(headers)
	flex $<

clean:
	rm -f lex.yy.c y.tab.c y.tab.h codeGen
