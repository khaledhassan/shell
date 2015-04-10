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

%%

commands: WORD {printf("%s\n", $1);}
		| commands command
		;

command: 
	WORD {
		printf("%s\n", $1);
		
	}
	;

%%

int yywrap(void){
    return 1;
}

void yyerror (char *s){
    fprintf (stderr, "%s\n", s);
}