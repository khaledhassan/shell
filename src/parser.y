/* vim: tabstop=4 expandtab
*/
%{
    #include <stdio.h>
    #include "shell.h"
    void yyerror(char *s);
%}

/* metacharacters */
%token LESSTHAN
%token GREATERTHAN
%token PIPE
%token AMPERSAND
%token BACKSLASH
%token QUOTE

%token NEWLINE
%token WORD

%token QUIT

%start line


%%
line: WORD



%%

int yywrap(void){
    return 1;
}

void yyerror(char *s){
    fprintf(stderr, "%s\n", s);
}
