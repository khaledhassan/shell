/* vim: tabstop=4 expandtab
*/
%{
    #include <stdio.h>
    #include "shell.h"
    void yyerror(char *s);
%}

%union {
	int num;
	char *str;
}

/* metacharacters */
%token LESSTHAN GREATERTHAN PIPE AMPERSAND BACKSLASH
%token QUOTE NEWLINE QUIT STRING

%token <str> WORD
%type <str> args program

%left PIPE

%define parse.error verbose

%%

program:
		program args NEWLINE
		|
		;

args: 
		WORD
		| args PIPE args {printf("%s pipe %s\n", $1, $3); }
		;


%%												

int yywrap(void){
    return 1;
}


void yyerror(char *s){
    fprintf(stderr, "%s\n", s);
}
