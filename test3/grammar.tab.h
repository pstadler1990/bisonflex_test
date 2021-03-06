
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NUMBER = 258,
     STRING = 259,
     IDENTIFIER = 260,
     ASSIGN = 261,
     EQUALS = 262,
     AND = 263,
     OR = 264,
     NOT = 265,
     REL_LT = 266,
     REL_LTEQ = 267,
     REL_NOTEQ = 268,
     REL_EQ = 269,
     REL_GTEQ = 270,
     REL_GT = 271,
     PLUS = 272,
     MINUS = 273,
     MOD = 274,
     MULTIPLY = 275,
     DIVIDE = 276,
     P_OPEN = 277,
     P_CLOSE = 278,
     BLOCK_IF = 279,
     BLOCK_THEN = 280,
     BLOCK_ELSE = 281,
     BLOCK_ENDIF = 282,
     LOOP_REPEAT = 283,
     LOOP_FOREVER = 284,
     LOOP_BREAK = 285,
     NEWLINE = 286,
     PRINT_BYTES = 287,
     PRINT = 288
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 39 "grammar.y"

    e_type nval;
    char* sname;



/* Line 1676 of yacc.c  */
#line 92 "grammar.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


