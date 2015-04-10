/* vim: tabstop=4 expandtab 
*/
%{
    #include <stdio.h>
    #include "y.tab.h"
%}

%%
<<EOF>>                 { return QUIT; }
"bye"                   { return QUIT; }   
                        /* Metacharacters*/
"<"                     { return LESSTHAN; }
">"                     { return GREATERTHAN; }
"|"                     { return PIPE; }
"&"                     { return AMPERSAND; }
"\\"                    { return BACKSLASH; }
 
\"(\\.|[^\n"\\])*\"     { return STRING; }
[ \t]+                  ; /* Eat whitespace */
\n                      { return NEWLINE; }
[a-zA-Z0-9_:./~-]+      { yylval.str = strdup(yytext); return WORD; }
%%