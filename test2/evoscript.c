#include "evoscript.h"
#include <stdlib.h>



void
e_init(e_context* context) {
    /* Initialize a evoScript context */
    e_table global_sym_table;
    e_table_init(&global_sym_table, E_GLOBAL_SYM_TAB_SIZE);
}


void
e_table_init(e_table* tab, unsigned int size) {
    if(tab == NULL) {
        return;
    }
    tab->size = size;
    tab->entries = 0;
    tab->first = NULL;
}


int 
e_table_add_entry(e_table* tab, const char* idname, e_table_value val) {
    if(tab->entries + 1 >= tab->size) {
        /* No more free space in global symbol table */
        return -1;
    }
    
}
