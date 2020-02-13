%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "evoscript.h"
    #include "grammar.tab.h"
    
    extern int yylex();
%}

%option noyywrap

%%
[ \t]                       ;
[\n:]                       { return NEWLINE; }
"if"                        { return BLOCK_IF; }
"then"                      { return BLOCK_THEN; }
"endif"                     { return BLOCK_ENDIF; }
"let"                       { return ASSIGN; }
"and"                       { return AND; }
"or"                        { return OR; }
"not"                       { return NOT; }
[0-9]+                      { yylval.nval.ival = atoi(yytext); yylval.nval.type = E_INTEGER; return INT; }
[0-9]*\.[0-9]+              { yylval.nval.fval = atof(yytext); yylval.nval.type = E_FLOAT; return FLOAT; }
[a-zA-Z_][a-zA-Z_0-9]*      { yylval.sname = strdup(yytext); return IDENTIFIER; } 
"="                         { return EQUALS; }
"+"                         { return PLUS; }
"-"                         { return MINUS; }
"*"                         { return MULTIPLY; }
"/"                         { return DIVIDE; }
"("                         { return P_OPEN; }
")"                         { return P_CLOSE; }
"<"                         { return REL_LT; }
"<="                        { return REL_LTEQ; }
"!="                        { return REL_NOTEQ; }
"=="                        { return REL_EQ; }
">="                        { return REL_GTEQ; }
">"                         { return REL_GT; }

%%
