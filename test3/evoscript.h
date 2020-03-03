#ifndef __EVOSCRIPT_H
#define __EVOSCRIPT_H

#include <stdint.h>
#include <stdio.h>

#define E_VERSION "0.01"
#define E_SYS_SIZE_DOUBLE   ((int)sizeof(double))
#define E_OUT_DS_SIZE       ((int)250/*500*/)
#define E_OUT_SIZE          ((int)500/*2000*/)
#define E_OUT_TOTAL_SIZE    ((int)E_OUT_DS_SIZE + E_OUT_SIZE)
#define E_OP_BSIZE          ((int)9)

typedef enum {
    E_STATUS_NESTING = -7,
    E_STATUS_UNDERFLOW = -6,
    E_STATUS_DATATMIS = -5,
    E_STATUS_NOTFOUND = -4,
    E_STATUS_ALRDYDEF = -3,
    E_STATUS_NESIZE = -2,
    E_STATUS_NOINIT = -1,
    E_STATUS_UNDEF = 0,
    E_STATUS_OK = 1,
} e_statusc;

typedef enum {
    E_CONCAT_FIRST,
    E_CONCAT_SECOND,
    E_CONCAT_BOTH
} e_concat_type;

#define E_MAX_STRLEN    ((int)1024)
typedef enum {
    E_ARGT_NULL = 2,
    E_ARGT_NUMBER = 0,
    E_ARGT_STRING = 1,
} e_arg_type;

typedef struct {
    int str_index; /* string index in .ds */
    char* sval;
    int slen;
} e_str_type;

typedef struct {
    enum { E_NUMBER, E_STRING } type;
    double val;
    e_str_type str;
} e_type;

typedef union {
    int ival;
    float fval;
    e_str_type sval;
} e_internal_type;

typedef struct {
    union {
        double val;
        e_str_type sval;
    };
    e_arg_type argtype;
} e_table_value;

#define E_TAB_ENTRY_USED    ((unsigned char)1)
#define E_TAB_ENTRY_FREE    ((unsigned char)0)

typedef struct table_entry {
    unsigned char used;
    char* idname;
    e_table_value svalue;
} e_table_entry;

typedef struct {
    e_statusc status;
    e_table_value svalue;
} e_table_entry_ret;

typedef struct {
    e_statusc status;
    e_arg_type argtype;
    int ival;
    float fval;
    e_str_type sval;
} e_status_ret;

// VM
typedef enum {
    E_OP_PUSHG = 0x10,     /* Push global variable,                    PUSHG [index],      s[-1]           */
    E_OP_POPG = 0x11,      /* Pop global variable,                     POPG [index]                        */
    E_OP_PUSHL = 0x12,     /* Push local variable,                     PUSHL [index]                       */
    E_OP_POPL = 0x13,      /* Pop local variable,                      POPG [index]                        */
    E_OP_PUSH = 0x14,      /* Push variable onto stack,                PUSH 3                              */
    
    E_OP_EQ = 0x20,        /* Equal check,                             EQ,                 s[-1]==s[-2]    */
    E_OP_LT = 0x21,        /* Less than,                               LT,                 s[-1]<s[-2]     */    
    E_OP_GT = 0x22,        /* Greater than,                            GT,                 s[-1]<s[-2]     */ 
    E_OP_LTEQ = 0x23,      /* Less than or equal,                      LTEQ,               s[-1]<=s[-2]    */ 
    E_OP_GTEQ = 0x24,      /* Greater than or equal,                   GTEQ,               s[-1]>=s[-2]    */ 
    
    E_OP_ADD = 0x30,
    E_OP_SUB = 0x32,
    E_OP_MUL = 0x33,
    E_OP_DIV = 0x34,
    E_OP_AND = 0x35,
    E_OP_OR = 0x36,
    E_OP_NOT = 0x37,
    E_OP_CONCAT = 0x38,    /* Concatenate strings                       CONCAT              s[s-1].[s-2]   */
    E_OP_MOD = 0x39,       /* Modulo                                    MOD                 s[-1] % s[-2]  */
    
    E_OP_JZ = 0x40,        /* Jump if zero,                            JZ [addr]                           */
    E_OP_JMP = 0x41,       /* Unconditional jump,                      JMP [addr]                          */

    E_OP_PRINT  = 0x50,    /* Print statement (debug)                  PRINT(expr)                         */
} e_opcode;

typedef struct {
    // Internal representation of an operation
    e_opcode opcode;
    // 2 args max
    e_table_value op1;
    e_table_value op2;
} e_op;

typedef struct {
    // Byte (external) representation of an operation
    uint8_t opcode;
    uint32_t op1;
    uint32_t op2;
} e_byte_op;

#define E_BP_STACK_SIZE     ((int)1024)
typedef struct {
    e_statusc status;
    e_internal_type val;
} e_stack_status_ret;

typedef struct {
    e_internal_type stack[E_BP_STACK_SIZE];
    unsigned int entries_nr;
    unsigned int entries;
} e_stack;

// Tables
#define E_GLOBAL_SYM_TAB_ENTRIES    ((int)128)
typedef struct {
    e_table_entry* tab_ptr;
    unsigned int entries_nr;
    unsigned int entries;
} e_table;

// Scopes
#define E_LOCAL_SYM_TAB_ENTRIES    ((int)128)
#define E_LOCAL_SYM_TAB_SCOPES     ((int)64)

// Global
void e_init(void);
e_op e_create_operation(e_opcode opcode, e_table_value op1, e_table_value op2);

// Symbol tables
e_status_ret e_table_add_entry(e_table* tab, const char* idname, e_table_value val);
e_status_ret e_table_find_entry(const e_table* tab, const char* idname);
e_table_value e_create_null(void);
e_table_value e_create_number(double val);
e_table_value e_create_string(const char* str, int ds_index);

// Stack
e_stack_status_ret e_stack_push(e_stack* stack, e_internal_type v);
e_stack_status_ret e_stack_pop(e_stack* stack);

// Scope
e_status_ret e_create_scope(void);
e_status_ret e_close_scope(void);

// Exported definitions
extern e_table global_sym_table;
extern e_table_entry global_sym_table_block[E_GLOBAL_SYM_TAB_ENTRIES];
extern e_table local_sym_table[E_LOCAL_SYM_TAB_SCOPES];
extern e_table_entry local_sym_table_block[E_LOCAL_SYM_TAB_SCOPES][E_LOCAL_SYM_TAB_ENTRIES];
extern e_stack bp_stack;
extern e_stack loop_stack;
extern unsigned int scope_level;
extern unsigned int loop_level;

// Flex / bison exports
extern int yylex();
extern int yyparse();
extern FILE* yyin;
typedef struct yy_buffer_state* YY_BUFFER_STATE;
extern YY_BUFFER_STATE yy_scan_string(const char* str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

#endif
