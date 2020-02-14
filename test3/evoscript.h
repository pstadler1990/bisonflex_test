#ifndef __EVOSCRIPT_H
#define __EVOSCRIPT_H

#define E_VERSION "0.01"

typedef struct {
    enum { E_NUMBER, E_STRING } type;
    double val;
} e_type;

typedef enum {
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
    E_ARGT_NULL,
    E_ARGT_NUMBER,
    E_ARGT_STRING
} e_arg_type;

typedef struct {
    char* sval;
    int slen;
} e_str_type;

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
    union {
        int ival;
        float fval;
        e_str_type sval;
    };
} e_status_ret;

// VM
typedef enum {
    E_OP_PUSHG,     /* Push global variable,                    PUSHG [index],      s[-1]           */
    E_OP_POPG,      /* Pop global variable,                     POPG [index]                        */
    E_OP_PUSH,      /* Push variable onto top of stack,         PUSH 3                              */
    E_OP_POP,       /* Pop variable from top of stack,          POP,                s[-1]           */
    
    E_OP_EQ,        /* Equal check,                             EQ,                 s[-1]==s[-2]    */
    E_OP_LT,        /* Less than,                               LT,                 s[-1]<s[-2]     */    
    E_OP_GT,        /* Greater than,                            GT,                 s[-1]<s[-2]     */ 
    E_OP_LTEQ,      /* Less than or equal,                      LTEQ,               s[-1]<=s[-2]    */ 
    E_OP_GTEQ,      /* Greater than or equal,                   GTEQ,               s[-1]>=s[-2]    */ 
    
    E_OP_ADD,
    E_OP_SUB,
    E_OP_MUL,
    E_OP_DIV,
    E_OP_AND,
    E_OP_OR,
    E_OP_NOT,
    
    E_OP_JZ,        /* Jump if zero,                            JZ [addr]                           */
} e_opcode;

typedef struct {
    e_opcode opcode;
    // 2 args max
    e_table_value op1;
    e_table_value op2;
} e_op;

#define E_BP_STACK_SIZE     ((int)255)
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
#define E_GLOBAL_SYM_TAB_ENTRIES    ((int)32)
typedef struct {
    e_table_entry* tab_ptr;
    unsigned int entries_nr;
    unsigned int entries;
} e_table;

// Global
void e_init(void);
e_op e_create_operation(e_opcode opcode, e_table_value op1, e_table_value op2);

// Symbol tables
e_status_ret e_table_add_entry(e_table* tab, const char* idname, e_table_value val);
e_status_ret e_table_find_entry(const e_table* tab, const char* idname);
e_table_value e_create_null(void);
e_table_value e_create_number(double val);
e_table_value e_create_float(float val);
e_table_value e_create_string(const char* str);

// Stack
e_stack_status_ret e_stack_push(e_stack* stack, e_internal_type v);
e_stack_status_ret e_stack_pop(e_stack* stack);

// Exported definitions
extern e_table global_sym_table;
extern e_table_entry global_sym_table_block[E_GLOBAL_SYM_TAB_ENTRIES];
extern e_stack bp_stack;

#endif
