/* vim: tabstop=4 expandtab 
*/
%{
    #include <stdio.h>
    #include <string.h>
    #include "y.tab.h"
    void yyerror(const char *s);
%}

%%
                        /* Built-ins */
"setenv"                { yylval.str = strdup(yytext); return SETENV; }
"printenv"              { yylval.str = strdup(yytext); return PRINTENV; }
"unsetenv"              { yylval.str = strdup(yytext); return UNSETENV; }
"cd"                    { yylval.str = strdup(yytext); return CD; }
"alias"                 { yylval.str = strdup(yytext); return ALIAS; }
"unalias"               { yylval.str = strdup(yytext); return UNALIAS; }
"bye"                   { return QUIT; }   
<<EOF>>                 { return QUIT; }
                        /* stderr redirects */
"2>"                    { return ERRTOFILE; }
"2>&1"                  { return ERRTOOUT; }
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
