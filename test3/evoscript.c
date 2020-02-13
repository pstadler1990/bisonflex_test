#include "evoscript.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void e_table_init(e_table* tab, unsigned int entries_nr);

// Global symbol table (fixed block)
e_table global_sym_table;
e_table_entry global_sym_table_block[E_GLOBAL_SYM_TAB_SIZE];


void
e_init(void) {
    /* Initialize the global symbol table */
    global_sym_table.tab_ptr = (e_table_entry*)&global_sym_table_block;
    e_table_init(&global_sym_table, E_GLOBAL_SYM_TAB_ENTRIES);
}


void
e_table_init(e_table* tab, unsigned int entries_nr) {
    if(tab == NULL) {
        return;
    }
    memset(tab->tab_ptr, 0, ((int)sizeof(e_table_entry) * entries_nr));
    tab->entries_nr = entries_nr;
    tab->entries = 0;
}


e_status_ret 
e_table_add_entry(e_table* tab, const char* idname, e_table_value val) {
    //
    if(tab == NULL || tab->entries_nr == 0) {
        return (e_status_ret) { .status = E_STATUS_NOINIT };
    }
    if(tab->entries + 1 >= tab->entries_nr) {
        /* Not enough free space */
        return (e_status_ret) { .status = E_STATUS_NESIZE };
    }
    
    e_table_entry new_node;

    new_node.idname = strdup(idname);
    new_node.svalue = val;
    new_node.svalue.argtype = val.argtype;
    new_node.used = E_TAB_ENTRY_USED;
    
    int slot_index = 0;
    if(tab->tab_ptr[0].used == E_TAB_ENTRY_FREE) {
        slot_index = 0;
    } else {
        int p = 0, f = 0;
        do {
            if(tab->tab_ptr[p].used == E_TAB_ENTRY_FREE 
                && f == 0) {
                f = p;
            } else if(tab->tab_ptr[p].used == E_TAB_ENTRY_USED) {
                if(strcmp(tab->tab_ptr[p].idname, idname) == 0) {
                    return (e_status_ret) { .status = E_STATUS_ALRDYDEF };
                }
            }
            p++;
        } while(p < tab->entries_nr);
        
        if(f != 0) {
            slot_index = f;
        } else {
            return (e_status_ret) { .status = E_STATUS_NESIZE };
        }
    }

    tab->tab_ptr[slot_index] = new_node;
    return (e_status_ret) { .status = E_STATUS_OK, .ival = slot_index };
}

e_status_ret
e_table_find_entry(const e_table* tab, const char* idname) {
    if(tab == NULL || tab->entries_nr == 0) {
        return (e_status_ret) { .status = E_STATUS_NOINIT };
    }

    unsigned int p = 0;
    signed int f = -1;
    do {
        if(tab->tab_ptr[p].used == E_TAB_ENTRY_USED 
            && f == -1) {
            if(strcmp(tab->tab_ptr[p].idname, idname) == 0) {
                f = 1;
                break;
            }
        }
        p++;
    } while(p < tab->entries_nr);
    
    if(f != -1) {
        return (e_status_ret) { .status = E_STATUS_OK, .ival = p };
    } else {
        return (e_status_ret) { .status = E_STATUS_NOTFOUND };
    }
}

e_table_value
e_create_null(void) {
    return (e_table_value) { .ival = 0, .argtype = E_ARGT_NULL };
}

e_table_value
e_create_int(int val) {
    return (e_table_value) { .ival = val, .argtype = E_ARGT_INT };
}

e_table_value
e_create_float(float val) {
    return (e_table_value) { .fval = val, .argtype = E_ARGT_FLOAT };
}

e_table_value
e_create_string(const char* str) {
    e_str_type new_str;
    
    new_str.sval = strdup(str);
    new_str.slen = strlen(str);
    
    return (e_table_value) { .sval = new_str, .argtype = E_ARGT_STRING };
}

e_op
e_create_operation(e_opcode opcode, e_table_value op1, e_table_value op2) {
    return (e_op) {
        .opcode = opcode,
        .op1 = op1,
        .op2 = op2
    };
}
