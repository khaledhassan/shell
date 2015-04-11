CFLAGS=-Wall -Wextra -std=gnu99
CC=colorgcc


all: shell

shell: shell.c lex.yy.c y.tab.c yacc
	$(CC) $(CFLAGS) shell.c lex.yy.c y.tab.c -o shell

lex.yy.c: scanner.l
	flex scanner.l
.PHONY: yacc
y.tab.c: parser.y
	bison -d -y parser.y	
.PHONY: clean
clean:
	rm -rf *.o shell lex.yy.c y.tab.c y.tab.h

