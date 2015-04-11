// vim: tabstop=4 expandtab

#include "shell.h"
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
    
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
    init_scanner_and_parser();

    while(1){
        print_prompt();
        get_command();
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

// Prints: user@hostname:pathname$
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

void get_command(void) {
    init_scanner_and_parser();
    if(yyparse()) {
        recover_from_errors();
    } 
}
void process_command(void);
void recover_from_errors(void) {
    fprintf(stderr, "Errors Occured.\n");
}
void ignoreCTRLC(int sig) {
    signal(sig, SIG_IGN);
    printf("Type 'bye' to exit\n");
    signal(SIGINT, ignoreCTRLC);
}

