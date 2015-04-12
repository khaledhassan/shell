/* vim: tabstop=4 expandtab
*/
%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "shell.h"
    int yylex(void);
    void yyerror(const char *s);
%}

%union {
	int num;
	char *str;
}

/* metacharacters */
%token LESSTHAN GREATERTHAN PIPE AMPERSAND BACKSLASH NEWLINE 
%token <str> SETENV PRINTENV UNSETENV CD ALIAS UNALIAS 
%token QUIT
%token <str> WORD
%type <str> command
%left PIPE

%start lines

%define parse.error verbose

%%

lines: line | lines line

line: NEWLINE { YYACCEPT; }
    | commands NEWLINE { YYACCEPT; }
    | QUIT { printf("cya\n"); exit(0);}
    ;

commands: command args { }
        | commands PIPE command args { }
        ;

command: builtin
       | WORD {
            command_t* this_command = &command_tab[num_commands];
            strcpy(this_command->name, $1);
            this_command->type = c_external;
            this_command->n_args = 1;
            strcpy(this_command->arg_tab.args[0], $1); // non-builtins need argv[0] to be their binary name?
            num_commands++;
}
     /*
       | WORD PIPE command { 
            printf("%s pipe into %s\n", $1, $3); 
         }
       | WORD LESSTHAN WORD {
            printf("%s into %s\n", $3, $1);
         }
       | WORD GREATERTHAN WORD {
            printf("%s into %s\n", $1, $3);
         }
       | WORD GREATERTHAN GREATERTHAN WORD {
            printf("%s appending into %s\n", $1, $4);
         }
     */
       ;

builtin: SETENV {
            command_t* this_command = &command_tab[num_commands];
            strcpy(this_command->name, $1);
            this_command->type = c_setenv;
            this_command->n_args = 0;
            num_commands++;
         }
       | PRINTENV {
            command_t* this_command = &command_tab[num_commands];
            strcpy(this_command->name, $1);
            this_command->type = c_printenv;
            this_command->n_args = 0;
            num_commands++;
         }
       | UNSETENV {
            command_t* this_command = &command_tab[num_commands];
            strcpy(this_command->name, $1);
            this_command->type = c_unsetenv;
            this_command->n_args = 0;
            num_commands++;
         }
       | CD {
            command_t* this_command = &command_tab[num_commands];
            strcpy(this_command->name, $1);
            this_command->type = c_cd;
            this_command->n_args = 0;
            num_commands++;
         }
       | ALIAS {
            command_t* this_command = &command_tab[num_commands];
            strcpy(this_command->name, $1);
            this_command->type = c_alias;
            this_command->n_args = 0;
            num_commands++;
         }
       | UNALIAS {
            command_t* this_command = &command_tab[num_commands];
            strcpy(this_command->name, $1);
            this_command->type = c_unalias;
            this_command->n_args = 0;
            num_commands++;
         }
       ;

args: /* no arguments */
    | args WORD /* 1 or more arguments */ {
        command_t* this_command = &command_tab[num_commands-1];
        args_t* this_command_argtab = &(this_command->arg_tab);
        strcpy(this_command_argtab->args[this_command->n_args], $2);
        this_command->n_args++;
      }


%%

int yywrap(void){
    return 1;
}


void yyerror(const char *s){
    fprintf(stderr, "%s\n", s);
}
