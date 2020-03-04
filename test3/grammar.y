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
    static int ds_store_string(const char* str);
    static int ds_get_size(void);
    
    void print_outstream(void);
    
    static int addr_count = 0;
    static uint8_t out_bytes[E_OUT_TOTAL_SIZE];
    static unsigned int out_b_cnt = 0;
    static unsigned int out_ds_cnt = E_OUT_SIZE + 1;
    
    unsigned int lc = 0;
    
    bool loop_has_break[E_LOCAL_SYM_TAB_SCOPES];

    bool ignore_next_push = false;
%}


%union {
    e_type nval;
    char* sname;
}

%token<nval> NUMBER STRING
%token<sname> IDENTIFIER
%token ASSIGN EQUALS
%token AND OR NOT
%token REL_LT REL_LTEQ REL_NOTEQ REL_EQ REL_GTEQ REL_GT
%token PLUS MINUS
%token MOD
%token MULTIPLY DIVIDE
%token P_OPEN P_CLOSE

%token BLOCK_IF BLOCK_THEN BLOCK_ELSE BLOCK_ELSEIF BLOCK_ENDIF
%token LOOP_REPEAT LOOP_FOREVER LOOP_BREAK

%token NEWLINE
%token PRINT_BYTES
%token PRINT

%left AND OR NOT
%left REL_LT REL_LTEQ REL_NOTEQ REL_EQ REL_GTEQ REL_GT
%left PLUS MINUS
%left MOD
%left MULTIPLY DIVIDE
%left P_OPEN P_CLOSE

%type<nval> number
%type<nval> assignable_expression
%type<nval> math_expression
%type<nval> string_expression

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
                printf("BREAK at level %d -> [%d]!\n", loop_level, addr_count);

				// Insert JMP [64 bit dummy_addr] to be patched later
                emit_op(e_create_operation(E_OP_JMP, e_create_number(0xFFFFFFFF), e_create_number(0xFFFFFFFF)));

				e_internal_type addr =  { .ival = addr_count };
				e_stack_status_ret s = e_stack_push(&loop_stack, addr);
				loop_has_break[loop_level] = true;
            }
            | PRINT_BYTES { print_outstream(); }
            | print_expression
            ;
            
line_sep: NEWLINE { lc++; };

assign: IDENTIFIER EQUALS assignable_expression {
                    /* Change value of number type variable, a = 3 */
                    // PUSHG $3 [index] (value, index)
                    // PUSHL $3 [index] (value, index)
                    e_opcode op;
                    e_status_ret s;
					e_table_value v;
					e_table_value opv;
					e_table* tab = &global_sym_table;

                    s = e_table_find_entry(&global_sym_table, $1);
                    op = E_OP_PUSHG;

                    if(s.status != E_STATUS_OK) {
        				s = e_table_find_entry(&local_sym_table[scope_level], $1);
        				op = E_OP_PUSHL;
        				tab = &local_sym_table[scope_level];
                    }

                    if(s.status == E_STATUS_OK) {
                        int gst_index = s.ival;

                        printf("reassign -> argtype expr: %d\n", $3.type);
						if($3.type == E_NUMBER) {
							printf("is number \n");
							v = e_create_number($3.val);
							opv = e_create_number(E_ARGT_NUMBER);
						} else if($3.type == E_STRING) {
							printf("is string \n");
							v = e_create_string($3.str.sval, $3.str.str_index);
							opv = e_create_number(E_ARGT_STRING);
						}

						printf("changing entry %s\n", $1);
						s = e_table_add_entry(tab, $1, v);

						if(s.status == E_STATUS_OK) {
							printf("------ OK -------\n");
                        	emit_op(e_create_operation(op, e_create_number(gst_index), opv));
                        } else {
					  		error_pprint(s.status);
					  	}
                    } else {
                        error_pprint(s.status);
                    }
        }
		| ASSIGN IDENTIFIER EQUALS assignable_expression {
            /* Number type (integer|float) definition with initialization, let x = 42 */
            e_opcode op;
            e_status_ret s;
            e_table_value v;
            e_table_value opv;

            printf("argtype expr: %d\n", $4.type);
            if($4.type == E_NUMBER) {
				v = e_create_number($4.val);
				opv = e_create_number(E_ARGT_NUMBER);
			} else if($4.type == E_STRING) {
				v = e_create_string($4.str.sval, $4.str.str_index);
                opv = e_create_number(E_ARGT_STRING);
			}

            if(scope_level == 0) {
                // PUSHG [index]
                s = e_table_add_entry(&global_sym_table, $2, v);
                op = E_OP_PUSHG;
            } else {
                // PUSHL [index]
                printf("Push local %s into scope %d\n", $2, scope_level);
                s = e_table_add_entry(&local_sym_table[scope_level], $2, v);
                op = E_OP_PUSHL;
            }

            if(s.status == E_STATUS_OK) {
                emit_op(e_create_operation(op, e_create_number(s.ival), opv));
            } else {
                error_pprint(s.status);
            }
        }
        ;

