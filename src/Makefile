CFLAGS=-Wall -Wextra -std=gnu99
DEBUG_FLAGS=-g
CC=gcc

all: shell args

args: args.c
	$(CC) $(CFLAGS) args.c -o args

.PHONY: debug
debug: shell.c lex.yy.c y.tab.c builtin.c regex_lib.c
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) shell.c lex.yy.c y.tab.c builtin.c regex_lib.c -o shell


shell: shell.c lex.yy.c y.tab.c builtin.c regex_lib.c
	$(CC) $(CFLAGS) shell.c lex.yy.c y.tab.c builtin.c regex_lib.c -o shell

lex.yy.c: scanner.l
	flex scanner.l

y.tab.c: parser.y
	bison -d -y parser.y	
.PHONY: clean
clean:
	rm -rf *.o shell lex.yy.c y.tab.c y.tab.h args

