
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
     TOKEN_ID = 258,
     TOKEN_TIPO = 259,
     TOKEN_INT = 260,
     TOKEN_RCOLCH = 261,
     TOKEN_SEMICOLON = 262,
     TOKEN_LCOLCH = 263,
     TOKEN_LPAREN = 264,
     TOKEN_RPAREN = 265,
     TOKEN_EOF = 266,
     TOKEN_PLUS = 267,
     TOKEN_MINUS = 268,
     TOKEN_MULT = 269,
     TOKEN_DIV = 270,
     TOKEN_ASSIGN = 271,
     TOKEN_COMP = 272,
     TOKEN_MENOR = 273,
     TOKEN_MENORIG = 274,
     TOKEN_MAIOR = 275,
     TOKEN_MAIORIG = 276,
     TOKEN_LCHAVE = 277,
     TOKEN_RCHAVE = 278,
     TOKEN_COLON = 279,
     TOKEN_DIF = 280,
     START = 281,
     COMENTARIO = 282,
     BRANCO = 283,
     DONE = 284,
     SIMBOLO = 285,
     TOKEN_UNKNOWN = 286,
     TOKEN_WHILE = 287,
     TOKEN_IF = 288,
     TOKEN_ELSE = 289,
     TOKEN_RETURN = 290,
     TOKEN_DECL = 291,
     TOKEN_PARAM = 292,
     TOKEN_CALL = 293,
     YYEOF = 294
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


