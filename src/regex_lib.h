#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include <regex.h>

/* The following is the size of a buffer to contain any error messages
   encountered when the regular expression is compiled. */

#define MAX_ERROR_MSG 0x1000

int compile_regex (regex_t * r, const char * regex_text);

int match_regex (regex_t * r, const char * to_match, char* first_result);

char *repl_str(const char *str, const char *old, const char *new);