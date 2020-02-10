#include "evoscript.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void e_table_init(e_table* tab, unsigned int entries_nr);
static e_table_value e_table_value_create_int(int val);
static e_table_value e_table_value_create_float(float val);
e_table_value e_table_value_create_string(const char* str);


// Global symbol table (fixed block)
e_table global_sym_table;
e_table_entry global_sym_table_block[E_GLOBAL_SYM_TAB_SIZE];


void
e_init(void) {
    global_sym_table.tab_ptr = (e_table_entry*)&global_sym_table_block;
    e_table_init(&global_sym_table, E_GLOBAL_SYM_TAB_ENTRIES);
    
    // Remove me
    e_table_add_entry(&global_sym_table, "x", e_table_value_create_int(42));
    e_table_add_entry(&global_sym_table, "y", e_table_value_create_string("Hello World"));
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
        /* No more free space in global symbol table */
        return E_STATUS_NESIZE;
    }
    
    e_table_entry new_node;
    
    new_node.idname = strdup(idname);
    new_node.val = val;
    
    if(tab->tab_ptr[tab->entries].used == E_TAB_ENTRY_FREE) {
        printf("First entry is free\n");
    } else {
        printf("First entry is *NOT* free\n");
    }
}

e_table_value
e_table_value_create_int(int val) {
    return (e_table_value) { .ival = val };
}

e_table_value
e_table_value_create_float(float val) {
    return (e_table_value) { .fval = val };
}

e_table_value
e_table_value_create_string(const char* str) {
    e_str_type new_str;
    
    new_str.sval = strdup(str);
    new_str.slen = strlen(str);
    
    return (e_table_value) { .sval = new_str };
}
