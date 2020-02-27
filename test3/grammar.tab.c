
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "grammar.y"

    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include "evoscript.h"
    #include <string.h>
    
    extern int yylex();
    extern int yyparse();
    
    extern FILE* yyin;
    
    void yyerror(const char* s);
    
    static void error_pprint(e_statusc error);
    static void emit_op(e_op op);
    static void inc_cnt(void);
    static void jmp_patch(unsigned int start_addr, unsigned int end_addr);
    
    static void double_to_bytearray(double din, uint8_t bin[]);
    static int ds_store_string(const char* str);
    
    void print_outstream(void);
    
    static int addr_count = 0;
    static uint8_t out_bytes[E_OUT_TOTAL_SIZE];
    static unsigned int out_b_cnt = 0;
    static unsigned int out_ds_cnt = E_OUT_SIZE + 1;
    
    unsigned int lc = 0;
    
    bool loop_has_break[E_LOCAL_SYM_TAB_SCOPES];


/* Line 189 of yacc.c  */
#line 108 "grammar.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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
     MULTIPLY = 274,
     DIVIDE = 275,
     P_OPEN = 276,
     P_CLOSE = 277,
     BLOCK_IF = 278,
     BLOCK_THEN = 279,
     BLOCK_ENDIF = 280,
     LOOP_REPEAT = 281,
     LOOP_FOREVER = 282,
     LOOP_BREAK = 283,
     NEWLINE = 284,
     PRINT_BYTES = 285
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 36 "grammar.y"

    e_type nval;
    char* sname;



