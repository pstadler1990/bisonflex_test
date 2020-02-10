%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "grammar.tab.h"
    
    extern int yylex();
%}

%option noyywrap

%%
[ \t]                       ;
\n                          { return NEWLINE; }
"let"                       { return ASSIGN; }
"gstdump"                   { return GL_SYM_DUMP; }
[0-9]+                      { yylval.ival = atoi(yytext); return INT; }
[a-zA-Z_][a-zA-Z_0-9]*      { yylval.sname = strdup(yytext); return IDENTIFIER; } 
"="                         { return EQUALS; }
"+"                         { return PLUS; }
"-"                         { return MINUS; }
"*"                         { return MULTIPLY; }
"/"                         { return DIVIDE; }

%%
