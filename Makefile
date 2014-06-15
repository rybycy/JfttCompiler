c: bison.y flex.l
	bison -d bison.y
	flex flex.l
	g++ -o c lex.yy.c bison.tab.c
