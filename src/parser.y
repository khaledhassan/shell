/* vim: tabstop=4 expandtab
*/
%{
    #include <stdio.h>
    #include "shell.h"
    #include <stdlib.h>
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
%type <str> command


%define parse.error verbose

%%

commands: NEWLINE
	 	| commands command NEWLINE
	 	| QUIT {printf("Cya\n"); exit(0);}
	 	;


command: 	WORD
		| WORD PIPE command 
			{
				printf("%s pipe into %s\n", $1, $3); 
			}
		| WORD LESSTHAN WORD
			{
				printf("%s into %s\n", $3, $1);
			}
		| WORD GREATERTHAN WORD
			{
				printf("%s into %s\n", $1, $3);
			}
		;


%%												

int yywrap(void){
    return 1;
}


void yyerror(char *s){
    fprintf(stderr, "%s\n", s);
}
