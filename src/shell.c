// vim: tabstop=4 expandtab

#define _GNU_SOURCE
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <dirent.h>


#include "shell.h"
#include "y.tab.h"

//-----------------------------------
// Values declared extern
//-----------------------------------

int num_commands;
command_t command_tab[MAXCOMMANDS];
alias_t alias_tab[MAXALIAS];
env_t env_tab[MAXENV];


//-----------------------------------
// Functions
//-----------------------------------

int main(void){

    shell_init();

    while(1){
        print_prompt();
        switch (get_command()) {
            case OK:
                process_command();
                break;
            case SYSERR:
                fprintf(stderr, "get_command() failed... cleaning up\n");
                break;
            default:
                break; // TODO: what goes here?
        }
    }

    return 0;
}

void shell_init(void) {
    // init all variables - None for now?
    // define variables/tables

    // get path envoronment variables
    strcpy(env_tab[0].name, "PATH");
    strcpy(env_tab[0].value, getenv("PATH"));
    env_tab[0].used = 1;

    // get home env
    strcpy(env_tab[1].name, "HOME");
    strcpy(env_tab[1].value, getenv("HOME"));
    env_tab[1].used = 1;

    // disable anything to kill shell
    sigset(SIGINT, SIG_IGN);
    //sigset(SIGINT, ignoreCTRLC); // TODO: custom handler should clean up token stream? Lex/Yacc don't like something

}

// Reinitilaizes all tables
void init_scanner_and_parser(void) {
    // Clear the command table
    for(int i = 0; i != MAXCOMMANDS; ++i) {
        strcpy(command_tab[i].name, "");
        command_tab[i].in_fd = 0; // TODO: What should the default be? 
        command_tab[i].out_fd = 0;
        // clear the arguments
        for(int j = 0; j != MAXARGS; ++j) {
            strcpy(command_tab[i].arg_tab.args[j], "");
        }
        command_tab[i].n_args = 0;
    }
    num_commands = 0;
}

int get_command(void){
    init_scanner_and_parser();

    if (yyparse()) {
        fprintf(stderr, "get_command: yyparse error?\n");
        return SYSERR;
    } else {
        return OK;
    }
}

// Prints: user@hostname:pathname
void print_prompt(void) {
    char user[LOGIN_NAME_MAX], host[HOST_NAME_MAX];
    char* path;
    // Get the username
    if(getlogin_r(user, LOGIN_NAME_MAX)) {
        fprintf(stderr, "getlogin() unsuccessful.\n");
    }
    // Get current path
    path = get_current_dir_name();
    // get current hostname
    if(gethostname(host, HOST_NAME_MAX)) {
        fprintf(stderr, "gethostname() unsuccessful.\n");
    }
    if(!strcmp(user, "root"))
        printf("%s@%s:%s# ", user, host, path);
    else
        printf("%s@%s:%s$ ", user, host, path);

    free(path);
}
void process_command(void){
    printf("process_command: commands seen:%d \n", num_commands);
    for (int i = 0; i != num_commands; ++i) {
        printf("command %d: %s, n_args: %d, args:", i, command_tab[i].name, command_tab[i].n_args);
        for (int j = 0; j != command_tab[i].n_args; ++j) {
            printf("%s ", command_tab[i].arg_tab.args[j]);
        }
        printf("\n");
    }
}

void recover_from_errors(void) {
    fprintf(stderr, "Errors Occured.\n");
}

void ignoreCTRLC(int sig) {
    signal(sig, SIG_IGN);
    printf("Type 'bye' to exit\n");
    signal(SIGINT, ignoreCTRLC);
}
// inputs
//  path_buf: modifies this to contain the path to the executable
//  size: size of the buffer being passed in 
int findCommand(char* path_buf, size_t size, char* command) {
    //make a copy of the PATH variable
    char path[MAXSTRLEN];
    strcpy(path, env_tab[0].value);
    char* token = strtok(path, ":");
    char* originalDir = get_current_dir_name();
    char* currentDir = get_current_dir_name();
    int match = 0;
    while(token && !match) {
        if(chdir(token)) {
            fprintf(stderr, "Directory <%s> does not exist\n", token);
            match = -1;
        } else {
            currentDir = get_current_dir_name();
            // Begin by opening the directory and reading the filenames to match with the command we're looking for.
            DIR *dip;
            struct dirent *dit;
            
            // If we can open the directory, then that's half the battle :P
            if((dip = opendir(currentDir)) == NULL) {
                fprintf(stderr, "Couldn't open the directory <%s>\n", currentDir);
                match = -1;
            }

            while((dit = readdir(dip)) != NULL) {
                if(!strcmp(command, dit->d_name)) {
                    snprintf(path_buf, size, "%s/%s\n", currentDir, dit->d_name);
                    match = 1;
                    break;
                }
            }

        }

        free(currentDir);
        token = strtok(NULL, ":");
        
    }
    if(chdir(originalDir)) {
        fprintf(stderr, "Could not change back to orignal directory\n");
    }
    free(originalDir);

    if(match == 1) {
        return 0;
    } else {
        return 1;
    }
}


