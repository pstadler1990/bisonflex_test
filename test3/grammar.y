%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "evoscript.h"
    
    extern int yylex();
    extern int yyparse();
    
    extern FILE* yyin;
    
    void yyerror(const char* s);
    
    static void error_pprint(e_statusc error);
    static void emit_op(e_op op); 
%}


%union {
    e_type nval;
    char* sname;
}

%token<nval> NUMBER
%token<sname> IDENTIFIER
%token ASSIGN EQUALS
%token AND OR NOT
%token REL_LT REL_LTEQ REL_NOTEQ REL_EQ REL_GTEQ REL_GT
%token PLUS MINUS
%token MULTIPLY DIVIDE
%token P_OPEN P_CLOSE
%token BLOCK_IF BLOCK_THEN BLOCK_ENDIF
%token NEWLINE

%left AND OR NOT
%left REL_LT REL_LTEQ REL_NOTEQ REL_EQ REL_GTEQ REL_GT
%left PLUS MINUS
%left MULTIPLY DIVIDE
%left P_OPEN P_CLOSE

%type<nval> number
%type<nval> math_expression


%%
prgm: expression_list
    ;   

expression_list: expression NEWLINE expression_list
                 | expression
                 ;
    
expression: assign
            | math_expression
            | if_expression
            ;
            
assign: ASSIGN IDENTIFIER EQUALS math_expression { 
            /* Number type (integer|float) definition with initialization, let x = 42 */
            // PUSHG [index]
            e_status_ret s = e_table_add_entry(&global_sym_table, $2, e_create_number($4.val));
            
            if(s.status == E_STATUS_OK) {
                emit_op(e_create_operation(E_OP_PUSHG, e_create_number(s.ival), e_create_null()));
            } else {
                error_pprint(s.status);
            }
        }
        | IDENTIFIER EQUALS math_expression {
            /* Change value of number type variable, a = 3 */
            // PUSHG $3 [index] (value, index)
            e_status_ret s = e_table_find_entry(&global_sym_table, $1);
            
            if(s.status == E_STATUS_OK) {
                int gst_index = s.ival;   
                emit_op(e_create_operation(E_OP_PUSHG, e_create_number(gst_index), e_create_null()));
            } else {
                error_pprint(s.status);
            }
        }
        ;

math_expression: number {
                    // PUSH [number]
                    emit_op(e_create_operation(E_OP_PUSH, e_create_number($1.val), e_create_null()));
                }
                | IDENTIFIER { 
                    // POPG [index]
                    e_status_ret s = e_table_find_entry(&global_sym_table, $1);
                    
                    if(s.status == E_STATUS_OK) {
                        int gst_index = s.ival;
                        emit_op(e_create_operation(E_OP_POPG, e_create_number(gst_index), e_create_null()));
                    } else {
                        error_pprint(s.status);
                    }
                }
                | math_expression REL_EQ math_expression {
                    /* a == b */
                    // PUSH a
                    // PUSH b
                    // EQ
                    emit_op(e_create_operation(E_OP_EQ, e_create_null(), e_create_null()));
                }
                | math_expression REL_NOTEQ math_expression {
                    /* a != b */
                    
                }
                | math_expression REL_LT math_expression {
                    /* a < b */
                    
                }
                | math_expression REL_GT math_expression {
                    /* a > b */
                    
                }
                | math_expression REL_LTEQ math_expression {
                    /* a <= b */
                   
                }
                | math_expression REL_GTEQ math_expression {
                    /* a >= b */
                    
                }
                | math_expression AND math_expression {
                    
                }
                | math_expression OR math_expression {
                    
                }
                | NOT math_expression {
                    
                }
                | P_OPEN math_expression P_CLOSE {
                    $$ = $2;
                }
                | math_expression MULTIPLY math_expression { 
                    
                }
                | math_expression DIVIDE math_expression { 
                    /* 3 / a */
                    
                }
                | math_expression PLUS math_expression { 
                    /* 3 + a */
                    
                }
                | math_expression MINUS math_expression { 
                    /* 3 - a */
                    
                }
                /*| MINUS math_expression {
                    if($2.type == E_INTEGER) {
                        $$.type = E_INTEGER; 
                        $$.ival = -$2.ival;
                    } else if($2.type == E_FLOAT) {
                        $$.type = E_FLOAT; e_arg_type
                        $$.fval = -$2.fval;
                    }
                }
                | PLUS math_expression { 
                    if($2.type == E_INTEGER) {
                        $$.type = E_INTEGER; 
                        $$.ival = $2.ival;
                    } else if($2.type == E_FLOAT) {
                        $$.type = E_FLOAT; 
                        $$.fval = $2.fval;
                    }
                }*/
                ;
                
if_expression: BLOCK_IF math_expression BLOCK_THEN expression_list BLOCK_ENDIF { 
                    /* if 1 then */
                    printf("If statement\n");
               }
               ;
         
number: NUMBER { $$.type = E_NUMBER; $$.val = $1.val; }
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

void emit_op(e_op op) {
    /* Emits (prints) an OP with up to 2 args */
    switch(op.opcode) {
        case E_OP_PUSHG:
            printf("PUSHG [%d]\n", (int)op.op1.val);
            break;
        case E_OP_POPG:
            printf("POPG [%d]\n", (int)op.op1.val);
            break;
        case E_OP_PUSH:
            switch(op.op1.argtype) {
                case E_ARGT_NUMBER:
                    printf("PUSH %f\n", op.op1.val);
                    break;
                }
            break;
        case E_OP_EQ:
            printf("EQ\n");
            break;
        }
}
