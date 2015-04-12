/* vim: tabstop=4 expandtab 
*/
%{
    #include <stdio.h>
    #include <string.h>
    #include "y.tab.h"
    #include "shell.h"
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
 
[-\[\]a-zA-Z0-9_:.,/~!@#\$%\^\(\)?\'*]+ { 
                            yylval.str = strdup(yytext); 

                            int alias_pos = find_alias(yylval.str);

                            if (alias_pos != -1) {
                                int value_len = strlen(alias_tab[alias_pos].value);
                                for (int i = value_len - 1; i >= 0; --i) {
                                    unput(alias_tab[alias_pos].value[i]);
                                }
                                free(yylval.str);
                            } else {
                                return WORD;
                            } 
                        }
\"(\\.|[^\n"\\])*\"     { yytext++; yytext[yyleng-2] = 0; yylval.str = strdup(yytext); return WORD; }

\n                      { return NEWLINE; }
[ \t]+                  ; /* Eat whitespace */

 /* . { yyerror("unrecognized character"); return 0; } */
%%

