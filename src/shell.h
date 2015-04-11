#ifndef SHELL_H_
#define SHELL_H_

//-----------------------------------
// Global Variables / Defines
//-----------------------------------

#define MAXCOMMANDS 50
#define MAXARGS 300
#define MAXALIAS 300

typedef struct {
    char*   args[MAXARGS];
} args_t;

typedef struct {
    char*   name;
    int     in_fd;
    int     out_fd;
    int     n_args;
    args_t  arg_tab;
} command_t;

typedef struct {
    int     used;
    char*   name;
    char*   value;
} alias_t;

typedef enum {
    external, 
    alias, 
    unalias,
    cd, 
    setenv, 
    printenv, 
    unsetenv
} builtin_cmd_t;

//-----------------------------------
// Function Prototypes
//-----------------------------------

void shell_init(void);
void init_scanner_and_parser();
void print_prompt(void);
void get_command(void);
void process_command(void);
void recover_from_errors(void);


#endif // SHELL_H_
