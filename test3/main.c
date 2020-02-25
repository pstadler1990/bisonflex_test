#include <stdio.h>
#include <stdlib.h>
#include "evoscript.h"
#include "grammar.tab.h"


int main(void) {
    e_init();
    
    char* buf;
    long fsize;
    
    FILE* f = fopen("script.evo", "r");
    if(!f) {
        printf("File not found\n");
        return 0;
    }
    fseek(f, 0, SEEK_END);
    fsize = ftell(f);
    rewind(f);
    buf = malloc(fsize + 1 * sizeof(char));
    fread(buf, sizeof(char), fsize, f);
    fclose(f);
    buf[fsize] = 0;
    
    printf("%s\n", buf);
  
    
    YY_BUFFER_STATE bs = yy_scan_string(buf);
    
    yyparse();
    yy_delete_buffer(bs);
    free(buf);
    
    return 0;
}
