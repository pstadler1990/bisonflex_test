%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "grammar.tab.h"
    //#include "tokens.h"
    
    extern int yylex();
%}

%option noyywrap

%%
[ \t]       ;
\n          { return NEWLINE; }
[0-9]+      { yylval.ival = atoi(yytext); return INT; }
"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MULTIPLY; }
"/"         { return DIVIDE; }

%%