assignable_expression: string_expression {
							if(!ignore_next_push) {
								// Add string data to data segment (bytecode section)
								int str_index = ds_store_string($1.str.sval);
								// This is totally independent from the variable's scope (both, global and local strings are stored in the
								// data section, only the variable's visibility is scope bound
								emit_op(e_create_operation(E_OP_PUSH, e_create_number(str_index), e_create_null()));
							} else {
								ignore_next_push = false;
							}
					   }
				   	   | math_expression
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

                        $$.type = s.argtype;
                        printf("identifier arg type: %d\n", s.argtype);
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
				| math_expression MOD math_expression {
					/* 3 % a */
					emit_op(e_create_operation(E_OP_MOD, e_create_null(), e_create_null()));
				}
                | math_expression PLUS math_expression { 
                    /* 3 + a */
					// Numbers result in an add operation
					if($1.type == E_ARGT_STRING || $3.type == E_ARGT_STRING) {
						printf("addign two string identifiers\n");

						$$.type = E_STRING;

						// If one expression is of type number, we can't create a new string while compiling,
						// as number values are not stored in the compiling process!
						// So the VM needs to build the string while runtime
						emit_op(e_create_operation(E_OP_CONCAT, e_create_null(), e_create_number(E_CONCAT_BOTH)));
					} else {
						// Number addition
						emit_op(e_create_operation(E_OP_ADD, e_create_null(), e_create_null()));
					}
                }
                | math_expression MINUS math_expression { 
                    /* 3 - a */
                    if($1.type == E_ARGT_STRING || $3.type == E_ARGT_STRING) {
                    	yyerror("Cannot substract string(s)");
                    }
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

string_expression: STRING {
						if(strlen($1.str.sval) >= E_MAX_STRLEN) {
							yyerror("String too long");
						}
						printf("Single string: %s\n", $1.str.sval);
						$$ = $1;
				   }
				   | string_expression PLUS string_expression {
				   		printf("string plus string_expr (%s) (%s)\n", $1.str.sval, $3.str.sval);

				   		char buf[E_MAX_STRLEN];

				   		unsigned int slen1 = strlen($1.str.sval);
				   		unsigned int slen2 = strlen($3.str.sval);
				   		if(slen1 + slen2 > E_MAX_STRLEN) {
				   			yyerror("Cannot concatenate strings (result string is too long)");
				   		}
				   		strcpy(buf, $1.str.sval);
				   		strcat(buf, $3.str.sval);

				   		$$.str.sval = buf;
				   }
				   | string_expression PLUS math_expression {
				   		printf("string plus math_expr\n");

						// Add string data to data segment (bytecode section)
						int str_index = ds_store_string($1.str.sval);
						// This is totally independent from the variable's scope (both, global and local strings are stored in the
						// data section, only the variable's visibility is scope bound
						emit_op(e_create_operation(E_OP_PUSH, e_create_number(str_index), e_create_null()));

						$$.type = E_STRING;
						$$.str.sval = strdup($1.str.sval);
						$$.str.str_index = str_index;

						// If one expression is of type number, we can't create a new string while compiling,
						// as number values are not stored in the compiling process!
						// So the VM needs to build the string while runtime
						if($1.type == E_ARGT_STRING && $3.type == E_ARGT_STRING) {
							emit_op(e_create_operation(E_OP_CONCAT, e_create_null(), e_create_number(E_CONCAT_BOTH)));

							// There's a false PUSH OP after the concat that we need to ignore (generated by the string concat itself in the string_expression rule)
						} else{
							emit_op(e_create_operation(E_OP_CONCAT, e_create_null(), e_create_number(E_CONCAT_SECOND)));
						}
						// There's a false PUSH OP after the concat that we need to ignore (generated by the string concat itself in the string_expression rule)
                        ignore_next_push = true;
				   }
				   | math_expression PLUS string_expression {
						printf("math plus string\n");

						// Add string data to data segment (bytecode section)
						int str_index = ds_store_string($3.str.sval);
						// This is totally independent from the variable's scope (both, global and local strings are stored in the
						// data section, only the variable's visibility is scope bound
						emit_op(e_create_operation(E_OP_PUSH, e_create_number(str_index), e_create_null()));

						$$.type = E_STRING;
						$$.str.sval = strdup($3.str.sval);
						$$.str.str_index = str_index;

						// If one expression is of type number, we can't create a new string while compiling,
						// as number values are not stored in the compiling process!
						// So the VM needs to build the string while runtime
						if($1.type == E_ARGT_STRING && $3.type == E_ARGT_STRING) {
							emit_op(e_create_operation(E_OP_CONCAT, e_create_null(), e_create_number(E_CONCAT_BOTH)));
						} else {
							emit_op(e_create_operation(E_OP_CONCAT, e_create_null(), e_create_number(E_CONCAT_FIRST)));
						}
						// There's a false PUSH OP after the concat that we need to ignore (generated by the string concat itself in the string_expression rule)
                        ignore_next_push = true;
				   }
				   ;
                
if_expression: if_condition expression_list if_elseif if_endifelse
			   | if_condition expression_list if_endifelse;

if_condition: BLOCK_IF math_expression if_blockthen {
					printf("IF BLOCK****\n");

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

if_endifelse: block_endif
			  | if_blockelse expression_list block_endif {
					// Get instruction count of opening if
					e_stack_status_ret s = e_stack_pop(&bp_stack);
					if(s.status == E_STATUS_OK) {
						// Patch jump dummy_addr from previous jump
						jmp_patch(s.val.ival, addr_count);
					}
					//error_pprint(s.status);
			  }
			  ;

if_elseif: if_elseif if_elseif_expression
		   | if_elseif_expression
		   ;

if_elseif_expression: elseif_condition expression_list;

block_endif: BLOCK_ENDIF {
					printf("ENDIF************\n");

					// Get instruction count of opening if
					e_stack_status_ret s = e_stack_pop(&bp_stack);
					if(s.status == E_STATUS_OK) {
						// Patch jump dummy_addr from previous jump
						jmp_patch(s.val.ival, addr_count);
					}
					e_status_ret s_scope = e_close_scope();
					//error_pprint(s.status);
					//error_pprint(s_scope.status);
				}
				;

elseif_condition: if_blockelseif math_expression if_blockthen {
					// Insert JNE [16 bit dummy_addr]
					emit_op(e_create_operation(E_OP_JZ, e_create_number(0xFFFFFFFF), e_create_number(0xFFFFFFFF)));

					// Copy jmp instruction to a table (to be patched later in the if_expression)
					e_internal_type addr =  { .ival = addr_count };
					e_stack_status_ret s2 = e_stack_push(&bp_stack, addr);
				 }
				 ;

if_blockelseif: BLOCK_ELSEIF {
						printf("ELSEIF BLOCK (line: %d)****\n", addr_count);

						// Get instruction count of opening if
						e_stack_status_ret s = e_stack_pop(&bp_stack);
						if(s.status == E_STATUS_OK) {
							// Patch jump dummy_addr from previous jump
							jmp_patch(s.val.ival, addr_count);
						}
						error_pprint(s.status);
			   }
			   ;

if_blockelse: if_else {
					printf("ELSE BLOCK****\n");

					// Get instruction count of opening if
					e_stack_status_ret s = e_stack_pop(&bp_stack);
					if(s.status == E_STATUS_OK) {
						// Patch jump dummy_addr from previous jump
						jmp_patch(s.val.ival, addr_count + 1);
					}
					error_pprint(s.status);

					// Insert JNE [16 bit dummy_addr]
					emit_op(e_create_operation(E_OP_JMP, e_create_number(0xFFFFFFFF), e_create_number(0xFFFFFFFF)));

					// Copy jmp instruction to a table (to be patched later in the if_expression)
					e_internal_type addr =  { .ival = addr_count };
					e_stack_status_ret s2 = e_stack_push(&bp_stack, addr);
					}
					;

if_else:  BLOCK_ELSE
		  | BLOCK_ELSE line_sep
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

						if(loop_has_break[loop_level]) {
							// Patch break
							// Get instruction count of opening if
							e_stack_status_ret s = e_stack_pop(&loop_stack);
							if(s.status == E_STATUS_OK) {
								jmp_patch(s.val.ival, addr_count);
								printf("patched loop break @%d with new address: %d\n", s.val.ival, addr_count);
							}
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

print_expression: PRINT P_OPEN assignable_expression P_CLOSE {
						if($3.type == E_STRING) {
							emit_op(e_create_operation(E_OP_PRINT, e_create_null(), e_create_number(E_ARGT_STRING)));
						} else if($3.type == E_NUMBER) {
							emit_op(e_create_operation(E_OP_PRINT, e_create_null(), e_create_number(E_ARGT_NUMBER)));
						}
				  }
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
            printf("PUSHG [%d]\n", (int)op.op1.val);
            byte_op.op1 = (uint32_t)op.op1.val;
            byte_op.op2 = (uint32_t)op.op2.val;
            break;
        case E_OP_POPG:
            printf("POPG [%d]\n", (int)op.op1.val);
            byte_op.op1 = (uint32_t)op.op1.val;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_PUSHL:
            printf("PUSHL [%d]\n", (int)op.op1.val);
            byte_op.op1 = (uint32_t)op.op1.val;
            byte_op.op2 = (uint32_t)op.op2.val;
            
            printf("************************# SYMBOL TABLE [%d] **\n", scope_level);
            for(unsigned int i=0; i < local_sym_table[scope_level].entries; i++) {

                printf("[%d] %s (type: %d)\n", i, local_sym_table[scope_level].tab_ptr[i].idname, local_sym_table[scope_level].tab_ptr[i].svalue.argtype);
            }
            printf("****************************************\n");
            
            break;
        case E_OP_POPL:
            printf("POPL [%d]\n", (int)op.op1.val);
            byte_op.op1 = (uint32_t)op.op1.val;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_PUSH:
            switch(op.op1.argtype) {
                case E_ARGT_NUMBER:
                    printf("PUSH %f\n", op.op1.val);
                    double_to_bytearray(op.op1.val, barr);
                    byte_op.op1 = (barr[7] << 24) | (barr[6] << 16) | (barr[5] << 8) | barr[4];
                    byte_op.op2 = (barr[3] << 24) | (barr[2] << 16) | (barr[1] << 8) | barr[0];
                    break;
                }
            break;
        case E_OP_EQ:
            printf("EQ\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_LT:
            printf("LT\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_GT:
            printf("GT\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_LTEQ:
            printf("LTEQ\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_GTEQ:
            printf("GTEQ\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_ADD:
            printf("ADD\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_SUB:
            printf("SUB\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_MUL:
            printf("MUL\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_DIV:
            printf("DIV\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_AND:
            printf("AND\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
		case E_OP_MOD:
			printf("MOD\n");
			byte_op.op1 = (uint32_t)0;
			byte_op.op2 = (uint32_t)0;
			break;
        case E_OP_OR:
            printf("OR\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
		case E_OP_CONCAT:
			printf("CONCAT\n");
			byte_op.op1 = (uint32_t)0;
			byte_op.op2 = (uint32_t)op.op2.val;
			break;
        case E_OP_JZ:
            printf("JZ [%d %d]\n", (int)op.op1.val, (int)op.op2.val);
            byte_op.op1 = (uint32_t)op.op1.val;
            byte_op.op2 = (uint32_t)op.op2.val;
            break;
        case E_OP_JMP:
            byte_op.op1 = (uint32_t)op.op1.val;
            byte_op.op2 = (uint32_t)op.op2.val;
            break;
		case E_OP_PRINT:
			printf("PRINT\n");
			byte_op.op1 = (uint32_t)0;
			byte_op.op2 = (uint32_t)op.op2.val;
			break;
        }
        
        //printf("-----(out_b_cnt: %d)\n", out_b_cnt);
        if(out_b_cnt + E_OP_BSIZE >= E_OUT_TOTAL_SIZE) {
        	yyerror("Program space exhausted");
        }
        
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

int ds_store_string(const char* str) {
    // Stores a string in the required format [LENGTH, 2 Bytes][<str data>]
    uint32_t r_len = strlen(str);
    if(r_len > UINT16_MAX || out_ds_cnt + r_len > E_OUT_TOTAL_SIZE) {
    	yyerror("String literal is too large");
    } else {
    	uint32_t start_index = out_ds_cnt;
    	uint16_t len = (uint16_t)r_len;

    	out_bytes[out_ds_cnt++] = (uint8_t)((len >> 8) & 0xFF);
    	out_bytes[out_ds_cnt++] = (uint8_t)(len & 0xFF);
    	for(uint16_t i = 0; i < len; i++) {
    		out_bytes[out_ds_cnt++] = (uint8_t)str[i];
    	}
    	return start_index;
    }
}

int
ds_get_size(void) {
	uint32_t i = 1;
	uint32_t offset = E_OUT_SIZE + i;

	if(i + 1 < E_OUT_SIZE) {
		uint16_t size = (out_bytes[offset] << 8) | out_bytes[offset+1];
		while(size != 0) {
			i = i + size + 2;
			offset = E_OUT_SIZE + i;
			size = (out_bytes[offset] << 8) | out_bytes[offset+1];
		}
	}
	return i;
}

void print_outstream(void) {
    printf("print stream *****\n");
    unsigned int r = 0;
    unsigned int print_out = 0;
    unsigned int ds = 0;
    for(unsigned int i = 0; i < E_OUT_TOTAL_SIZE; i++) {
        printf("0x%02X ", out_bytes[i]);
        
        //if(!print_out && (i == E_OUT_SIZE || (r == 0 && out_bytes[i] == 0x00))) {
       	if(!print_out && i == E_OUT_SIZE) {
            // Opcode 0x00, end
			// printf("\n -- DATA SEGMENT -- \n");
			//i = E_OUT_SIZE;
			print_out = 1;
        }

        //if(print_out) {
        //	ds++;
        //	if(ds == ds_get_size()) {
        //		break;
        //	}
        //}
    
        if(++r == 9) {
            printf("\n");
            r = 0;
        }
    }
    printf("\n");
}
