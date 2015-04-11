// vim: tabstop=4 expandtab

#include <stdio.h>

//extern void yylex();

int main(void){


//    printf("Hello, world!\n");
    while(1){
        yyparse();
    }

    return 0;
}


void shell_init(void) {
    // init all variables - None for now?
    // define variables/tables
    
    // get path envoronment variables
    // get home env
    // disable anything to kill shell
}
void init_scanner_and_parser();
void print_prompt(void);
void get_command(void);
void process_command(void);
void recover_from_errors(void);

