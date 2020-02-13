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
    e_number nval;
    char* sname;
}


%token<nval> INT
%token<nval> FLOAT
%token<sname> IDENTIFIER
%token ASSIGN EQUALS
%token AND OR NOT
%token REL_LT REL_LTEQ REL_NOTEQ REL_EQ REL_GTEQ REL_GT
%token PLUS MINUS
%token MULTIPLY DIVIDE
%token P_OPEN P_CLOSE
%token BLOCK_IF BLOCK_THEN BLOCK_ENDIF
%token NEWLINE
%token GL_SYM_DUMP

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
            | math_expression {
                if($1.type == E_INTEGER) {
                    printf("> %d\n", $1.ival);
                } else if($1.type == E_FLOAT) {
                    printf("> %f\n", $1.fval);    
                }
            }
            | if_expression
            | GL_SYM_DUMP { e_table_memdump(&global_sym_table); }
            ;
            
assign: ASSIGN IDENTIFIER EQUALS math_expression { 
            /* Number type (integer|float) definition with initialization, let x = 42 */
            // PUSHG $4, $2  [value, name]
            e_table_value op1;
            switch($4.type) {
                case E_INTEGER:
                    op1 = e_create_int($4.ival);
                    break;
                case E_FLOAT:
                    op1 = e_create_float($4.fval);
                    break;
            }
            
            e_status_ret s = e_table_add_entry(&global_sym_table, $2, op1);
            
            if(s.status == E_STATUS_OK) {
                printf("ival: %d\n", s.ival);
                emit_op(e_create_operation(E_OP_PUSHG, op1, e_create_int(s.ival)));
            } else {
                error_pprint(s.status);
            }
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
                        $$.type = E_INTEGER;
                        $$.ival = returned_val.svalue.ival;
                    } else if(returned_val.svalue.argtype == E_ARGT_FLOAT) {
                        $$.type = E_FLOAT;
                        $$.fval = returned_val.svalue.fval;
                    } else {
                        // TODO: Implicit cast from string? $$ = atoi();
                        yyerror("Cannot use non-numerical type here\n");
                    }
                }
                | math_expression REL_EQ math_expression {
                    /* a == b */
                    $$.type = E_INTEGER;
                    if($1.type == E_INTEGER) {
                        if($3.type == E_INTEGER) {
                            $$.ival = $1.ival == $3.ival;
                        } else if($3.type == E_FLOAT) {
                            $$.ival = $1.ival == (int)$3.fval;
                        }
                    } else if($1.type == E_FLOAT) {
                        if($3.type == E_INTEGER) {
                            $$.ival = (int)$1.fval == $3.ival;
                        } else if($3.type == E_FLOAT) {
                            $$.ival = (int)$1.fval == (int)$3.fval;
                        }
                    }
                }
                | math_expression REL_NOTEQ math_expression {
                    /* a != b */
                    $$.type = E_INTEGER;
                    if($1.type == E_INTEGER) {
                        if($3.type == E_INTEGER) {
                            $$.ival = $1.ival != $3.ival;
                        } else if($3.type == E_FLOAT) {
                            $$.ival = $1.ival != (int)$3.fval;
                        }
                    } else if($1.type == E_FLOAT) {
                        if($3.type == E_INTEGER) {
                            $$.ival = (int)$1.fval != $3.ival;
                        } else if($3.type == E_FLOAT) {
                            $$.ival = (int)$1.fval != (int)$3.fval;
                        }
                    }
                }
                | math_expression REL_LT math_expression {
                    /* a < b */
                    $$.type = E_INTEGER;
                    if($1.type == E_INTEGER) {
                        if($3.type == E_INTEGER) {
                            $$.ival = $1.ival < $3.ival;
                        } else if($3.type == E_FLOAT) {
                            $$.ival = $1.ival < (int)$3.fval;
                        }
                    } else if($1.type == E_FLOAT) {
                        if($3.type == E_INTEGER) {
                            $$.ival = (int)$1.fval < $3.ival;
                        } else if($3.type == E_FLOAT) {
                            $$.ival = (int)$1.fval < (int)$3.fval;
                        }
                    }
                }
                | math_expression REL_GT math_expression {
                    /* a > b */
                    $$.type = E_INTEGER;
                    if($1.type == E_INTEGER) {
                        if($3.type == E_INTEGER) {
                            $$.ival = $1.ival > $3.ival;
                        } else if($3.type == E_FLOAT) {
                            $$.ival = $1.ival > (int)$3.fval;
                        }
                    } else if($1.type == E_FLOAT) {
                        if($3.type == E_INTEGER) {
                            $$.ival = (int)$1.fval > $3.ival;
                        } else if($3.type == E_FLOAT) {
                            $$.ival = (int)$1.fval > (int)$3.fval;
                        }
                    }
                }
                | math_expression REL_LTEQ math_expression {
                    /* a <= b */
                    $$.type = E_INTEGER;
                    if($1.type == E_INTEGER) {
                        if($3.type == E_INTEGER) {
                            $$.ival = $1.ival <= $3.ival;
                        } else if($3.type == E_FLOAT) {
                            $$.ival = $1.ival <= (int)$3.fval;
                        }
                    } else if($1.type == E_FLOAT) {
                        if($3.type == E_INTEGER) {
                            $$.ival = (int)$1.fval <= $3.ival;
                        } else if($3.type == E_FLOAT) {
                            $$.ival = (int)$1.fval <= (int)$3.fval;
                        }
                    }
                }
                | math_expression REL_GTEQ math_expression {
                    /* a >= b */
                    $$.type = E_INTEGER;
                    if($1.type == E_INTEGER) {
                        if($3.type == E_INTEGER) {
                            $$.ival = $1.ival >= $3.ival;
                        } else if($3.type == E_FLOAT) {
                            $$.ival = $1.ival >= (int)$3.fval;
                        }
                    } else if($1.type == E_FLOAT) {
                        if($3.type == E_INTEGER) {
                            $$.ival = (int)$1.fval >= $3.ival;
                        } else if($3.type == E_FLOAT) {
                            $$.ival = (int)$1.fval >= (int)$3.fval;
                        }
                    }
                }
                | math_expression AND math_expression {
                    $$.type = E_INTEGER;
                    if($1.type == E_INTEGER) {
                        if($3.type == E_INTEGER) {
                            $$.ival = $1.ival & $3.ival;
                        } else if($3.type == E_FLOAT) {
                            $$.ival = $1.ival & (int)$3.fval;
                        }
                    } else if($1.type == E_FLOAT) {
                        if($3.type == E_INTEGER) {
                            $$.ival = (int)$1.fval & $3.ival;
                        } else if($3.type == E_FLOAT) {
                            $$.ival = (int)$1.fval & (int)$3.fval;
                        }
                    }
                }
                | math_expression OR math_expression {
                    $$.type = E_INTEGER;
                    if($1.type == E_INTEGER) {
                        if($3.type == E_INTEGER) {
                            $$.ival = $1.ival | $3.ival;
                        } else if($3.type == E_FLOAT) {
                            $$.ival = $1.ival | (int)$3.fval;
                        }
                    } else if($1.type == E_FLOAT) {
                        if($3.type == E_INTEGER) {
                            $$.ival = (int)$1.fval | $3.ival;
                        } else if($3.type == E_FLOAT) {
                            $$.ival = (int)$1.fval | (int)$3.fval;
                        }
                    }
                }
                | NOT math_expression {
                    $$.type = E_INTEGER;
                    if($2.type == E_INTEGER) {
                        $$.ival = !$2.ival;
                    } else if($2.type == E_FLOAT) {
                        $$.ival = !$2.fval;
                    }
                }
                | P_OPEN math_expression P_CLOSE {
                    $$ = $2;
                }
                | math_expression MULTIPLY math_expression { 
                    if($1.type == E_INTEGER && $3.type == E_INTEGER) {
                        $$.type = E_INTEGER;
                        $$.ival = $1.ival * $3.ival;
                    } else {
                        $$.type = E_FLOAT;
                        if($1.type == E_INTEGER) {
                           $$.fval = $1.ival * $3.fval; 
                        } else if($3.type == E_INTEGER) {
                            $$.fval = $1.fval * $3.ival; 
                        } else {
                            $$.fval = $1.fval * $3.fval;
                        }
                    }
                }
                | math_expression DIVIDE math_expression { 
                    /* 3 / a */
                    if($1.type == E_INTEGER && $3.type == E_INTEGER) {
                        $$.type = E_INTEGER;
                        $$.ival = $1.ival / $3.ival;
                    } else {
                        $$.type = E_FLOAT;
                        if($1.type == E_INTEGER) {
                           $$.fval = $1.ival / $3.fval; 
                        } else if($3.type == E_INTEGER) {
                            $$.fval = $1.fval / $3.ival; 
                        } else {
                            $$.fval = $1.fval / $3.fval;
                        }
                    }
                }
                | math_expression PLUS math_expression { 
                    /* 3 + a */
                    if($1.type == E_INTEGER && $3.type == E_INTEGER) {
                        $$.type = E_INTEGER;
                        $$.ival = $1.ival + $3.ival;
                    } else {
                        $$.type = E_FLOAT;
                        if($1.type == E_INTEGER) {
                           $$.fval = $1.ival + $3.fval; 
                        } else if($3.type == E_INTEGER) {
                            $$.fval = $1.fval + $3.ival; 
                        } else {
                            $$.fval = $1.fval + $3.fval;
                        }
                    }
                }
                | math_expression MINUS math_expression { 
                    /* 3 - a */
                    if($1.type == E_INTEGER && $3.type == E_INTEGER) {
                        $$.type = E_INTEGER;
                        $$.ival = $1.ival - $3.ival;
                    } else {
                        $$.type = E_FLOAT;
                        if($1.type == E_INTEGER) {
                           $$.fval = $1.ival - $3.fval; 
                        } else if($3.type == E_INTEGER) {
                            $$.fval = $1.fval - $3.ival; 
                        } else {
                            $$.fval = $1.fval - $3.fval;
                        }
                    }
                }
                /*| MINUS math_expression {
                    if($2.type == E_INTEGER) {
                        $$.type = E_INTEGER; 
                        $$.ival = -$2.ival;
                    } else if($2.type == E_FLOAT) {
                        $$.type = E_FLOAT; 
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

void emit_op(e_op op) {
    /* Emits (prints) an OP with up to 2 args */
    switch(op.opcode) {
        case E_OP_PUSHG:
            switch(op.op1.argtype) {
                case E_INTEGER:
                    printf("PUSHG %d, [%d]\n", op.op1.ival, op.op2.ival);
                    break;
                case E_FLOAT:
                    printf("PUSHG %f, [%d]\n", op.op1.fval, op.op2.ival);
                    break;
            }
            break;
        }
}
