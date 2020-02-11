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
%token GL_SYM_DUMP

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
            | GL_SYM_DUMP { e_table_memdump(&global_sym_table); }
            ;
            
assign: ASSIGN IDENTIFIER { 
            /* Empty declaration, variables are initialized with their default values, let x */
            e_statusc status = e_table_add_entry(&global_sym_table, $2, e_create_int(0));
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
        }
        | ASSIGN IDENTIFIER EQUALS int_expression { 
            /* Integer definition with initialization, let x = 42 */
            e_statusc status = e_table_add_entry(&global_sym_table, $2, e_create_int($4));
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
        }
        | IDENTIFIER EQUALS int_expression {
            /* change value of variable, a = 3 */
            e_statusc status = e_table_change_entry(&global_sym_table, $1, e_create_int($3));
            if(status != E_STATUS_OK) {
                switch(status) {
                    case E_STATUS_NOTFOUND:
                    default:
                        yyerror("Trying to assign to unknown identifier\n");
                        break;
                    case E_STATUS_NOINIT:
                        yyerror("Symbol table not inizialized\n");
                        break;
                }
            }
        }
        ;

int_expression: INT
                | IDENTIFIER { 
                    e_table_entry_ret returned_val = e_table_load_entry(&global_sym_table, $1);
                    if(returned_val.status != E_STATUS_OK) {
                        switch(returned_val.status) {
                            case E_STATUS_NOTFOUND:
                            default:
                                yyerror("Trying to assign to unknown identifier\n");
                                break;
                            case E_STATUS_NOINIT:
                                yyerror("Symbol table not inizialized\n");
                                break;
                        }
                    }
                    
                    if(returned_val.svalue.argtype == E_ARGT_INT) {
                        $$ = returned_val.svalue.val.ival;
                    } else if(returned_val.svalue.argtype == E_ARGT_FLOAT) {
                        $$ = (int)returned_val.svalue.val.fval;
                    } else {
                        // TODO: Implicit cast from string? $$ = atoi();
                        yyerror("Cannot use non-numerical type here\n");
                    }
                }
                | int_expression MULTIPLY int_expression { $$ = $1 * $3; }
                | int_expression DIVIDE int_expression { $$ = $1 / $3; }
                | int_expression PLUS int_expression { $$ = $1 + $3; }
                | int_expression MINUS int_expression { $$ = $1 - $3; }
                | MINUS int_expression { $$ = -$2; }
                | PLUS int_expression { $$ = $2; }
                ;
%%

void yyerror(const char* s) {
    printf("Error happend: %s\n", s);
    exit(-1);
}
