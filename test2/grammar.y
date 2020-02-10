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
        | ASSIGN IDENTIFIER EQUALS int_expression { 
            e_statusc status = e_table_add_entry(&global_sym_table, $2, e_table_value_create_int($4));
            if(status != E_STATUS_OK) {
                /* Error assigning variable */
                switch(status) {
                    case E_STATUS_NOINIT:
                    default:
                        yyerror("Symbol table not inizialized\n");
                        break;
                    case E_STATUS_NESIZE:
                        yyerror("Symbol table full: too many variables\n");
                        break;
                    case E_STATUS_ALRDYDEF:
                        yyerror("Symbol already defined\n");
                        break;
                }
            }
            
            //e_table_memdump(&global_sym_table); // TODO: Remove call
        }
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
