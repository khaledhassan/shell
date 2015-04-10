/* vim: tabstop=4 expandtab
*/
%{
    #include <stdio.h>
    #include "shell.h"
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
line:   WORD



%%

int yywrap(void){
    return 1;
}

