#ifndef __EVOSCRIPT_H
#define __EVOSCRIPT_H

#define E_VERSION "0.01"

typedef struct {
    char* sval;
    int slen;
} e_str_type;

typedef union {
    int ival;
    float fval;
    e_str_type sval;
} e_table_value;

typedef struct table_entry {
    char* idname;
    e_table_value val;
    struct table_entry* next;
} e_table_entry;

#define E_GLOBAL_SYM_TAB_ENTRIES    ((int)32)    
#define E_GLOBAL_SYM_TAB_SIZE   ((int)sizeof(e_table_entry) * E_GLOBAL_SYM_TAB_ENTRIES)

typedef struct {
    e_table_entry* first;
    unsigned int size;
    unsigned int entries;
} e_table;

// Context
typedef struct {
    e_table global_sym_table;
} e_context;

// Global
void e_init(e_context* context);

// Symbol tables
void e_table_init(e_table* tab, unsigned int size);
int e_table_add_entry(e_table* tab, const char* idname, e_table_value val);

#endif
