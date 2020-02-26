%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include "evoscript.h"
    #include <string.h>
    
    extern int yylex();
    extern int yyparse();
    
    extern FILE* yyin;
    
    void yyerror(const char* s);
    
    static void error_pprint(e_statusc error);
    static void emit_op(e_op op);
    static void inc_cnt(void);
    static void jmp_patch(unsigned int start_addr, unsigned int end_addr);
    
    static void double_to_bytearray(double din, uint8_t bin[]);
    
    void print_outstream(void);
    
    static int addr_count = 0;
    static uint8_t out_bytes[9999];
    static unsigned int out_b_cnt = 0;
    
    unsigned int lc = 0;
    
    bool loop_has_break[99];
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
%token LOOP_REPEAT LOOP_FOREVER LOOP_BREAK

%token NEWLINE
%token PRINT_BYTES

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

expression_list: expression line_sep expression_list
                 | expression
                 | expression line_sep
                 ;
    
expression: assign
            | math_expression
            | if_expression
            | loop_expression
            | LOOP_BREAK { 
                if(loop_level == 0) {
                    yyerror("Break without proper loop");
                }
                printf("BREAK at level %d -> [%d]!\n", loop_level, addr_count   );
                
                loop_has_break[loop_level] = true;
                
                // Insert JMP [64 bit dummy_addr]
                emit_op(e_create_operation(E_OP_JMP, e_create_number(0xFFFFFFFF), e_create_number(0xFFFFFFFF)));
                
                // Copy jmp instruction to a table (to be patched later in the loop expression)
                e_internal_type addr =  { .ival = addr_count };
                e_stack_status_ret s = e_stack_push(&bp_stack, addr);
            }
            | PRINT_BYTES { print_outstream(); }
            ;
            
line_sep: NEWLINE { lc++; printf("new line [%d]\n", lc); };
            
