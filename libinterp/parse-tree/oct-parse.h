/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

#ifndef YY_OCTAVE_LIBINTERP_PARSE_TREE_OCT_PARSE_H_INCLUDED
# define YY_OCTAVE_LIBINTERP_PARSE_TREE_OCT_PARSE_H_INCLUDED
/* Debug traces.  */
#ifndef OCTAVE_DEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define OCTAVE_DEBUG 1
#  else
#   define OCTAVE_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define OCTAVE_DEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined OCTAVE_DEBUG */
#if OCTAVE_DEBUG
extern int octave_debug;
#endif

/* Token type.  */
#ifndef OCTAVE_TOKENTYPE
# define OCTAVE_TOKENTYPE
  enum octave_tokentype
  {
    ADD_EQ = 258,
    SUB_EQ = 259,
    MUL_EQ = 260,
    DIV_EQ = 261,
    LEFTDIV_EQ = 262,
    POW_EQ = 263,
    EMUL_EQ = 264,
    EDIV_EQ = 265,
    ELEFTDIV_EQ = 266,
    EPOW_EQ = 267,
    AND_EQ = 268,
    OR_EQ = 269,
    EXPR_AND_AND = 270,
    EXPR_OR_OR = 271,
    EXPR_AND = 272,
    EXPR_OR = 273,
    EXPR_NOT = 274,
    EXPR_LT = 275,
    EXPR_LE = 276,
    EXPR_EQ = 277,
    EXPR_NE = 278,
    EXPR_GE = 279,
    EXPR_GT = 280,
    LEFTDIV = 281,
    EMUL = 282,
    EDIV = 283,
    ELEFTDIV = 284,
    EPLUS = 285,
    EMINUS = 286,
    HERMITIAN = 287,
    TRANSPOSE = 288,
    PLUS_PLUS = 289,
    MINUS_MINUS = 290,
    POW = 291,
    EPOW = 292,
    NUM = 293,
    IMAG_NUM = 294,
    STRUCT_ELT = 295,
    NAME = 296,
    END = 297,
    DQ_STRING = 298,
    SQ_STRING = 299,
    FOR = 300,
    PARFOR = 301,
    WHILE = 302,
    DO = 303,
    UNTIL = 304,
    IF = 305,
    ELSEIF = 306,
    ELSE = 307,
    SWITCH = 308,
    CASE = 309,
    OTHERWISE = 310,
    BREAK = 311,
    CONTINUE = 312,
    FUNC_RET = 313,
    UNWIND = 314,
    CLEANUP = 315,
    TRY = 316,
    CATCH = 317,
    GLOBAL = 318,
    PERSISTENT = 319,
    FCN_HANDLE = 320,
    CLASSDEF = 321,
    PROPERTIES = 322,
    METHODS = 323,
    EVENTS = 324,
    ENUMERATION = 325,
    METAQUERY = 326,
    SUPERCLASSREF = 327,
    FQ_IDENT = 328,
    GET = 329,
    SET = 330,
    FCN = 331,
    LEXICAL_ERROR = 332,
    END_OF_INPUT = 333,
    INPUT_FILE = 334,
    UNARY = 335
  };
#endif
/* Tokens.  */
#define ADD_EQ 258
#define SUB_EQ 259
#define MUL_EQ 260
#define DIV_EQ 261
#define LEFTDIV_EQ 262
#define POW_EQ 263
#define EMUL_EQ 264
#define EDIV_EQ 265
#define ELEFTDIV_EQ 266
#define EPOW_EQ 267
#define AND_EQ 268
#define OR_EQ 269
#define EXPR_AND_AND 270
#define EXPR_OR_OR 271
#define EXPR_AND 272
#define EXPR_OR 273
#define EXPR_NOT 274
#define EXPR_LT 275
#define EXPR_LE 276
#define EXPR_EQ 277
#define EXPR_NE 278
#define EXPR_GE 279
#define EXPR_GT 280
#define LEFTDIV 281
#define EMUL 282
#define EDIV 283
#define ELEFTDIV 284
#define EPLUS 285
#define EMINUS 286
#define HERMITIAN 287
#define TRANSPOSE 288
#define PLUS_PLUS 289
#define MINUS_MINUS 290
#define POW 291
#define EPOW 292
#define NUM 293
#define IMAG_NUM 294
#define STRUCT_ELT 295
#define NAME 296
#define END 297
#define DQ_STRING 298
#define SQ_STRING 299
#define FOR 300
#define PARFOR 301
#define WHILE 302
#define DO 303
#define UNTIL 304
#define IF 305
#define ELSEIF 306
#define ELSE 307
#define SWITCH 308
#define CASE 309
#define OTHERWISE 310
#define BREAK 311
#define CONTINUE 312
#define FUNC_RET 313
#define UNWIND 314
#define CLEANUP 315
#define TRY 316
#define CATCH 317
#define GLOBAL 318
#define PERSISTENT 319
#define FCN_HANDLE 320
#define CLASSDEF 321
#define PROPERTIES 322
#define METHODS 323
#define EVENTS 324
#define ENUMERATION 325
#define METAQUERY 326
#define SUPERCLASSREF 327
#define FQ_IDENT 328
#define GET 329
#define SET 330
#define FCN 331
#define LEXICAL_ERROR 332
#define END_OF_INPUT 333
#define INPUT_FILE 334
#define UNARY 335

