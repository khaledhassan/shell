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
#include "builtin.h"
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
    //sigset(SIGINT, ignore_CTRLC); // TODO: custom handler should clean up token stream? Lex/Yacc don't like something

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
    /* Policy: if a builtin command is present, it must be the only command present */
    int builtin_count = 0;
    for (int i = 0; i != num_commands; ++i) {
        if (command_tab[i].type != c_external) {
            builtin_count++;
        }
    }

    if (builtin_count > 1) {
        fprintf(stderr, "process_command: found %d builtin commands, a max of 1 per line is supported\n", builtin_count);
        return;
    }

    if (command_tab[0].type == c_external && builtin_count != 0) {
        fprintf(stderr, "process_command: found builtin command but not at beginning of line\n");
        return;
    }


    for (int i = 0; i != num_commands; ++i) {
        switch (command_tab[i].type ) {
            case c_cd:
                bi_cd(&command_tab[i]);
                break;
            case c_alias:
                bi_alias(&command_tab[i]);
                break;
            case c_unalias:
                bi_unalias(&command_tab[i]);
                break;
            case c_setenv:
                bi_setenv(&command_tab[i]);
                break;
            case c_printenv:
                bi_printenv(&command_tab[i]);
                break;
            case c_unsetenv:
                bi_unsetenv(&command_tab[i]);
                break;
            case c_external: {
                char full_path[MAXSTRLEN];
                int command_res = find_command(full_path, MAXSTRLEN, command_tab[i].name);
                if (command_res == 0) { // command is found
                    int pid;
                    pid = fork();

                    if (pid == 0) { // child
                        int argv_size = command_tab[i].n_args + 1;
                        char* argv[argv_size];
                        for (int j = 0; j != argv_size - 1; ++j) {
                            argv[j] = command_tab[i].arg_tab.args[j];
                        }
                        argv[argv_size] = NULL;

                        execv(full_path, argv);
                        exit(1);
                    } 

                    wait(0);
 
                } else if(command_res == -1) {
                    continue;
                } else if(command_res == 1) {
                    printf("Command '%s' not found.\n", command_tab[i].name);
                }
            }
                
                break;
        }

    }

}

void recover_from_errors(void) {
    fprintf(stderr, "Errors Occured.\n");
}

void ignore_CTRLC(int sig) {
    signal(sig, SIG_IGN);
    printf("Type 'bye' to exit\n");
    signal(SIGINT, ignore_CTRLC);
}
//  find_command
//  inputs:
//      path_buf: buffer will contain the path to the command if found
//      size: size of the buffer being passed in 
//      command: the name of the command/program being called
//  outputs:
//      returns 0 if command was found, 1 if not found, -1 for error.
int find_command(char* path_buf, size_t size, char* command) {
    //make a copy of the PATH variable
    char path[MAXSTRLEN];
    strcpy(path, env_tab[0].value);
    char* token = strtok(path, ":");
    char* originalDir = get_current_dir_name();
    char* currentDir = get_current_dir_name();
    int match = 0, search_path = 1;

    if(strlen(command) > 2) {
        if(command[0] == '.' && command[1] == '/') {
            search_path = 0;
            command += 2;
        }
    }
    if(search_path) {
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
                        snprintf(path_buf, size, "%s/%s", currentDir, dit->d_name);
                        match = 1;
                        break;
                    }
                }
            }
            free(currentDir);
            token = strtok(NULL, ":");      
        }    
    } else {
        printf("Looking for %s\n", command);
        DIR *dip;
        struct dirent *dit;
        
        // If we can open the directory, then that's half the battle :P
        if((dip = opendir(currentDir)) == NULL) {
            fprintf(stderr, "Couldn't open the directory <%s>\n", currentDir);
            match = -1;
        }

        while((dit = readdir(dip)) != NULL) {
            if(!strcmp(command, dit->d_name)) {
                snprintf(path_buf, size, "%s/%s", currentDir, dit->d_name);
                match = 1;
                break;
            }
        }
    }
    
    if(chdir(originalDir)) {
        fprintf(stderr, "Could not change back to orignal directory\n");
    }
    free(originalDir);

    if(match == 1) {
        return 0;
    } else if(match == -1) {
        return -1;
    } else {
        return 1;
    }
}


