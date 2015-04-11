/* vim: tabstop=4 expandtab
*/
%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "shell.h"
    void yyerror(const char *s);
%}

%union {
	int num;
	char *str;
}

/* metacharacters */
%token LESSTHAN GREATERTHAN PIPE AMPERSAND BACKSLASH NEWLINE 

%token SETENV PRINTENV UNSETENV CD ALIAS UNALIAS QUIT
%token <str> WORD

%type <str> args program
%type <str> command

%left PIPE

%start lines

%define parse.error verbose

%%

lines: line | lines line

line: NEWLINE
    | commands NEWLINE
    | QUIT { printf("cya\n"); exit(0);}
    ;

commands: command { printf("single command\n"); }
        | commands command { printf("multiple commands\n"); }
        ;

command: builtin
       | WORD
       | WORD PIPE command { 
            printf("%s pipe into %s\n", $1, $3); 
         }
       | WORD LESSTHAN WORD {
            printf("%s into %s\n", $3, $1);
         }
       | WORD GREATERTHAN WORD {
            printf("%s into %s\n", $1, $3);
         }
       ;

builtin: SETENV WORD WORD { printf("setenv %s=%s\n", $2, $3); }
       | PRINTENV { printf("printenv\n"); }
       | UNSETENV WORD { printf("unsetenv %s\n", $2); }
       | CD { printf("cd to home\n"); }
       | CD WORD { printf("cd to %s\n", $2); }
       | ALIAS { printf("print aliases\n"); }
       | ALIAS WORD WORD { printf("set alias: %s to %s\n", $2, $3); }
       | UNALIAS WORD { printf("unalias %s\n", $2); }
       ;

%%

int yywrap(void){
    return 1;
}


void yyerror(const char *s){
    fprintf(stderr, "%s\n", s);
}
