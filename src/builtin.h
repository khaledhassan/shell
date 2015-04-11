// vim: tabstop=4 expandtab

#ifndef BUILTIN_H_
#define BUILTIN_H_

#include "shell.h"

//-----------------------------------
// Function Prototypes
//-----------------------------------

void cd(command_t* cmd);
void alias(command_t* cmd);
void unalias(command_t* cmd);
void setenv(command_t* cmd);
void printenv(command_t* cmd);
void unsetenv(command_t* cmd);

#endif // BUILTIN_H_
