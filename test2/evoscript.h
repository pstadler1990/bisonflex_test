#ifndef __EVOSCRIPT_H
#define __EVOSCRIPT_H

#define E_VERSION "0.01"

typedef enum {
    E_STATUS_ALRDYDEF = -3,
    E_STATUS_NESIZE = -2,
    E_STATUS_NOINIT = -1,
    E_STATUS_UNDEF = 0,
    E_STATUS_OK = 1,
} e_statusc;

typedef enum {
    E_ARGT_INT,
    E_ARGT_FLOAT,
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
} e_table_value;

#define E_TAB_ENTRY_USED    ((unsigned char)1)
#define E_TAB_ENTRY_FREE    ((unsigned char)0)

typedef struct table_entry {
    unsigned char used;
    char* idname;
    e_table_value val;
    e_arg_type argtype;
} e_table_entry;

#define E_GLOBAL_SYM_TAB_ENTRIES    ((int)32)    
#define E_GLOBAL_SYM_TAB_SIZE   ((int)sizeof(e_table_entry) * E_GLOBAL_SYM_TAB_ENTRIES)

typedef struct {
    e_table_entry* tab_ptr;
    unsigned int entries_nr;
    unsigned int entries;
} e_table;

// Global
void e_init(void);

// Symbol tables
int e_table_add_entry(e_table* tab, const char* idname, e_table_value val, e_arg_type argtype);
void e_table_memdump(const e_table* tab);
#endif
