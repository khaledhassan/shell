/* vim: tabstop=4 expandtab 
*/
%{
    #include <stdio.h>
    #include "y.tab.h"
%}

%x QUOTEDSTRING
%%
<<EOF>>                 { printf("BYE! "); return QUIT; }
"bye"                   { printf("BYE! "); return QUIT; }   
                        /* Metacharacters*/
"<"                     { printf("LESSTHAN "); return LESSTHAN; }
">"                     { printf("GREATERTHAN "); return GREATERTHAN; }
"|"                     { printf("PIPE" ); return PIPE; }
"&"                     { printf("AMPERSAND "); return AMPERSAND; }
"\\"                    { printf("BACKSLASH "); return BACKSLASH; }
    /*"\""                    { printf("QUOTE "); return QUOTE; } */
    /* <INITIAL>"\""           { BEGIN(QUOTEDSTRING); printf("BEGINNING QUOTEDSTRING CONDITION\n"); } */
    /* <QUOTEDSTRING>"\""      { BEGIN(INITIAL); printf("BEGINNING INITIAL CONDITION\n"); } */
    /* <QUOTEDSTRING>(^\")+    { printf("QUOTED STRING: %s ", yytext); } */
\"(\\.|[^\n"\\])*\"     { printf("QUOTED STRING: %s", yytext); return WORD; }
[ \t]+                  ; /* Eat whitespace */
\n                      { printf("\n"); return NEWLINE; }
[a-zA-Z0-9_:./~-]+      { printf("WORD: %s ", yytext); return WORD; }
%%

