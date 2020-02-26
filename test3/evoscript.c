#include "evoscript.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void e_table_init(e_table* tab, unsigned int entries_nr);
static void e_clear_table(e_table* tab);
static void e_stack_init(e_stack* bp_stack, unsigned int size);

// Global symbol table (fixed block)
e_table global_sym_table;
e_table_entry global_sym_table_block[E_GLOBAL_SYM_TAB_ENTRIES];

// Local symbol tables
unsigned int scope_level;
unsigned int loop_level;
e_table local_sym_table[E_LOCAL_SYM_TAB_SCOPES];
e_table_entry local_sym_table_block[E_LOCAL_SYM_TAB_SCOPES][E_LOCAL_SYM_TAB_ENTRIES];

// Backpatch stack
e_stack bp_stack;
// Loop patch stack
e_stack loop_stack;

void
e_init(void) {
    // Initialize the global symbol table
    global_sym_table.tab_ptr = (e_table_entry*)&global_sym_table_block;
    e_table_init(&global_sym_table, E_GLOBAL_SYM_TAB_ENTRIES);
    
    // Initialize backpatch stack
    e_stack_init(&bp_stack, E_BP_STACK_SIZE);
    e_stack_init(&loop_stack, E_BP_STACK_SIZE);
    
    // Initialize local scopes
    scope_level = 0;
    loop_level = 0;
    for(unsigned int s = 0; s < E_LOCAL_SYM_TAB_SCOPES; s++) {
        // As we won't want to use dynamic memory allocation, there's a hard coded limit of nesting (scopes)
        local_sym_table[s].tab_ptr = (e_table_entry*)&local_sym_table_block[s];
        e_table_init(&local_sym_table[s], E_LOCAL_SYM_TAB_ENTRIES);
    }
}

// Table
void
e_table_init(e_table* tab, unsigned int entries_nr) {
    if(tab == NULL) {
        return;
    }
    
    tab->entries_nr = entries_nr;
    tab->entries = 0;
    e_clear_table(tab);
}

void 
e_clear_table(e_table* tab) {
    memset(tab->tab_ptr, 0, ((int)sizeof(e_table_entry) * tab->entries));
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
                    return (e_status_ret) { .status = E_STATUS_OK };
                    //return (e_status_ret) { .status = E_STATUS_ALRDYDEF };
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
    tab->entries++;
 
    return (e_status_ret) { .status = E_STATUS_OK, .ival = slot_index };
}

e_status_ret
e_table_find_entry(const e_table* tab, const char* idname) {
    if(tab == NULL || tab->entries_nr == 0) {
        return (e_status_ret) { .status = E_STATUS_NOINIT };
    }

    printf("------ FINDTABLE -------\n");
    for(int i = 0; i < tab->entries; i++) {
        printf("[%d] %s\n", i, tab->tab_ptr[i].idname);
    }
    printf("-------------------\n");
    
    unsigned int p = 0;
    signed int f = -1;
    do {
        if(tab->tab_ptr[p].used == E_TAB_ENTRY_USED) {
            if(strcmp(tab->tab_ptr[p].idname, idname) == 0) {
                f = 1;
                break;
            }
        }
        p++;
    } while(p < tab->entries);
    
    if(f != -1) {
        return (e_status_ret) { .status = E_STATUS_OK, .ival = p };
    } else {
        return (e_status_ret) { .status = E_STATUS_NOTFOUND };
    }
}

e_table_value
e_create_null(void) {
    return (e_table_value) { .val = 0, .argtype = E_ARGT_NULL };
}

e_table_value
e_create_number(double val) {
    return (e_table_value) { .val = val, .argtype = E_ARGT_NUMBER };
}

e_table_value
e_create_string(const char* str) {
    e_str_type new_str;
    
    new_str.sval = strdup(str);
    new_str.slen = strlen(str);
    
    return (e_table_value) { .sval = new_str, .argtype = E_ARGT_STRING };
}

// Stack
void
e_stack_init(e_stack* stack, unsigned int size) {
    if(stack == NULL) {
        return;
    }
    memset(stack->stack, 0, ((int)sizeof(e_internal_type) * size));
    stack->entries_nr = size;
    stack->entries = 0;
}

e_stack_status_ret
e_stack_push(e_stack* stack, e_internal_type v) {
    if(stack == NULL) {
        return (e_stack_status_ret) { .status = E_STATUS_NOINIT };
    }
    
    if(stack->entries + 1 >= stack->entries_nr) {
        /* Not enough free space */
        return (e_stack_status_ret) { .status = E_STATUS_NESIZE };
    }
    
    stack->stack[stack->entries++] = v;
    return (e_stack_status_ret) { .status = E_STATUS_OK };
}

e_stack_status_ret
e_stack_pop(e_stack* stack) {
    if(stack == NULL) {
        return (e_stack_status_ret) { .status = E_STATUS_NOINIT };
    }
    
    if(stack->entries == 0) {
        /* Stack underflow */
        return (e_stack_status_ret) { .status = E_STATUS_UNDERFLOW };
    }
    return (e_stack_status_ret) { .status = E_STATUS_OK, .val = stack->stack[--stack->entries] };
}


// OP-Code generation
e_op
e_create_operation(e_opcode opcode, e_table_value op1, e_table_value op2) {
    return (e_op) {
        .opcode = opcode,
        .op1 = op1,
        .op2 = op2
    };
}

// Blocks / Scopes
e_status_ret
e_create_scope(void) {
    if(scope_level + 1 < E_LOCAL_SYM_TAB_SCOPES) {
        scope_level++;
    } else {
        return (e_status_ret) { .status = E_STATUS_NESIZE };
    }
    
    // e_clear_table(&local_sym_table[scope_level]);
    
    if(scope_level == 1) return (e_status_ret) { .status = E_STATUS_OK }; 
    // TODO: We are missing the 0th scope block with this approach!
    
    // Copy previous scope
    printf("previous table entries: %d\n", local_sym_table[scope_level - 1].entries);

    // Copy previous table into new one
    for(unsigned int i = 0; i < local_sym_table[scope_level - 1].entries; i++) {
        local_sym_table[scope_level].tab_ptr[i] = local_sym_table[scope_level - 1].tab_ptr[i];
    }
    local_sym_table[scope_level].entries = local_sym_table[scope_level - 1].entries;

    
    printf("BLOCK BEGIN, level: %d with %d entries\n", scope_level, local_sym_table[scope_level].entries);

    printf("------ SCOPE -------\n");
    for(int i = 0; i < local_sym_table[scope_level].entries; i++) {
        printf("[%d] %s\n", i, local_sym_table[scope_level].tab_ptr[i].idname);
    }
    printf("-------------------\n");
    
    return (e_status_ret) { .status = E_STATUS_OK };
}


e_status_ret
e_close_scope(void) {
    if(scope_level == 0) {
        return (e_status_ret) { .status = E_STATUS_NESTING };
    }
    printf("/// CLOSING SCOPE %d ///\n", scope_level);
    e_clear_table(&local_sym_table[scope_level]);
    scope_level--;
    
    return (e_status_ret) { .status = E_STATUS_OK };
}
