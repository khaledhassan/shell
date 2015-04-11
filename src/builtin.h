// vim: tabstop=4 expandtab

#ifndef BUILTIN_H_
#define BUILTIN_H_

#include "shell.h"

//-----------------------------------
// Function Prototypes
//-----------------------------------

int bi_cd(command_t* cmd);
int bi_alias(command_t* cmd);
int bi_unalias(command_t* cmd);
int bi_setenv(command_t* cmd);
int bi_printenv(command_t* cmd);
int bi_unsetenv(command_t* cmd);

#endif // BUILTIN_H_
