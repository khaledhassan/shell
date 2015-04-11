// vim: tabstop=4 expandtab

#include "builtin.h"
#include <stdio.h>
#include "shell.h"

int cd(command_t* cmd){
    if (cmd->n_args == 0) { // cd to home
        if (chdir(env_tab[1].value) == -1) {
            fprintf(stderr, "cd: $HOME variable not set!\n");
            return SYSERR;
        } else {
            return OK;
        }
    } else { // cd to some path
        if (chdir(cmd->arg_tab[0]) == -1) {
            fprintf(stderr, "cd: invlid path!\n");
            return SYSERR;
        } else {
            return OK;
        }
    }
}

void alias(command_t* cmd){

}

void unalias(command_t* cmd){

}

void setenv(command_t* cmd){

}

void printenv(command_t* cmd){

}

void unsetenv(command_t* cmd){

}

