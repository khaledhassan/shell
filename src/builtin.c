// vim: tabstop=4 expandtab

#include "builtin.h"
#include <stdio.h>
#include "shell.h"

int bi_cd(command_t* cmd){
    if (cmd->n_args == 0) { // cd to home
        if (chdir(env_tab[1].value) != 0) {
            fprintf(stderr, "cd: $HOME variable not set!\n");
            return SYSERR;
        } else {
            return OK;
        }
    } else { // cd to some path
        if (chdir(cmd->arg_tab.args[0]) != 0) {
            fprintf(stderr, "cd: invalid path!\n");
            return SYSERR;
        } else {
            return OK;
        }
    }
}

int bi_alias(command_t* cmd){
    if (cmd->n_args == 0) { // print aliases
        for (int i = 0; i != MAXALIAS; ++i) {
            if (alias_tab[i].used) {
                printf("%s is aliased to %s\n", alias_tab[i].name, alias_tab[i].value);
            }
        }
        return OK;
    } else if (cmd->n_args == 2) { // set alias
        int pos = 0;
        int found = 0;
        while (pos < MAXALIAS && found == 0) {
            if (alias_tab[pos].used == 0) {
                found = 1;
            } else {
                ++pos;
            }
        }

        if (found == 0) {
            fprintf(stderr, "alias: cannot set new alias -- table full?\n");
            return SYSERR;
        } else {
            alias_tab[pos].used = 1;
            strcpy(alias_tab[pos].name, cmd->arg_tab.args[0]);
            strcpy(alias_tab[pos].value, cmd->arg_tab.args[1]);
            return OK;
        }

    } else { // wrong number of args
        fprintf(stderr, "alias expects exactly 0 or 2 arguments, %d provided\n", cmd->n_args);
        return SYSERR;
    }
}

int bi_unalias(command_t* cmd){

}

int bi_setenv(command_t* cmd){

}

int bi_printenv(command_t* cmd){

}

int bi_unsetenv(command_t* cmd){

}

