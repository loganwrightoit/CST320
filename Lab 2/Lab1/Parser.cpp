/***********************************************************
* Author:                   Logan Wright
* Date Created:             02/06/2015
* Last Modification Date:   02/06/2015
* Lab Number:               CST 320 Lab 2
* Filename:                 Parser.cpp
*
* Overview:
*   Parses a file using a Mini-C grammar.
*
* Input:
*   None.
*
* Output:
*   None.
************************************************************/

#include "Parser.h"

Parser::Parser()
{
}

Parser::~Parser()
{
}

bool Parser::parse(std::vector<Tokenizer::Token> tokens)
{


    return true;
}

/*
    FUNCTION	  →   TYPE Identifier ( ARG_LIST ) COMPOUND_STMT
    --------------------------------------------------
    ARG_LIST	  →	  ARG
                    | ARG_LIST , ARG
    --------------------------------------------------
    ARG           →	  TYPE Identifier
    --------------------------------------------------
    DECLARATION	  →	  TYPE IDENT_LIST ;
    --------------------------------------------------
    TYPE	      →   Integer
                    | Float
    --------------------------------------------------
    IDENT_LIST	  →	  Identifier , IDENT_LIST Identifier
    --------------------------------------------------
    STATEMENT	  →	  FOR_STMT
                    | WHILE_STMT
                    | EXPRESSION ;
                    | IF_STMT
                    | COMPOUND_STMT
                    | DECLARATION
                    | ;
    --------------------------------------------------
    FOR_STMT	  →	  for ( EXPRESSION ; OPT_EXPR ; OPT_EXPR ) STATEMENT
    --------------------------------------------------
    OPT_EXPR	  →	  EXPRESSION
                    | λ
    --------------------------------------------------
    WHILE_STMT    →   while ( EXPRESSION ) STATEMENT
    --------------------------------------------------
    IF_STMT	      →	  if ( EXPRESSION ) STATEMENT ELSEPART
    --------------------------------------------------
    ELSEPART	  →	  else STATEMENT
                    | λ
    --------------------------------------------------
    COMPOUND_STMT →	  { STMT_LIST }
    --------------------------------------------------
    STMT_LIST	  →	  STMT_LIST STATEMENT
                    | λ
    --------------------------------------------------
    EXPRESSION	  →	  Identifier = EXPRESSION
                    | RVALUE
    --------------------------------------------------
    RVALUE	      →   RVALUE COMPARE MAG
                    | MAG
    --------------------------------------------------
    COMPARE	      →   ==
                    | <
                    | >
                    | <=
                    | >=
                    | !=
    --------------------------------------------------
    MAG	          →   MAG + TERM
                    | MAG - TERM
                    | TERM
    --------------------------------------------------
    TERM	      →	  TERM * FACTOR
                    | TERM / FACTOR
                    | FACTOR
    --------------------------------------------------
    FACTOR	      →	  ( EXPRESSION )
                    | - FACTOR
                    | + FACTOR
                    | Identifier
                    | TYPE
*/