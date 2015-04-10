/* vim: tabstop=4 expandtab 
*/
%{
    #include <stdio.h>
    #include "y.tab.h"
%}

%%
<<EOF>>                 { printf("BYE! "); return QUIT; }
"bye"                   { printf("BYE! "); return QUIT; }   
                        /* Metacharacters*/
"<"                     { printf("LESSTHAN "); return LESSTHAN; }
">"                     { printf("GREATERTHAN "); return GREATERTHAN; }
"|"                     { printf("PIPE" ); return PIPE; }
"&"                     { printf("AMPERSAND "); return AMPERSAND; }
"\\"                    { printf("BACKSLASH "); return BACKSLASH; }
"\""                    { printf("QUOTE "); return QUOTE; }


[ \t]+                  ; /* Eat whitespace */
\n                      { printf("\n"); return NEWLINE; }
[a-zA-Z0-9_:./~-]+      { printf("WORD: %s", yytext); return WORD; }
%%

