%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "evoscript.h"
    #include "grammar.tab.h"
%}

%option noyywrap

%%
[ \t]                       ;
[\n:]                       { return NEWLINE; }
"?"                         { return PRINT_BYTES; }
"print"                     { return PRINT; }
"if"                        { return BLOCK_IF; }
"then"                      { return BLOCK_THEN; }
"else"                      { return BLOCK_ELSE; }
"elseif"                    { return BLOCK_ELSEIF; }
"endif"                     { return BLOCK_ENDIF; }
"let"                       { return ASSIGN; }
"and"                       { return AND; }
"or"                        { return OR; }
"not"                       { return NOT; }
"repeat"                    { return LOOP_REPEAT; }
"forever"                   { return LOOP_FOREVER; }
"break"                     { return LOOP_BREAK; }
"mod"                       { return MOD; }
[0-9]+                      { yylval.nval.val = atof(yytext); yylval.nval.type = E_NUMBER; return NUMBER; }
[0-9]*\.[0-9]+              { yylval.nval.val = atof(yytext); yylval.nval.type = E_NUMBER; return NUMBER; }
[a-zA-Z_][a-zA-Z_0-9]*      { yylval.sname = strdup(yytext); return IDENTIFIER; }
\"([^\\\"]|\\.)*\"          {
                                yylval.nval.type = E_STRING;
                                yylval.nval.str.slen = strlen(yytext);
                                yylval.nval.str.sval = strdup(yytext+1);
                                yylval.nval.str.sval[yylval.nval.str.slen-2] = 0;
                                return STRING;
                            }
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
