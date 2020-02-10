%{
    #include <stdio.h>
    #include <stdlib.h>
    
    #include "evoscript.h"
    
    extern int yylex();
    extern int yyparse();
    
    extern FILE* yyin;
    
    void yyerror(const char* s);
%}

%union {
    int ival;
    char* sname;
}

%token<ival> INT
%token<sname> IDENTIFIER
%token ASSIGN EQUALS
%token PLUS MINUS
%token MULTIPLY DIVIDE
%token NEWLINE

%left PLUS MINUS
%left MULTIPLY DIVIDE

%type<ival> int_expression

%%
prgm:
    | prgm line
    ;   
    
line: NEWLINE
    | expression NEWLINE { printf("ok\n"); }
    
expression: assign
            | int_expression { printf("> %d\n", $1); }
            
assign: ASSIGN IDENTIFIER { printf("Assigning identifier %s\n", $2); }
        | ASSIGN IDENTIFIER EQUALS int_expression { printf("Assigning identifier %s with value %d\n", $2, $4); }
        ;

int_expression: INT 
                | int_expression MULTIPLY int_expression { $$ = $1 * $3; }
                | int_expression DIVIDE int_expression { $$ = $1 / $3; }
                | int_expression PLUS int_expression { $$ = $1 + $3; }
                | int_expression MINUS int_expression { $$ = $1 - $3; }
                ;
%%

int main(void) {
    printf("evoScript %s\n", E_VERSION);
    e_init();
    
    yyin = stdin;
    do {
        yyparse();
    } while(!feof(yyin));
    
    return 0;
}

void yyerror(const char* s) {
    printf("Error happend: %s\n", s);
    exit(-1);
}