/* Line 214 of yacc.c  */
#line 181 "grammar.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 193 "grammar.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  29
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   120

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  31
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  15
/* YYNRULES -- Number of rules.  */
#define YYNRULES  41
/* YYNRULES -- Number of states.  */
#define YYNSTATES  69

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   285

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     9,    11,    14,    16,    18,    20,
      22,    24,    26,    28,    33,    37,    42,    44,    46,    50,
      54,    58,    62,    66,    70,    74,    78,    81,    85,    89,
      93,    97,   101,   103,   107,   111,   113,   116,   120,   122,
     124,   127
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      32,     0,    -1,    33,    -1,    34,    35,    33,    -1,    34,
      -1,    34,    35,    -1,    36,    -1,    37,    -1,    39,    -1,
      42,    -1,    28,    -1,    30,    -1,    29,    -1,     6,     5,
       7,    38,    -1,     5,     7,    37,    -1,     6,     5,     7,
      37,    -1,    45,    -1,     5,    -1,    37,    14,    37,    -1,
      37,    13,    37,    -1,    37,    11,    37,    -1,    37,    16,
      37,    -1,    37,    12,    37,    -1,    37,    15,    37,    -1,
      37,     8,    37,    -1,    37,     9,    37,    -1,    10,    37,
      -1,    21,    37,    22,    -1,    37,    19,    37,    -1,    37,
      20,    37,    -1,    37,    17,    37,    -1,    37,    18,    37,
      -1,     4,    -1,    40,    33,    25,    -1,    23,    37,    41,
      -1,    24,    -1,    24,    35,    -1,    43,    33,    27,    -1,
      44,    -1,    26,    -1,    26,    35,    -1,     3,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    67,    67,    70,    71,    72,    75,    76,    77,    78,
      79,    92,    95,    97,   121,   143,   169,   173,   212,   217,
     222,   227,   232,   237,   242,   245,   248,   251,   254,   257,
     261,   293,   317,   325,   342,   360,   361,   364,   395,   414,
     415,   418
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NUMBER", "STRING", "IDENTIFIER",
  "ASSIGN", "EQUALS", "AND", "OR", "NOT", "REL_LT", "REL_LTEQ",
  "REL_NOTEQ", "REL_EQ", "REL_GTEQ", "REL_GT", "PLUS", "MINUS", "MULTIPLY",
  "DIVIDE", "P_OPEN", "P_CLOSE", "BLOCK_IF", "BLOCK_THEN", "BLOCK_ENDIF",
  "LOOP_REPEAT", "LOOP_FOREVER", "LOOP_BREAK", "NEWLINE", "PRINT_BYTES",
  "$accept", "prgm", "expression_list", "expression", "line_sep", "assign",
  "math_expression", "string_expression", "if_expression", "if_condition",
  "if_blockthen", "loop_expression", "loop_begin", "loop_repeat", "number", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    31,    32,    33,    33,    33,    34,    34,    34,    34,
      34,    34,    35,    36,    36,    36,    37,    37,    37,    37,
      37,    37,    37,    37,    37,    37,    37,    37,    37,    37,
      37,    37,    38,    39,    40,    41,    41,    42,    43,    44,
      44,    45
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     3,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     4,     3,     4,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     3,     3,     3,
       3,     3,     1,     3,     3,     1,     2,     3,     1,     1,
       2,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    41,    17,     0,     0,     0,     0,    39,    10,    11,
       0,     2,     4,     6,     7,     8,     0,     9,     0,    38,
      16,     0,     0,    17,    26,     0,     0,    12,    40,     1,
       5,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    14,     0,    27,    35,    34,
       3,    24,    25,    20,    22,    19,    18,    23,    21,    30,
      31,    28,    29,    33,    37,    32,    15,    13,    36
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    10,    11,    12,    28,    13,    14,    67,    15,    16,
      49,    17,    18,    19,    20
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -15
static const yytype_int8 yypact[] =
{
      38,   -15,     6,     3,     4,     4,     4,   -14,   -15,   -15,
      16,   -15,   -14,   -15,    90,   -15,    38,   -15,    38,   -15,
     -15,     4,    13,   -15,   100,    75,    61,   -15,   -15,   -15,
      38,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,    -2,    18,    90,     1,   -15,   -14,   -15,
     -15,   100,   100,    32,    32,    32,    32,    32,    32,    -1,
      -1,   -15,   -15,   -15,   -15,   -15,    90,   -15,   -15
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -15,   -15,    -6,   -15,    -9,   -15,    -4,   -15,   -15,   -15,
     -15,   -15,   -15,   -15,   -15
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      24,    25,    26,    30,     1,    65,    23,     1,    22,    23,
      43,     4,    44,    21,     4,    27,    29,    45,    41,    42,
      46,     0,     5,    63,    50,     5,     0,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    68,
       0,     1,    66,     2,     3,    64,     0,     0,     4,    39,
      40,    41,    42,     0,     0,     0,     0,     0,     0,     5,
       0,     6,     0,     0,     7,     0,     8,     0,     9,    31,
      32,     0,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,     0,    31,    32,    48,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,     0,    47,    31,    32,
       0,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42
};

