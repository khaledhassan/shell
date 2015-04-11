// vim: tabstop=4 expandtab

#include "shell.h"
#include <stdio.h>


int main(void){

    shell_init();
    init_scanner_and_parser();


    while(1){
        yyparse();
    }

    return 0;
}


void shell_init(void) {
    // init all variables - None for now?
    // define variables/tables

    // get path envoronment variables
    env_tab[0].name = "PATH";
    env_tab[0].value = getenv("PATH");
    env_tab[0].used = 1;

    // get home env
    env_tab[1].name = "HOME";
    env_tab[1].value = getenv("HOME");
    env_tab[1].used = 1;

    // disable anything to kill shell
    signal(SIGINT, ignoreCTRLC);

}

// Reinitilaizes all tables
void init_scanner_and_parser(void) {
    // Clear the command table
    for(int i = 0; i < MAXCOMMANDS; ++i) {
        command_tab[i].name = "";
        command_tab[i].in_fd = 0; // TODO: What should the default be? 
        command_tab[i].out_fd = 0;
        // clear the arguments
        for(int j = 0; j < command_tab[i].n_args; ++j) {
            command_tab[i].arg_tab[j].args = "";
        }
        command_tab[i].n_args = 0;
    }
    num_commands = 0;
}

void print_prompt(void);
void get_command(void);
void process_command(void);
void recover_from_errors(void);
void ignoreCTRLC(int sig) {
    signal(sig, SIG_IGN);
    printf("Type 'bye' to exit\n");
    signal(SIGINT, ignoreCTRLC);
}

