/* vim: tabstop=4 expandtab 
*/
%{
    #include <stdio.h>
    #include <string.h>
    #include "y.tab.h"
    #include "shell.h"
    void yyerror(const char *s);
%}

%s SAWALIAS
%s SAWBRACE
%%
                        /* Built-ins */
"setenv"                { BEGIN(SAWALIAS); yylval.str = strdup(yytext); return SETENV; }
"printenv"              { yylval.str = strdup(yytext); return PRINTENV; }
"unsetenv"              { BEGIN(SAWALIAS); yylval.str = strdup(yytext); return UNSETENV; }
"cd"                    { yylval.str = strdup(yytext); return CD; }
"alias"                 { BEGIN(SAWALIAS); yylval.str = strdup(yytext); return ALIAS; }
"unalias"               { BEGIN(SAWALIAS); yylval.str = strdup(yytext); return UNALIAS; }
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
"~"[-\[\]a-zA-Z0-9_:.,/!@#\$%\^\(\)?\'*]?                     {
                             int env_pos = find_env("HOME");
                             if (env_pos == -1) {
                                 fprintf(stderr, "error: HOME environment variable does not exist!\n");
                             } else {
                                 int value_len = strlen(env_tab[env_pos].value);
                                 unput('/'); //trailing slash at the end of home directory
                                 for (int i = value_len - 1; i >= 0; --i) {
                                     unput(env_tab[env_pos].value[i]);
                                 }
                             }
                        }
"${"                    { BEGIN(SAWBRACE); }
<SAWBRACE>"}"           { BEGIN(INITIAL); }

<SAWALIAS>[-\[\]a-zA-Z0-9_:.,/!@#\$%\^\(\)?\'*]+ {
                            yylval.str = strdup(yytext);
                            return WORD;
                        }


<INITIAL>[-\[\]a-zA-Z0-9_:.,/!@#\$%\^\(\)?\'*]+ {
                            yylval.str = strdup(yytext);

                            int alias_pos = find_alias(yylval.str);

                            if ((first_token == 1) && (alias_pos != -1)) {
                                int value_len = strlen(alias_tab[alias_pos].value);
                                for (int i = value_len - 1; i >= 0; --i) {
                                    unput(alias_tab[alias_pos].value[i]);
                                }
                                free(yylval.str);
                            } else {
                                first_token = 0;
                                return WORD;
                            } 
                        }

<SAWBRACE>[-\[\]a-zA-Z0-9_:.,/!@#\$%\^\(\)?\'*]+ {
                            yylval.str = strdup(yytext);

                            int env_pos = find_env(yylval.str);
                            int alias_pos = find_alias(yylval.str);
                            if (env_pos != -1) {
                                int value_len = strlen(env_tab[env_pos].value);
                                for (int i = value_len - 1; i >= 0; --i) {
                                    unput(env_tab[env_pos].value[i]);
                                }
                                free(yylval.str);
                            } else {
                                first_token = 0;
                                return WORD;
                            }
                        }

\"(\\.|[^\n"\\])*\"     { yytext++; yytext[yyleng-2] = 0; yylval.str = strdup(yytext); return WORD; }

\n                      { BEGIN(INITIAL); return NEWLINE; }
[ \t]+                  ; /* Eat whitespace */

 /* . { yyerror("unrecognized character"); return 0; } */
%%

