#include <stdio.h>
#include "evoscript.h"
#include "grammar.tab.h"

extern int yylex();
extern int yyparse();
extern FILE* yyin;


int main(void) {
    printf("evoScript main %s\n", E_VERSION);
    e_init();
    
    
    yyin = stdin;
    do {
        yyparse();
    } while(!feof(yyin));
    
    return 0;
}
