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
            // PUSHG $4, $2  (value, index)
            e_table_value op1;
            switch($4.type) {
                case E_INTEGER:
                    op1 = e_create_int($4.ival);
                    break;
                case E_FLOAT:
                    op1 = e_create_float($4.fval);
                    break;
                default:
                    yyerror("Unsupported number type");
            }
            
            e_status_ret s = e_table_add_entry(&global_sym_table, $2, op1);
            
            if(s.status == E_STATUS_OK) {
                emit_op(e_create_operation(E_OP_PUSHG, e_create_int(s.ival), e_create_null()));
            } else {
                error_pprint(s.status);
            }
        }
        | IDENTIFIER EQUALS math_expression {
            /* Change value of number type variable, a = 3 */
            // PUSHG $3 [index] (value, index)
            e_status_ret s = e_table_find_entry(&global_sym_table, $1);
            
            if(s.status == E_STATUS_OK) {
                e_table_value op1;
                int gst_index = s.ival;
                
                switch($3.type) {
                    case E_INTEGER:
                        op1 = e_create_int($3.ival);
                        break;
                    case E_FLOAT:
                        op1 = e_create_float($3.fval);
                        break;
                    default:
                        yyerror("Unsupported number type");
                }
                emit_op(e_create_operation(E_OP_PUSHG, e_create_int(gst_index), e_create_null()));
            } else {
                error_pprint(s.status);
            }
        }
        ;

math_expression: number {
                    // PUSH [number]
                    e_table_value op1;
                    
                    switch($1.type) {
                        case E_INTEGER:
                            op1 = e_create_int($1.ival);
                            break;
                        case E_FLOAT:
                            op1 = e_create_float($1.fval);
                            break;
                    }
                    emit_op(e_create_operation(E_OP_PUSH, op1, e_create_null()));
                }
                | IDENTIFIER { 
                    // POPG [index]
                    e_status_ret s = e_table_find_entry(&global_sym_table, $1);
                    
                    if(s.status == E_STATUS_OK) {
                        int gst_index = s.ival;
                        emit_op(e_create_operation(E_OP_POPG, e_create_int(gst_index), e_create_null()));
                    } else {
                        error_pprint(s.status);
                    }
                }
                | math_expression REL_EQ math_expression {
                    /* a == b */
                    // ADD
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
         
number: INT { $$.type = E_INTEGER; $$.ival = (int)$1.ival; }
        |FLOAT { $$.type = E_FLOAT; $$.fval = (float)$1.fval; }
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
            printf("PUSHG [%d]\n", op.op1.ival);
            break;
        case E_OP_POPG:
            printf("POPG [%d]\n", op.op1.ival);
            break;
        case E_OP_PUSH:
            switch(op.op1.argtype) {
                case E_ARGT_INT:
                    printf("PUSHi %d\n", op.op1.ival);
                    break;
                case E_ARGT_FLOAT:
                    printf("PUSHf %f\n", op.op1.fval);
                    break;
                }
            break;
        }
}
