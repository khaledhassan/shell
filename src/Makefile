FLAGS=-Wall -Wextra
CC=gcc


all: shell

shell: shell.c lex.yy.c yacc
	$(CC) $(CFLAGS) shell.c lex.yy.c -o shell

lex.yy.c: scanner.l
	flex scanner.l
.PHONY: yacc
yacc: parser.y
	bison -d -y parser.y	
.PHONY: clean
clean:
	rm -rf *.o shell lex.yy.c y.tab.c y.tab.h

