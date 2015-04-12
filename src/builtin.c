// vim: tabstop=4 expandtab

#include "builtin.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
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
        int pos = find_alias(cmd->arg_tab.args[0]);
        if (pos == -1) { // alias not found, insert
            pos = 0;
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
        } else { // alias found, update
            strcpy(alias_tab[pos].value, cmd->arg_tab.args[1]);
            return OK;
        }
    } else { // wrong number of args
        fprintf(stderr, "alias expects exactly 0 or 2 arguments, %d provided\n", cmd->n_args);
        return SYSERR;
    }
}

int bi_unalias(command_t* cmd){
    if (cmd->n_args != 1) {
        fprintf(stderr, "unalias expects exactly 1 argument, %d provided\n", cmd->n_args);
        return SYSERR;
    }

    int pos = 0;
    int found = 0;

    while (pos < MAXALIAS && found == 0) {
        if (alias_tab[pos].used == 1 && strcmp(alias_tab[pos].name, cmd->arg_tab.args[0]) == 0 ) {
            found = 1;
        } else {
            ++pos;
        }
    }

    if (found == 0) {
        fprintf(stderr, "unalias: cannot find alias with name %s\n", cmd->arg_tab.args[0]);
        return SYSERR;
    } else {
        alias_tab[pos].used = 0;
        return OK;
    }
}

int bi_setenv(command_t* cmd){
    if (cmd->n_args != 2) {
        fprintf(stderr, "setenv expects exactly 2 arguments, %d given\n", cmd->n_args);
        return SYSERR;
    } else {
        // first look for variable
        int pos = 0;
        int found = 0;

        while (pos < MAXENV && found == 0) {
            if (env_tab[pos].used == 1 && strcmp(env_tab[pos].name, cmd->arg_tab.args[0]) == 0 ) {
                found = 1;
            } else {
                ++pos;
            }
        }
        // if found, update it
        if (found == 1) {
             strcpy(env_tab[pos].value, cmd->arg_tab.args[1]);
             return OK;
        } else {
        // if not found, add it
            pos = 0;
            while (pos < MAXENV) {
                if (env_tab[pos].used == 0) {
                    env_tab[pos].used = 1;
                    strcpy(env_tab[pos].name, cmd->arg_tab.args[0]);
                    strcpy(env_tab[pos].value, cmd->arg_tab.args[1]);
                    return OK;
                 } else {
                     ++pos;
                 }
            }
            return SYSERR; // could not find space
        }
    }
}

int bi_printenv(command_t* cmd){
    for (int i = 0; i != MAXENV; ++i) {
            if (env_tab[i].used) {
                printf("%s=%s\n", env_tab[i].name, env_tab[i].value);
            }
    }
    return OK;
}

int bi_unsetenv(command_t* cmd){
    int pos = 0;
    int found = 0;

    while (pos < MAXENV && found == 0) {
        if (env_tab[pos].used == 1 && strcmp(env_tab[pos].name, cmd->arg_tab.args[0]) == 0 ) {
            found = 1;
        } else {
            ++pos;
        }
    }

    if (found == 1) {
        env_tab[pos].used = 0;
        return OK;
    } else {
        return SYSERR;
    }
}