/* Value type.  */
#if ! defined OCTAVE_STYPE && ! defined OCTAVE_STYPE_IS_DECLARED

union OCTAVE_STYPE
{
#line 136 "libinterp/parse-tree/oct-parse.yy" /* yacc.c:1909  */

  int dummy_type;

  // The type of the basic tokens returned by the lexer.
  octave::token *tok_val;

  // Comment strings that we need to deal with mid-rule.
  octave::comment_list *comment_type;

  // Types for the nonterminals we generate.
  char punct_type;
  octave::tree *tree_type;
  octave::tree_matrix *tree_matrix_type;
  octave::tree_cell *tree_cell_type;
  octave::tree_expression *tree_expression_type;
  octave::tree_constant *tree_constant_type;
  octave::tree_fcn_handle *tree_fcn_handle_type;
  octave::tree_funcall *tree_funcall_type;
  octave::tree_function_def *tree_function_def_type;
  octave::tree_anon_fcn_handle *tree_anon_fcn_handle_type;
  octave::tree_identifier *tree_identifier_type;
  octave::tree_index_expression *tree_index_expression_type;
  octave::tree_colon_expression *tree_colon_expression_type;
  octave::tree_argument_list *tree_argument_list_type;
  octave::tree_parameter_list *tree_parameter_list_type;
  octave::tree_command *tree_command_type;
  octave::tree_if_command *tree_if_command_type;
  octave::tree_if_clause *tree_if_clause_type;
  octave::tree_if_command_list *tree_if_command_list_type;
  octave::tree_switch_command *tree_switch_command_type;
  octave::tree_switch_case *tree_switch_case_type;
  octave::tree_switch_case_list *tree_switch_case_list_type;
  octave::tree_decl_elt *tree_decl_elt_type;
  octave::tree_decl_init_list *tree_decl_init_list_type;
  octave::tree_decl_command *tree_decl_command_type;
  octave::tree_statement *tree_statement_type;
  octave::tree_statement_list *tree_statement_list_type;
  octave_user_function *octave_user_function_type;

  octave::tree_classdef *tree_classdef_type;
  octave::tree_classdef_attribute* tree_classdef_attribute_type;
  octave::tree_classdef_attribute_list* tree_classdef_attribute_list_type;
  octave::tree_classdef_superclass* tree_classdef_superclass_type;
  octave::tree_classdef_superclass_list* tree_classdef_superclass_list_type;
  octave::tree_classdef_body* tree_classdef_body_type;
  octave::tree_classdef_property* tree_classdef_property_type;
  octave::tree_classdef_property_list* tree_classdef_property_list_type;
  octave::tree_classdef_properties_block* tree_classdef_properties_block_type;
  octave::tree_classdef_methods_list* tree_classdef_methods_list_type;
  octave::tree_classdef_methods_block* tree_classdef_methods_block_type;
  octave::tree_classdef_event* tree_classdef_event_type;
  octave::tree_classdef_events_list* tree_classdef_events_list_type;
  octave::tree_classdef_events_block* tree_classdef_events_block_type;
  octave::tree_classdef_enum* tree_classdef_enum_type;
  octave::tree_classdef_enum_list* tree_classdef_enum_list_type;
  octave::tree_classdef_enum_block* tree_classdef_enum_block_type;

#line 280 "libinterp/parse-tree/oct-parse.h" /* yacc.c:1909  */
};

typedef union OCTAVE_STYPE OCTAVE_STYPE;
# define OCTAVE_STYPE_IS_TRIVIAL 1
# define OCTAVE_STYPE_IS_DECLARED 1
#endif



#ifndef YYPUSH_MORE_DEFINED
# define YYPUSH_MORE_DEFINED
enum { YYPUSH_MORE = 4 };
#endif

typedef struct octave_pstate octave_pstate;

int octave_parse (octave::base_parser& parser);
int octave_push_parse (octave_pstate *ps, int pushed_char, OCTAVE_STYPE const *pushed_val, octave::base_parser& parser);
int octave_pull_parse (octave_pstate *ps, octave::base_parser& parser);
octave_pstate * octave_pstate_new (void);
void octave_pstate_delete (octave_pstate *ps);

#endif /* !YY_OCTAVE_LIBINTERP_PARSE_TREE_OCT_PARSE_H_INCLUDED  */
