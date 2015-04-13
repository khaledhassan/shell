/* vim: tabstop=4 expandtab
*/
%{
    #define _GNU_SOURCE
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <unistd.h>
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
%token ERRTOFILE ERRTOOUT
%token <str> SETENV PRINTENV UNSETENV CD ALIAS UNALIAS 
%token QUIT
%token <str> WORD
%type <str> command
%left PIPE

%start lines

%%

lines: line | lines line

line: NEWLINE { YYACCEPT; }
    | error NEWLINE { abort_command = 1; }
    | commands NEWLINE { YYACCEPT; }
    | QUIT { printf("cya\n"); exit(0);}
    ;

commands: command args {
/*                int alias_pos = find_alias($1);
                if (alias_pos != -1) { // alias found
                    yyUnput(alias_tab[alias_pos].value, strlen(alias_tab[alias_pos].value));
                }
*/          }
        | commands PIPE command args { 
            int pipe_tab[2];
            pipe(pipe_tab);
            command_tab[num_commands-2].out_fd = pipe_tab[1];
            command_tab[num_commands-1].in_fd = pipe_tab[0];
         }
        | commands redir { }
        | commands AMPERSAND { run_in_background = 1; }
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
    ;

redir: LESSTHAN WORD {
            int infd = open($2, O_RDONLY);
            if (infd == -1) {
                fprintf(stderr, "Cannot open input file: %s\n", $2);
                abort_command = 1;
            } else {
                command_tab[0].in_fd = infd;
            }
       }
     | GREATERTHAN WORD {
            int outfd = open($2, O_WRONLY | O_TRUNC | O_CREAT);
            if (outfd == -1) {
                fprintf(stderr, "Cannot open output file: %s\n", $2);
                abort_command = 1;
            } else {
                command_tab[num_commands-1].out_fd = outfd;
            }
       }
     | GREATERTHAN GREATERTHAN WORD {
            int outfd = open($3, O_WRONLY | O_APPEND | O_CREAT);
            if (outfd == -1) {
                fprintf(stderr, "Cannot open output file: %s\n", $3);
                abort_command = 1;
            } else {
                command_tab[num_commands-1].out_fd = outfd;
            }
       }
     | ERRTOFILE WORD {
            if (redir_stderr != 0) {
                // restore saved stderr in case it was redirected
                dup2(saved_stderr, 2);
                fprintf(stderr, "Cannot redirect stderr twice!\n");
                abort_command = 1;
            } else {
                redir_stderr = 1;
                int outfd = open($2, O_WRONLY | O_TRUNC | O_CREAT);
                if (outfd == -1) {
                    fprintf(stderr, "Cannot open output file: %s\n", $2);
                    abort_command = 1;
                } else {
                    close(STDERR_FILENO);
                    dup(outfd);
                    close(outfd);
                }
            }
       }
     | ERRTOOUT {
            if (redir_stderr != 0) {
                // restore saved stderr in case it was redirected
                dup2(saved_stderr, 2);
                fprintf(stderr, "Cannot redirect stderr twice!\n");
                abort_command = 1;
            } else {
                redir_stderr = 1;
                close(STDERR_FILENO);
                dup(STDOUT_FILENO);
            }
       }
     ;

%%

int yywrap(void){
    return 1;
}


void yyerror(const char *s){
    fprintf(stderr, "%s\n", s);
}
