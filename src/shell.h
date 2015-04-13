// vim: tabstop=4 expandtab

#ifndef SHELL_H_
#define SHELL_H_

#include <stdlib.h>

//-----------------------------------
// Global Variables / Defines
//-----------------------------------

#define MAXCOMMANDS 50
#define MAXARGS 300
#define MAXALIAS 300
#define MAXENV 300
#define MAXSTRLEN 300
#define HOST_NAME_MAX 64
#define LOGIN_NAME_MAX 256

#define OK       0
#define SYSERR  -1

typedef struct {
    char   args[MAXARGS][MAXSTRLEN];
} args_t;

typedef enum {
    c_external,
    c_alias,
    c_unalias,
    c_cd,
    c_setenv,
    c_printenv,
    c_unsetenv
} cmd_t;

typedef struct {
    char    name[MAXSTRLEN];
    cmd_t   type;
    int     in_fd;
    int     out_fd;
    int     n_args;
    args_t  arg_tab;
} command_t;

extern int num_commands;
extern command_t command_tab[MAXCOMMANDS];

typedef struct {
    int     used;
    char    name[MAXSTRLEN];
    char    value[MAXSTRLEN];
} alias_t;

extern alias_t alias_tab[MAXALIAS];

typedef struct {
    char    name[MAXSTRLEN];
    char    value[MAXSTRLEN];
    int     used;
} env_t;

extern env_t env_tab[MAXENV];

extern int abort_command;
extern int run_in_background;
extern int redir_stderr;
extern int saved_stderr;
extern int first_token;

//-----------------------------------
// Function Prototypes
//-----------------------------------

void shell_init(void);
void init_scanner_and_parser(void);
void print_prompt(void);
int get_command(void);
void process_command(void);
void recover_from_errors(void);
void ignoreCTRLC(int);
int find_command(char*, size_t, char*);
int find_alias(char* alias_name);
int find_env(char* env_name);

#endif // SHELL_H_
