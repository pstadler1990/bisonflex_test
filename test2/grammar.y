%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "evoscript.h"
    
    extern int yylex();
    extern int yyparse();
    
    extern FILE* yyin;
    
    void yyerror(const char* s);
    
    static void error_pprint(e_statusc error);
%}


%union {
    e_number nval;
    char* sname;
}


%token<nval> INT
%token<nval> FLOAT
%token<sname> IDENTIFIER
%token ASSIGN EQUALS
%token PLUS MINUS
%token MULTIPLY DIVIDE
%token NEWLINE
%token GL_SYM_DUMP

%left PLUS MINUS
%left MULTIPLY DIVIDE

%type<nval> number
%type<nval> math_expression


%%
prgm:
    | prgm line
    ;   
    
line: NEWLINE
    | expression NEWLINE { printf("ok\n"); }
    
expression: assign
            | math_expression { printf(">\n"); }
            | GL_SYM_DUMP { e_table_memdump(&global_sym_table); }
            ;
            
assign: ASSIGN IDENTIFIER EQUALS math_expression { 
            /* Number type (integer|float) definition with initialization, let x = 42 */
            e_statusc status;
            switch($4.type) {
                case E_INTEGER:
                    status = e_table_add_entry(&global_sym_table, $2, e_create_int($4.ival));
                    break;
                case E_FLOAT:
                    status = e_table_add_entry(&global_sym_table, $2, e_create_float($4.fval));
                    break;
                default:
                    yyerror("Unsupported number type");
            }
            error_pprint(status);
        }
        | IDENTIFIER EQUALS math_expression {
            /* Change value of number type variable, a = 3 */
            e_statusc status;
            switch($3.type) {
                case E_INTEGER:
                    status = e_table_change_entry(&global_sym_table, $1, e_create_int($3.ival));
                    break;
                case E_FLOAT:
                    status = e_table_change_entry(&global_sym_table, $1, e_create_float($3.fval));
                    break;
                default:
                    yyerror("Unsupported number type");
            }
            error_pprint(status);
        }
        ;

math_expression: number 
                | IDENTIFIER { 
                    e_table_entry_ret returned_val = e_table_load_entry(&global_sym_table, $1);
                    error_pprint(returned_val.status);
                    
                    if(returned_val.svalue.argtype == E_ARGT_INT) {
                        $$.type = 0;
                        $$.ival = returned_val.svalue.val.ival;
                    } else if(returned_val.svalue.argtype == E_ARGT_FLOAT) {
                        $$.type = 1;
                        $$.fval = returned_val.svalue.val.fval;
                    } else {
                        // TODO: Implicit cast from string? $$ = atoi();
                        yyerror("Cannot use non-numerical type here\n");
                    }
                }
                | math_expression MULTIPLY math_expression { $$.ival = $1.ival * $3.ival; }
                | math_expression DIVIDE math_expression { $$.ival = $1.ival / $3.ival; }
                | math_expression PLUS math_expression { $$.ival = $1.ival + $3.ival; }
                | math_expression MINUS math_expression { $$.ival = $1.ival - $3.ival; }
                | MINUS math_expression { $$.ival = -$2.ival; }
                | PLUS math_expression { $$.ival = $2.ival; }
                ;
                  
number: INT { $$.ival = (int)$1.ival; }
        |FLOAT { $$.fval = (float)$1.fval; }
        ;
%%

void yyerror(const char* s) {
    printf("Error happend: %s\n", s);
    exit(-1);
}

void error_pprint(e_statusc error) {
    if(error != E_STATUS_OK) {
        switch(error) {
            case E_STATUS_NOINIT:
                yyerror("Symbol not inizialized\n");
                break;
            case E_STATUS_NESIZE:
                yyerror("Not enough size\n");
                break;
            case E_STATUS_ALRDYDEF:
                yyerror("Symbol already defined\n");
                break;
            case E_STATUS_NOTFOUND:
                yyerror("Symbol not defined\n");
                break;
            default:
                yyerror("Syntax error");
        }
    }
}
