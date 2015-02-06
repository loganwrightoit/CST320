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

bool Parser::isNextType(Tokenizer::TokenType type)
{
    if (type == (pos + 1)->type)
    {
        ++pos;
        return true;
    }
    else
    {
        return false;
    }
}

bool Parser::isNextValue(char* input)
{
    if (input == (pos + 1)->value)
    {
        ++pos;
        return true;
    }
    else
    {
        return false;
    }
}

bool Parser::parse(std::vector<Tokenizer::Token> tokens)
{
    pos = tokens.begin();

    // Assume int main() { COMPOUND_STMT } comes first
    return function();
}

// FUNCTION → TYPE Identifier ( ARG_LIST ) COMPOUND_STMT
bool Parser::function()
{
    if (type())
    {
        if (isNextType(Tokenizer::Identifier))
        {
            if (isNextValue("("))
            {
                if (arg_list())
                {
                    if (isNextValue(")"))
                    {
                        return compound_stmt();
                    }
                }
            }
        }
    }

    return false;
}

// ARG_LIST → ARG | ARG_LIST , ARG
bool Parser::arg_list()
{

    return false;
}

// ARG → TYPE Identifier
bool Parser::arg()
{

    return false;
}

// DECLARATION → TYPE IDENT_LIST ;
bool Parser::declaration()
{

    return false;
}

// TYPE → int | float
bool Parser::type()
{

    return false;
}

// IDENT_LIST → Identifier , IDENT_LIST Identifier
bool Parser::ident_list()
{

    return false;
}

// STATEMENT → FOR_STMT | WHILE_STMT | EXPRESSION ; | IF_STMT | COMPOUND_STMT | DECLARATION | ;
bool Parser::statement()
{

    return false;
}

// FOR_STMT → for ( EXPRESSION ; OPT_EXPR ; OPT_EXPR ) STATEMENT
bool Parser::for_stmt()
{

    return false;
}

// OPT_EXPR → EXPRESSION | λ
bool Parser::opt_expr()
{

    return false;
}

// WHILE_STMT → while ( EXPRESSION ) STATEMENT
bool Parser::while_stmt()
{

    return false;
}

// IF_STMT → if ( EXPRESSION ) STATEMENT ELSEPART
bool Parser::if_stmt()
{

    return false;
}

// ELSEPART → else STATEMENT | λ
bool Parser::elsepart()
{

    return false;
}

// COMPOUND_STMT → { STMT_LIST }
bool Parser::compound_stmt()
{

    return false;
}

// STMT_LIST → STMT_LIST STATEMENT | λ
bool Parser::stmt_list()
{

    return false;
}

// EXPRESSION → Identifier = EXPRESSION | RVALUE
bool Parser::expression()
{

    return false;
}

// RVALUE → RVALUE COMPARE MAG | MAG
bool Parser::rvalue()
{

    return false;
}

// COMPARE → == | < | > | <= | >= | !=
bool Parser::compare()
{

    return false;
}

// MAG → MAG + TERM | MAG - TERM | TERM
bool Parser::mag()
{

    return false;
}

// TERM → TERM * FACTOR | TERM / FACTOR | FACTOR
bool Parser::term()
{

    return false;
}

// FACTOR → ( EXPRESSION ) | - FACTOR | + FACTOR | Identifier | TYPE
bool Parser::factor()
{

    return false;
}