#include "evoscript.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void e_table_init(e_table* tab, unsigned int entries_nr);
static int e_table_find_entry(const e_table* tab, const char* idname);

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


e_statusc 
e_table_add_entry(e_table* tab, const char* idname, e_table_value val) {
    if(tab == NULL || tab->entries_nr == 0) {
        return E_STATUS_NOINIT;
    }
    if(tab->entries + 1 >= tab->entries_nr) {
        /* Not enough free space */
        return E_STATUS_NESIZE;
    }
    
    e_table_entry new_node;

    new_node.idname = strdup(idname);
    new_node.svalue.val = val.val;
    new_node.svalue.argtype = val.argtype;
    new_node.used = E_TAB_ENTRY_USED;
    
    unsigned int slot_index = 0;
    if(tab->tab_ptr[0].used == E_TAB_ENTRY_FREE) {
        slot_index = 0;
    } else {
        unsigned int p = 0, f = 0;
        do {
            if(tab->tab_ptr[p].used == E_TAB_ENTRY_FREE 
                && f == 0) {
                f = p;
            } else if(tab->tab_ptr[p].used == E_TAB_ENTRY_USED) {
                if(strcmp(tab->tab_ptr[p].idname, idname) == 0) {
                    return E_STATUS_ALRDYDEF;
                }
            }
            p++;
        } while(p < tab->entries_nr);
        
        if(f != 0) {
            slot_index = f;
        } else {
            return E_STATUS_NESIZE;
        }
    }

    tab->tab_ptr[slot_index] = new_node;
    return E_STATUS_OK;
}


e_statusc
e_table_change_entry(e_table* tab, const char* idname, e_table_value val) {
    if(tab == NULL || tab->entries_nr == 0) {
        return E_STATUS_NOINIT;
    }
    
    unsigned int slot_index = 0;
    if(tab->tab_ptr[0].used == E_TAB_ENTRY_FREE) {
        return E_STATUS_NOTFOUND;
    } else {
        int tmp_slot = e_table_find_entry(tab, idname);
        if(tmp_slot < 0) {
           return tmp_slot; /* error */
        }
        slot_index = tmp_slot;
    }
    
// if(tab->tab_ptr[slot_index].svalue.argtype != val.argtype) {
// /* error, data type mismatch */
// return E_STATUS_DATATMIS;
// }
        
    tab->tab_ptr[slot_index].svalue = val;
    return E_STATUS_OK;
}

int
e_table_find_entry(const e_table* tab, const char* idname) {
    if(tab == NULL || tab->entries_nr == 0) {
        return E_STATUS_NOINIT;
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
        return p;
    } else {
        return E_STATUS_NOTFOUND;
    }
}

e_table_entry_ret
e_table_load_entry(const e_table* tab, const char* idname) {
    int tmp_slot = e_table_find_entry(tab, idname);
    if(tmp_slot < 0) {
        return (e_table_entry_ret) { .status = E_STATUS_NOTFOUND, .svalue = 0 };
    }
    
    return (e_table_entry_ret) { .status = E_STATUS_OK, .svalue = tab->tab_ptr[tmp_slot].svalue };
}


void
e_table_memdump(const e_table* tab) {
    printf("** Memory dump of global system table **\n");
    for(unsigned int r = 0; r < tab->entries_nr; r++) {
        if(tab->tab_ptr[r].used == E_TAB_ENTRY_USED) {
            
            printf("USED\t%s:\t", tab->tab_ptr[r].idname);
            switch(tab->tab_ptr[r].svalue.argtype) {
                case E_ARGT_INT:
                    printf("%d\n", tab->tab_ptr[r].svalue.val.ival);
                    break;
                case E_ARGT_FLOAT:
                    printf("%f\n", tab->tab_ptr[r].svalue.val.fval);
                    break;
                default:
                    printf("%s\n", tab->tab_ptr[r].svalue.val.sval);
            }
        } else {
            printf("-FREE-\n");
        }
    }
}

e_table_value
e_create_int(int val) {
    return (e_table_value) { .val.ival = val, .argtype = E_ARGT_INT };
}

e_table_value
e_create_float(float val) {
    return (e_table_value) { .val.fval = val, .argtype = E_ARGT_FLOAT };
}

e_table_value
e_create_string(const char* str) {
    e_str_type new_str;
    
    new_str.sval = strdup(str);
    new_str.slen = strlen(str);
    
    return (e_table_value) { .val.sval = new_str, .argtype = E_ARGT_STRING };
}