static const yytype_int8 yycheck[] =
{
       4,     5,     6,    12,     3,     4,     5,     3,     5,     5,
      16,    10,    18,     7,    10,    29,     0,    21,    19,    20,
       7,    -1,    21,    25,    30,    21,    -1,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    48,
      -1,     3,    46,     5,     6,    27,    -1,    -1,    10,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    23,    -1,    -1,    26,    -1,    28,    -1,    30,     8,
       9,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,     8,     9,    24,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    22,     8,     9,
      -1,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     5,     6,    10,    21,    23,    26,    28,    30,
      32,    33,    34,    36,    37,    39,    40,    42,    43,    44,
      45,     7,     5,     5,    37,    37,    37,    29,    35,     0,
      35,     8,     9,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    33,    33,    37,     7,    22,    24,    41,
      33,    37,    37,    37,    37,    37,    37,    37,    37,    37,
      37,    37,    37,    25,    27,     4,    37,    38,    35
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 10:

/* Line 1455 of yacc.c  */
#line 79 "grammar.y"
    { 
                if(loop_level == 0) {
                    yyerror("Break without proper loop");
                }
                printf("BREAK at level %d -> [%d]!\n", loop_level, addr_count);

				// Insert JMP [64 bit dummy_addr] to be patched later
                emit_op(e_create_operation(E_OP_JMP, e_create_number(0xFFFFFFFF), e_create_number(0xFFFFFFFF)));

				e_internal_type addr =  { .ival = addr_count };
				e_stack_status_ret s = e_stack_push(&loop_stack, addr);
				loop_has_break[loop_level] = true;
            ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 92 "grammar.y"
    { print_outstream(); ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 95 "grammar.y"
    { lc++; ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 97 "grammar.y"
    {
        	e_opcode op;
            e_status_ret s;

            // Add string data to data segment (bytecode section)
            int str_index = ds_store_string((yyvsp[(4) - (4)].nval).str.sval);
            // This is totally independent from the variable's scope (both, global and local strings are stored in the
            // data section, only the variable's visibility is scope bound
            emit_op(e_create_operation(E_OP_PUSH, e_create_number(str_index), e_create_null()));

            if(scope_level == 0) {
            	s = e_table_add_entry(&global_sym_table, (yyvsp[(2) - (4)].sname), e_create_string((yyvsp[(4) - (4)].nval).str.sval));
            	op = E_OP_PUSHG;
            } else {
            	s = e_table_add_entry(&local_sym_table[scope_level], (yyvsp[(2) - (4)].sname), e_create_string((yyvsp[(4) - (4)].nval).str.sval));
                op = E_OP_PUSHL;
            }

        	if(s.status == E_STATUS_OK) {
				emit_op(e_create_operation(op, e_create_number(s.ival), e_create_number(E_ARGT_STRING)));
			} else {
				error_pprint(s.status);
			}
        ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 121 "grammar.y"
    {
                    /* Change value of number type variable, a = 3 */
                    // PUSHG $3 [index] (value, index)
                    // PUSHL $3 [index] (value, index)
                    e_opcode op;
                    e_status_ret s;

                    s = e_table_find_entry(&global_sym_table, (yyvsp[(1) - (3)].sname));
                    op = E_OP_PUSHG;

                    if(s.status != E_STATUS_OK) {
        				s = e_table_find_entry(&local_sym_table[scope_level], (yyvsp[(1) - (3)].sname));
        				op = E_OP_PUSHL;
                    }

                    if(s.status == E_STATUS_OK) {
                        int gst_index = s.ival;
                        emit_op(e_create_operation(op, e_create_number(gst_index), e_create_number(E_ARGT_NUMBER)));
                    } else {
                        error_pprint(s.status);
                    }
        ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 143 "grammar.y"
    {
            /* Number type (integer|float) definition with initialization, let x = 42 */
            e_opcode op;
            e_status_ret s;

            printf("argtype expr: %d\n", (yyvsp[(4) - (4)].nval).type);

            if(scope_level == 0) {
                // PUSHG [index]
                s = e_table_add_entry(&global_sym_table, (yyvsp[(2) - (4)].sname), e_create_number((yyvsp[(4) - (4)].nval).val));
                op = E_OP_PUSHG;
            } else {
                // PUSHL [index]
                printf("Push local %s into scope %d\n", (yyvsp[(2) - (4)].sname), scope_level);
                s = e_table_add_entry(&local_sym_table[scope_level], (yyvsp[(2) - (4)].sname), e_create_number((yyvsp[(4) - (4)].nval).val));
                op = E_OP_PUSHL;
            }

            if(s.status == E_STATUS_OK) {
                emit_op(e_create_operation(op, e_create_number(s.ival), e_create_number(E_ARGT_NUMBER)));
            } else {
                error_pprint(s.status);
            }
        ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 169 "grammar.y"
    {
                    // PUSH [number]
                    emit_op(e_create_operation(E_OP_PUSH, e_create_number((yyvsp[(1) - (1)].nval).val), e_create_null()));
                ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 173 "grammar.y"
    { 
                    /* Find and pop a global or local variable */
                    e_opcode op;
                    e_status_ret s;
                    
                    if(scope_level > 0) {
                        // POPL [index]
                        printf("Finding local %s in scope %d\n", (yyvsp[(1) - (1)].sname), scope_level);
                        s = e_table_find_entry(&local_sym_table[scope_level], (yyvsp[(1) - (1)].sname));
                        op = E_OP_POPL;
                    }
                    
                    if(scope_level == 0 || s.status == E_STATUS_NOTFOUND) {
                        printf("Finding global %s\n", (yyvsp[(1) - (1)].sname));
                        // POPG [index]
                        s = e_table_find_entry(&global_sym_table, (yyvsp[(1) - (1)].sname));
                        op = E_OP_POPG;
                    }

                    if(s.status == E_STATUS_OK) {
                        int gst_index = s.ival;
                        emit_op(e_create_operation(op, e_create_number(gst_index), e_create_null()));

                        (yyval.nval).type = s.argtype;
                        if(s.argtype == E_ARGT_NUMBER) {
                        	(yyval.nval).val = s.fval;
                        } else if(s.argtype == E_ARGT_STRING) {
                        	(yyval.nval).str.sval = strdup(s.sval.sval);
                        	printf("--> STR: %s\n", (yyval.nval).str.sval);
                        }

                    } else {
                        for(int i = 0; i < local_sym_table[scope_level].entries; i++) {
                            printf("\t%s\t%f\n", local_sym_table[scope_level].tab_ptr[i].idname, local_sym_table[scope_level].tab_ptr[i].svalue.val);
                        }

                        error_pprint(s.status);
                    }
                ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 212 "grammar.y"
    {
                    /* a == b */
                    // EQ
                    emit_op(e_create_operation(E_OP_EQ, e_create_null(), e_create_null()));
                ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 217 "grammar.y"
    {
                    /* a != b */
                    emit_op(e_create_operation(E_OP_EQ, e_create_null(), e_create_null()));
                    emit_op(e_create_operation(E_OP_NOT, e_create_null(), e_create_null()));
                ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 222 "grammar.y"
    {
                    /* a < b */
                    // LT
                    emit_op(e_create_operation(E_OP_LT, e_create_null(), e_create_null()));
                ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 227 "grammar.y"
    {
                    /* a > b */
                    // GT
                    emit_op(e_create_operation(E_OP_GT, e_create_null(), e_create_null()));
                ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 232 "grammar.y"
    {
                    /* a <= b */
                    // LTEQ
                    emit_op(e_create_operation(E_OP_LTEQ, e_create_null(), e_create_null()));
                ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 237 "grammar.y"
    {
                    /* a >= b */
                    // GTEQ
                    emit_op(e_create_operation(E_OP_GTEQ, e_create_null(), e_create_null()));
                ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 242 "grammar.y"
    {
                    emit_op(e_create_operation(E_OP_AND, e_create_null(), e_create_null()));
                ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 245 "grammar.y"
    {
                    emit_op(e_create_operation(E_OP_OR, e_create_null(), e_create_null()));
                ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 248 "grammar.y"
    {
                    emit_op(e_create_operation(E_OP_NOT, e_create_null(), e_create_null()));
                ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 251 "grammar.y"
    {
                    (yyval.nval) = (yyvsp[(2) - (3)].nval);
                ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 254 "grammar.y"
    { 
                    emit_op(e_create_operation(E_OP_MUL, e_create_null(), e_create_null()));
                ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 257 "grammar.y"
    { 
                    /* 3 / a */
                    emit_op(e_create_operation(E_OP_DIV, e_create_null(), e_create_null()));
                ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 261 "grammar.y"
    { 
                    /* 3 + a */
                    printf("OP1 type is %d\n", (yyvsp[(1) - (3)].nval).type);
                    printf("OP2 type is %d\n", (yyvsp[(3) - (3)].nval).type);

                    // Both math_expressions are of type string:
					// ADD ends in:
					// Result type is string
					// Concatenate both strings and store the new string in the ds
					if((yyvsp[(1) - (3)].nval).type == E_ARGT_STRING && (yyvsp[(3) - (3)].nval).type == E_ARGT_STRING) {
						char buf[E_MAX_STRLEN];
						unsigned int slen1 = strlen((yyvsp[(1) - (3)].nval).str.sval);
						unsigned int slen2 = strlen((yyvsp[(3) - (3)].nval).str.sval);
						printf("len1: %d len2: %d\n", slen1, slen2);
						if(slen1 + slen2 > E_MAX_STRLEN) {
							yyerror("String too long");
						} else {
							strcpy(buf, (yyvsp[(1) - (3)].nval).str.sval);
							strcat(buf, (yyvsp[(3) - (3)].nval).str.sval);
							printf("New - string %s\n", buf);

							// Add string data to data segment (bytecode section)
							int str_index = ds_store_string(buf);
							emit_op(e_create_operation(E_OP_PUSH, e_create_number(str_index), e_create_null()));

							(yyval.nval).type = E_STRING;
						}
					} else {
						// Numbers result in an add operation
						emit_op(e_create_operation(E_OP_ADD, e_create_null(), e_create_null()));
					}
                ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 293 "grammar.y"
    { 
                    /* 3 - a */
                    emit_op(e_create_operation(E_OP_SUB, e_create_null(), e_create_null()));
                ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 317 "grammar.y"
    {
						if(strlen((yyvsp[(1) - (1)].nval).str.sval) >= E_MAX_STRLEN) {
							yyerror("String too long");
						}
						(yyval.nval) = (yyvsp[(1) - (1)].nval);
				   ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 325 "grammar.y"
    { 
                    // Get instruction count of opening if
                    e_stack_status_ret s = e_stack_pop(&bp_stack);
                    if(s.status == E_STATUS_OK) {
                        // Patch jump dummy_addr from previous jump
                        jmp_patch(s.val.ival, addr_count);
                    }
                    
                    printf("BLOCK END\n");
                    
                    e_status_ret s_scope = e_close_scope();
                    
                    error_pprint(s.status);
                    error_pprint(s_scope.status);
               ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 342 "grammar.y"
    {
                    // Insert JNE [16 bit dummy_addr]
                    emit_op(e_create_operation(E_OP_JZ, e_create_number(0xFFFFFFFF), e_create_number(0xFFFFFFFF)));
                    
                    // Copy jmp instruction to a table (to be patched later in the if_expression)
                    e_internal_type addr =  { .ival = addr_count };
                    e_stack_status_ret s = e_stack_push(&bp_stack, addr);
                     
                    e_status_ret s_scope = e_create_scope();
                    if(s_scope.status == E_STATUS_OK) {
                        printf("created scope\n");
                    }
                    
                    error_pprint(s.status);
                    error_pprint(s_scope.status);
              ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 364 "grammar.y"
    {
                    printf("LOOP END\n");
                    
                    // Get instruction count of opening if
                    e_stack_status_ret s = e_stack_pop(&bp_stack);
                    if(s.status == E_STATUS_OK) {
                        printf("Patching loop address: %d\n", s.val.ival);
                        // Add unconditional jump (jmp) to previously stored address
                        // TODO: Support 64bit address
                        emit_op(e_create_operation(E_OP_JMP, e_create_number(s.val.ival), e_create_number(0xFFFFFFFF)));

						if(loop_has_break[loop_level]) {
							// Patch break
							// Get instruction count of opening if
							e_stack_status_ret s = e_stack_pop(&loop_stack);
							if(s.status == E_STATUS_OK) {
								jmp_patch(s.val.ival, addr_count);
								printf("patched loop break @%d with new address: %d\n", s.val.ival, addr_count);
							}
						}

                        e_status_ret s_scope = e_close_scope();
                        loop_level = scope_level;
                        
                        error_pprint(s_scope.status);
                    }
                
                    error_pprint(s.status);
                 ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 395 "grammar.y"
    { 
                // Loop creates a new scope
                e_status_ret s_scope = e_create_scope();
                if(s_scope.status == E_STATUS_OK) {
                    printf("created scope\n");
                    
                    loop_level = scope_level;
                    loop_has_break[loop_level] = false;
                    
                    // Store the line counter of the opening loop block to patch in the closing block
                    e_internal_type addr =  { .ival = addr_count };
                    e_stack_status_ret s = e_stack_push(&bp_stack, addr);

                    error_pprint(s.status);
                }
                error_pprint(s_scope.status);
            ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 418 "grammar.y"
    { (yyval.nval).type = E_NUMBER; (yyval.nval).val = (yyvsp[(1) - (1)].nval).val; ;}
    break;



/* Line 1455 of yacc.c  */
#line 1919 "grammar.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 420 "grammar.y"


void yyerror(const char* s) {
    printf("Error happend in line [%d]: %s\n", lc, s);
    exit(-1);
}

void error_pprint(e_statusc error) {
    if(error != E_STATUS_OK) {
        switch(error) {
            case E_STATUS_NOINIT:
                yyerror("Symbol not inizialized\n");
                break;
            case E_STATUS_NESIZE:
                yyerror("Not enough size\n");
                break;
            case E_STATUS_ALRDYDEF:
                yyerror("Symbol already defined\n");
                break;
            case E_STATUS_NOTFOUND:
                yyerror("Symbol not defined\n");
                break;
            default:
                yyerror("Syntax error");
        }
    }
}

void inc_cnt(void) {
    addr_count++;
}

void jmp_patch(unsigned int start_addr, unsigned int end_addr) {
    /* Patches a conditional jump dummy 16 bit address with the given end_addr */
    printf("PATCH jump @%d with new addr: %d\n", start_addr, end_addr);
    // TODO: Replace magic numbers with sizeof or defined constant
    // TODO: This can me a macro!
    out_bytes[((start_addr - 1) * 9) + 1] = (uint8_t)((end_addr >> 24) & 0xFF);
    out_bytes[((start_addr - 1) * 9) + 2] = (uint8_t)((end_addr >> 16) & 0xFF);
    out_bytes[((start_addr - 1) * 9) + 3] = (uint8_t)((end_addr >> 8) & 0xFF);
    out_bytes[((start_addr - 1) * 9) + 4] = (uint8_t)(end_addr & 0xFF);
}

void emit_op(e_op op) {
    /* Emits (prints) an OP with up to 2 args */
    inc_cnt();
    //printf("[%d]", addr_count);
    uint8_t barr[E_SYS_SIZE_DOUBLE];
    
    e_byte_op byte_op = { .opcode = (uint8_t)op.opcode };
    
    switch(op.opcode) {
        case E_OP_PUSHG:
            printf("PUSHG [%d]\n", (int)op.op1.val);
            byte_op.op1 = (uint32_t)op.op1.val;
            byte_op.op2 = (uint32_t)op.op2.val;
            break;
        case E_OP_POPG:
            printf("POPG [%d]\n", (int)op.op1.val);
            byte_op.op1 = (uint32_t)op.op1.val;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_PUSHL:
            printf("PUSHL [%d]\n", (int)op.op1.val);
            byte_op.op1 = (uint32_t)op.op1.val;
            byte_op.op2 = (uint32_t)op.op2.val;
            
            printf("************************ SYMBOL TABLE [%d] **\n", scope_level);
            for(unsigned int i=0; i < local_sym_table[scope_level].entries; i++) {
                printf("[%d] %s\n", i, local_sym_table[scope_level].tab_ptr[i].idname);
            }
            printf("****************************************\n");
            
            break;
        case E_OP_POPL:
            printf("POPL [%d]\n", (int)op.op1.val);
            byte_op.op1 = (uint32_t)op.op1.val;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_PUSH:
            switch(op.op1.argtype) {
                case E_ARGT_NUMBER:
                    printf("PUSH %f\n", op.op1.val);
                    double_to_bytearray(op.op1.val, barr);
                    byte_op.op1 = (barr[7] << 24) | (barr[6] << 16) | (barr[5] << 8) | barr[4];
                    byte_op.op2 = (barr[3] << 24) | (barr[2] << 16) | (barr[1] << 8) | barr[0];
                    break;
                }
            break;
        case E_OP_EQ:
            printf("EQ\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_LT:
            printf("LT\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_GT:
            printf("GT\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_LTEQ:
            printf("LTEQ\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_GTEQ:
            printf("GTEQ\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_ADD:
            printf("ADD\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_SUB:
            printf("SUB\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_MUL:
            printf("MUL\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_DIV:
            printf("DIV\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_AND:
            printf("AND\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_OR:
            printf("OR\n");
            byte_op.op1 = (uint32_t)0;
            byte_op.op2 = (uint32_t)0;
            break;
        case E_OP_JZ:
            printf("JZ [%d %d]\n", (int)op.op1.val, (int)op.op2.val);
            byte_op.op1 = (uint32_t)op.op1.val;
            byte_op.op2 = (uint32_t)op.op2.val;
            break;
        case E_OP_JMP:
            byte_op.op1 = (uint32_t)op.op1.val;
            byte_op.op2 = (uint32_t)op.op2.val;
            break;
        }
        
        //printf("-----(out_b_cnt: %d)\n", out_b_cnt);
        if(out_b_cnt + E_OP_BSIZE >= E_OUT_TOTAL_SIZE) {
        	yyerror("Program space exhausted");
        }
        
        out_bytes[out_b_cnt++] = (uint8_t)byte_op.opcode;
        out_bytes[out_b_cnt++] = (uint8_t)((byte_op.op1 >> 24) & 0xFF);
        out_bytes[out_b_cnt++] = (uint8_t)((byte_op.op1 >> 16) & 0xFF);
        out_bytes[out_b_cnt++] = (uint8_t)((byte_op.op1 >> 8) & 0xFF);
        out_bytes[out_b_cnt++] = (uint8_t)(byte_op.op1 & 0xFF);
        
        out_bytes[out_b_cnt++] = (uint8_t)((byte_op.op2 >> 24) & 0xFF);
        out_bytes[out_b_cnt++] = (uint8_t)((byte_op.op2 >> 16) & 0xFF);
        out_bytes[out_b_cnt++] = (uint8_t)((byte_op.op2 >> 8) & 0xFF);
        out_bytes[out_b_cnt++] = (uint8_t)(byte_op.op2 & 0xFF);
}

void double_to_bytearray(double din, uint8_t bin[]) {
    union {
        double d;
        uint8_t b[E_SYS_SIZE_DOUBLE];
    } u;
    u.d = din;
    memcpy(bin, u.b, E_SYS_SIZE_DOUBLE);
}

int ds_store_string(const char* str) {
    // Stores a string in the required format [LENGTH, 2 Bytes][<str data>]
    uint32_t r_len = strlen(str);
    if(r_len > UINT16_MAX || out_ds_cnt + 1 > E_OUT_TOTAL_SIZE) {
    	yyerror("String literal is too large");
    } else {
    	uint32_t start_index = out_ds_cnt;
    	uint16_t len = (uint16_t)r_len;

    	out_bytes[out_ds_cnt++] = (uint8_t)((len >> 8) & 0xFF);
    	out_bytes[out_ds_cnt++] = (uint8_t)(len & 0xFF);
    	for(uint16_t i = 0; i < len; i++) {
    		out_bytes[out_ds_cnt++] = (uint8_t)str[i];
    	}
    	return start_index;
    }
    return -1;
}

void print_outstream(void) {
    printf("print stream *****\n");
    unsigned int r = 0;
    unsigned int print_out = 0;
    for(unsigned int i = 0; i < E_OUT_TOTAL_SIZE; i++) {
        if(i%9 == 0 && !print_out) {
            printf("[%d] ", i / 9);
        }
    
        printf("0x%02X ", out_bytes[i]);
        
        if(r == 0 && out_bytes[i] == 0x00) {
            // Opcode 0x00, end
            if(!print_out) {
				printf("\n -- DATA SEGMENT -- \n");
				i = E_OUT_SIZE;
				print_out = 1;
			}
        }
    
        if(++r == 9) {
            printf("\n");
            r = 0;
        }
        
        
    }
    printf("\n");
}

