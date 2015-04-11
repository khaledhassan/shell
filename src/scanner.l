/* vim: tabstop=4 expandtab 
*/
%{
    #include <stdio.h>
    #include "y.tab.h"
%}

%%
                        /* Built-ins */
"setenv"                { return SETENV; }
"printenv"              { return PRINTENV; }
"unsetenv"              { return UNSETENV; }
"cd"                    { return CD; }
"alias"                 { return ALIAS; }
"unalias"               { return UNALIAS; }
"bye"                   { return QUIT; }   
<<EOF>>                 { return QUIT; }
                        /* Metacharacters */
"<"                     { return LESSTHAN; }
">"                     { return GREATERTHAN; }
"|"                     { return PIPE; }
"&"                     { return AMPERSAND; }
"\\"                    { return BACKSLASH; }
 
[a-zA-Z0-9_:./~-]+      { yylval.str = strdup(yytext); return WORD; }
\"(\\.|[^\n"\\])*\"     { yytext++; yytext[yyleng-2] = 0; yylval.str = strdup(yytext); return WORD; }

\n                      { return NEWLINE; }
[ \t]+                  ; /* Eat whitespace */

. { yyerror("unrecognized character"); return 0; }
%%
