// vim: tabstop=4 expandtab

#include <stdio.h>

//extern void yylex();

int main(void){


//    printf("Hello, world!\n");
    while(1){
        yyparse();
    }

    return 0;
}

