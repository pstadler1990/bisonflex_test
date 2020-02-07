%{
    #include <stdio.h>
    #include <stdlib.h>
    
    extern int yylex();
    extern int yyparse();
    
    extern FILE* yyin;
    
    void yyerror(const char* s);
%}

%union {
    int ival;
}

%token<ival> INT
%token PLUS MINUS
%token MULTIPLY DIVIDE
%token NEWLINE

%left PLUS MINUS
%left MULTIPLY DIVIDE

%type<ival> expression

%%
prgm:
    | prgm line
    ;   
    
line: NEWLINE
    | expression NEWLINE { printf("%d ok\n", $1); }

expression: INT { $$ = $1; }
            | expression MULTIPLY expression { $$ = $1 * $3; }
            | expression DIVIDE expression { $$ = $1 / $3; }
            | expression PLUS expression { $$ = $1 + $3; }
            | expression MINUS expression { $$ = $1 - $3; }
            ;
%%

int main(void) {
    printf("Test language interpreter, welcome!\n");
    yyin = stdin;
    do {
        yyparse();
    } while(!feof(yyin));
    
    return 0;
}

void yyerror(const char* s) {
    printf("Error happend: %s\n", s);
    exit(-1);
}