assign: ASSIGN IDENTIFIER EQUALS math_expression { 
            /* Number type (integer|float) definition with initialization, let x = 42 */
            e_opcode op;
            e_status_ret s;
            
            if(scope_level == 0) {
                // PUSHG [index]
                s = e_table_add_entry(&global_sym_table, $2, e_create_number($4.val));
                op = E_OP_PUSHG;
            } else {
                // PUSHL [index]
                printf("Push local %s into scope %d\n", $2, scope_level);
                s = e_table_add_entry(&local_sym_table[scope_level], $2, e_create_number($4.val));
                op = E_OP_PUSHL;
            }
            
            if(s.status == E_STATUS_OK) {
                emit_op(e_create_operation(op, e_create_number(s.ival), e_create_null()));
            } else {
                error_pprint(s.status);
            }
        }
        | IDENTIFIER EQUALS math_expression {
            /* Change value of number type variable, a = 3 */
            // PUSHG $3 [index] (value, index)
            // PUSHL $3 [index] (value, index)
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
                    /* Find and pop a global or local variable */
                    e_opcode op;
                    e_status_ret s;
                    
                    if(scope_level > 0) {
                        // POPL [index]
                        printf("Finding local %s in scope %d\n", $1, scope_level);
                        s = e_table_find_entry(&local_sym_table[scope_level], $1);
                        op = E_OP_POPL;
                    }
                    
                    if(scope_level == 0 || s.status == E_STATUS_NOTFOUND) {
                        printf("Finding global %s\n", $1);
                        // POPG [index]
                        s = e_table_find_entry(&global_sym_table, $1);
                        op = E_OP_POPG;
                    }
                    
                    if(s.status == E_STATUS_OK) {
                        int gst_index = s.ival;
                        emit_op(e_create_operation(op, e_create_number(gst_index), e_create_null()));
                    } else {
                        for(int i = 0; i < local_sym_table[scope_level].entries; i++) {
                            printf("\t%s\t%f\n", local_sym_table[scope_level].tab_ptr[i].idname, local_sym_table[scope_level].tab_ptr[i].svalue.val);
                        }
                    
                        error_pprint(s.status);
                    }
                }
                | math_expression REL_EQ math_expression {
                    /* a == b */
                    // EQ
                    emit_op(e_create_operation(E_OP_EQ, e_create_null(), e_create_null()));
                }
                | math_expression REL_NOTEQ math_expression {
                    /* a != b */
                    emit_op(e_create_operation(E_OP_EQ, e_create_null(), e_create_null()));
                    emit_op(e_create_operation(E_OP_NOT, e_create_null(), e_create_null()));
                }
                | math_expression REL_LT math_expression {
                    /* a < b */
                    // LT
                    emit_op(e_create_operation(E_OP_LT, e_create_null(), e_create_null()));
                }
                | math_expression REL_GT math_expression {
                    /* a > b */
                    // GT
                    emit_op(e_create_operation(E_OP_GT, e_create_null(), e_create_null()));
                }
                | math_expression REL_LTEQ math_expression {
                    /* a <= b */
                    // LTEQ
                    emit_op(e_create_operation(E_OP_LTEQ, e_create_null(), e_create_null()));
                }
                | math_expression REL_GTEQ math_expression {
                    /* a >= b */
                    // GTEQ
                    emit_op(e_create_operation(E_OP_GTEQ, e_create_null(), e_create_null()));
                }
                | math_expression AND math_expression {
                    emit_op(e_create_operation(E_OP_AND, e_create_null(), e_create_null()));
                }
                | math_expression OR math_expression {
                    emit_op(e_create_operation(E_OP_OR, e_create_null(), e_create_null()));
                }
                | NOT math_expression {
                    emit_op(e_create_operation(E_OP_NOT, e_create_null(), e_create_null()));
                }
                | P_OPEN math_expression P_CLOSE {
                    $$ = $2;
                }
                | math_expression MULTIPLY math_expression { 
                    emit_op(e_create_operation(E_OP_MUL, e_create_null(), e_create_null()));
                }
                | math_expression DIVIDE math_expression { 
                    /* 3 / a */
                    emit_op(e_create_operation(E_OP_DIV, e_create_null(), e_create_null()));
                }
                | math_expression PLUS math_expression { 
                    /* 3 + a */
                    emit_op(e_create_operation(E_OP_ADD, e_create_null(), e_create_null()));
                }
                | math_expression MINUS math_expression { 
                    /* 3 - a */
                    emit_op(e_create_operation(E_OP_SUB, e_create_null(), e_create_null()));
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
                
if_expression: if_condition expression_list BLOCK_ENDIF { 
                    // Get instruction count of opening if
                    e_stack_status_ret s = e_stack_pop(&bp_stack);
                    if(s.status == E_STATUS_OK) {
                        // Patch jump dummy_addr from previous jump
                        jmp_patch(s.val.ival, addr_count);
                    }
                    
                    printf("BLOCK END\n");
                    
                    e_status_ret s_scope = e_close_scope();
                    
                    error_pprint(s.status);
                    error_pprint(s_scope.status);
               }
               ;
               
if_condition: BLOCK_IF math_expression if_blockthen {
                    // Insert JNE [16 bit dummy_addr]
                    emit_op(e_create_operation(E_OP_JZ, e_create_number(0xFFFFFFFF), e_create_number(0xFFFFFFFF)));
                    
                    // Copy jmp instruction to a table (to be patched later in the if_expression)
                    e_internal_type addr =  { .ival = addr_count };
                    e_stack_status_ret s = e_stack_push(&bp_stack, addr);
                     
                    e_status_ret s_scope = e_create_scope();
                    if(s_scope.status == E_STATUS_OK) {
                        printf("created scope\n");
                    }
                    
                    error_pprint(s.status);
                    error_pprint(s_scope.status);
              }
              ;
              
if_blockthen: BLOCK_THEN
              | BLOCK_THEN line_sep
              ;
              
loop_expression: loop_begin expression_list LOOP_FOREVER {
                    printf("LOOP END\n");
                    
                    // Get instruction count of opening if
                    e_stack_status_ret s = e_stack_pop(&bp_stack);
                    if(s.status == E_STATUS_OK) {
                        printf("Patching loop address: %d\n", s.val.ival);
                        // Add unconditional jump (jmp) to previously stored address
                        // TODO: Support 64bit address
                        emit_op(e_create_operation(E_OP_JMP, e_create_number(s.val.ival), e_create_number(0xFFFFFFFF)));
                        
                        // Patch break (if any)
                        if(loop_has_break[loop_level]) {
                            // Get instruction count of opening if
                            e_stack_status_ret s = e_stack_pop(&bp_stack);
                            printf("patch loop break @%d\n", s.val.ival);
                            if(s.status == E_STATUS_OK) {
                                // Patch jump dummy_addr from previous jump
                                jmp_patch(s.val.ival, addr_count);
                            }
                            loop_has_break[loop_level] = false;
                        }
                        
                        e_status_ret s_scope = e_close_scope();
                        loop_level = scope_level;
                        
                        error_pprint(s_scope.status);
                    }
                
                    error_pprint(s.status);
                 }
                 ;

loop_begin: loop_repeat { 
                // Loop creates a new scope
                e_status_ret s_scope = e_create_scope();
                if(s_scope.status == E_STATUS_OK) {
                    printf("created scope\n");
                    
                    loop_level = scope_level;
                    loop_has_break[loop_level] = false;
                    
                    // Store the line counter of the opening loop block to patch in the closing block
                    e_internal_type addr =  { .ival = addr_count };
                    e_stack_status_ret s = e_stack_push(&bp_stack, addr);
                    
                    error_pprint(s.status);
                }
                error_pprint(s_scope.status);
            }
            ;
                 
loop_repeat: LOOP_REPEAT 
            | LOOP_REPEAT line_sep
            ;
         
number: NUMBER { $$.type = E_NUMBER; $$.val = $1.val; }
        ;
%%

void yyerror(const char* s) {
    printf("Error happend in line [%d]: %s\n", lc, s);
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

void inc_cnt(void) {
    addr_count++;
}

void jmp_patch(unsigned int start_addr, unsigned int end_addr) {
    /* Patches a conditional jump dummy 16 bit address with the given end_addr */
    printf("PATCH jump @%d with new addr: %d\n", start_addr, end_addr);
    // TODO: Replace magic numbers with sizeof or defined constant
    // TODO: This can me a macro!
    out_bytes[((start_addr - 1) * 9) + 1] = (uint8_t)((end_addr >> 24) & 0xFF);
    out_bytes[((start_addr - 1) * 9) + 2] = (uint8_t)((end_addr >> 16) & 0xFF);
    out_bytes[((start_addr - 1) * 9) + 3] = (uint8_t)((end_addr >> 8) & 0xFF);
    out_bytes[((start_addr - 1) * 9) + 4] = (uint8_t)(end_addr & 0xFF);
}

void emit_op(e_op op) {
    /* Emits (prints) an OP with up to 2 args */
    inc_cnt();
    //printf("[%d]", addr_count);
    uint8_t barr[E_SYS_SIZE_DOUBLE];
    
    e_byte_op byte_op = { .opcode = (uint8_t)op.opcode };
    
    switch(op.opcode) {
        case E_OP_PUSHG:
            //printf("PUSHG [%d]\n", (int)op.op1.val);
            byte_op.op1 = (uint32_t)op.op1.val;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_POPG:
            //printf("POPG [%d]\n", (int)op.op1.val);
            byte_op.op1 = (uint32_t)op.op1.val;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_PUSHL:
            //printf("PUSHL [%d]\n", (int)op.op1.val);
            byte_op.op1 = (uint32_t)op.op1.val;
            byte_op.op2 = (uint32_t)0;
            
            printf("************************ SYMBOL TABLE [%d] **\n", scope_level);
            for(unsigned int i=0; i < local_sym_table[scope_level].entries; i++) {
                printf("[%d] %s\n", i, local_sym_table[scope_level].tab_ptr[i].idname);
            }
            printf("****************************************\n");
            
            break;
        case E_OP_POPL:
            //printf("POPL [%d]\n", (int)op.op1.val);
            byte_op.op1 = (uint32_t)op.op1.val;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_PUSH:
            switch(op.op1.argtype) {
                case E_ARGT_NUMBER:
                    //printf("PUSH %f\n", op.op1.val);
                    double_to_bytearray(op.op1.val, barr);
                    byte_op.op1 = (barr[7] << 24) | (barr[6] << 16) | (barr[5] << 8) | barr[4];
                    byte_op.op2 = (barr[3] << 24) | (barr[2] << 16) | (barr[1] << 8) | barr[0];
                    break;
                }
            break;
        case E_OP_EQ:
            //printf("EQ\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_LT:
            //printf("LT\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_GT:
            //printf("GT\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_LTEQ:
            //printf("LTEQ\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_GTEQ:
            //printf("GTEQ\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_ADD:
            //printf("ADD\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_SUB:
            //printf("SUB\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_MUL:
            //printf("MUL\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_DIV:
            //printf("DIV\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_AND:
            //printf("AND\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_OR:
            //printf("OR\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_JZ:
            //printf("JZ [%d %d]\n", (int)op.op1.val, (int)op.op2.val);
            byte_op.op1 = (uint32_t)op.op1.val;
            byte_op.op2 = (uint32_t)op.op2.val;
            break;
        case E_OP_JMP:
            byte_op.op1 = (uint32_t)op.op1.val;
            byte_op.op2 = (uint32_t)op.op2.val;
            break;
        }
        
        //printf("-----(out_b_cnt: %d)\n", out_b_cnt);
        
        out_bytes[out_b_cnt++] = (uint8_t)byte_op.opcode;
        out_bytes[out_b_cnt++] = (uint8_t)((byte_op.op1 >> 24) & 0xFF);
        out_bytes[out_b_cnt++] = (uint8_t)((byte_op.op1 >> 16) & 0xFF);
        out_bytes[out_b_cnt++] = (uint8_t)((byte_op.op1 >> 8) & 0xFF);
        out_bytes[out_b_cnt++] = (uint8_t)(byte_op.op1 & 0xFF);
        
        out_bytes[out_b_cnt++] = (uint8_t)((byte_op.op2 >> 24) & 0xFF);
        out_bytes[out_b_cnt++] = (uint8_t)((byte_op.op2 >> 16) & 0xFF);
        out_bytes[out_b_cnt++] = (uint8_t)((byte_op.op2 >> 8) & 0xFF);
        out_bytes[out_b_cnt++] = (uint8_t)(byte_op.op2 & 0xFF);
}

void double_to_bytearray(double din, uint8_t bin[]) {
    union {
        double d;
        uint8_t b[E_SYS_SIZE_DOUBLE];
    } u;
    u.d = din;
    memcpy(bin, u.b, E_SYS_SIZE_DOUBLE);
}

void print_outstream(void) {
    printf("print stream *****\n");
    unsigned int r = 0;
    for(unsigned int i = 0; i < 999; i++) {
        if(i%9 == 0) {
            printf("[%d] ", i / 9);
        }
    
        printf("0x%02X ", out_bytes[i]);
        
        if(r == 0 && out_bytes[i] == 0x00) {
            // Opcode 0x00, end
            break;
        }
    
        if(++r == 9) {
            printf("\n");
            r = 0;
        }
        
        
    }
    printf("\n");
}
