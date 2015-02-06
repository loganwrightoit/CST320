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
#include <iostream>
using namespace std;

Parser::Parser()
{
}

Parser::~Parser()
{
}

bool Parser::equals(Tokenizer::TokenType type)
{
    return type == pos->type();
}

bool Parser::equals(char* input)
{
    return input == pos->value();
}

void Parser::save()
{
    lastPos = pos;
}

// Restores position and returns false;
bool Parser::failure()
{
    cout << "[DEBUG]: Parser failed at token: " << pos->value().c_str() << ", at position: " << pos->pos() << endl;

    pos = lastPos;
    return false;
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
    save();

    if (type())
    {
        ++pos;
        if (equals(Tokenizer::Identifier))
        {
            ++pos;
            if (equals("("))
            {
                ++pos;
                if (arg_list())
                {
                    ++pos;
                    if (equals(")"))
                    {
                        ++pos;
                        if (compound_stmt())
                        {
                            ++pos;
                            return true;
                        }
                    }
                }
            }
        }
    }

    return failure();
}

// ARG_LIST → ARG ARG_LIST2
bool Parser::arg_list()
{
    save();

    if (arg())
    {
        ++pos;
        if (arg_list2())
        {
            ++pos;
            return true;
        }
    }

    return failure();
}

// ARG_LIST2 → , ARG ARG_LIST2 | λ
bool Parser::arg_list2()
{
    save();

    if (equals(","))
    {
        ++pos;
        if (arg())
        {
            ++pos;
            if (arg_list2())
            {
                ++pos;
                return true;
            }
        }
        return failure();
    }

    return true; // λ
}

// ARG → TYPE Identifier
bool Parser::arg()
{
    save();

    if (type())
    {
        ++pos;
        if (equals(Tokenizer::Identifier))
        {
            ++pos;
            return true;
        }
    }

    return failure();
}

// DECLARATION → TYPE IDENT_LIST ;
bool Parser::declaration()
{
    save();

    if (type())
    {
        ++pos;
        if (ident_list())
        {
            ++pos;
            if (equals(";"))
            {
                ++pos;
                return true;
            }
        }
    }

    return failure();
}

// TYPE → int | float | string | true | false
bool Parser::type()
{
    if (equals("int") || equals("float") || equals("true") || equals("false") || equals("string"))
    {
        ++pos;
        return true;
    }

    return false;
}

// IDENT_LIST → Identifier , IDENT_LIST Identifier
bool Parser::ident_list()
{
    save();

    if (equals(Tokenizer::Identifier))
    {
        ++pos;
        if (equals(","))
        {
            ++pos;
            if (ident_list())
            {
                ++pos;
                if (equals(Tokenizer::Identifier))
                {
                    ++pos;
                    return true;
                }
            }
        }
        return failure();
    }

    return true; // λ
}

// STATEMENT → FOR_STMT | WHILE_STMT | EXPRESSION ; | IF_STMT | COMPOUND_STMT | DECLARATION | ;
bool Parser::statement()
{
    save();

    if (for_stmt())
    {
        ++pos;
        return true;
    }
    else if (while_stmt())
    {
        ++pos;
        return true;
    }
    else if (expression())
    {
        ++pos;
        if (equals(";"))
        {
            ++pos;
            return true;
        }
    }
    else if (if_stmt())
    {
        ++pos;
        return true;
    }
    else if (compound_stmt())
    {
        ++pos;
        return true;
    }
    else if (declaration())
    {
        ++pos;
        return true;
    }
    else if (pos->value() == ";")
    {
        ++pos;
        return true;
    }

    return failure();
}

// FOR_STMT → for ( EXPRESSION ; OPT_EXPR ; OPT_EXPR ) STATEMENT
bool Parser::for_stmt()
{
    save();

    if (equals("for"))
    {
        ++pos;
        if (equals("("))
        {
            ++pos;
            if (expression())
            {
                ++pos;
                if (equals(";"))
                {
                    ++pos;
                    if (opt_expr())
                    {
                        ++pos;
                        if (equals(";"))
                        {
                            ++pos;
                            if (opt_expr())
                            {
                                ++pos;
                                if (equals(")"))
                                {
                                    ++pos;
                                    if (statement())
                                    {
                                        ++pos;
                                        return true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return failure();
}

// OPT_EXPR → EXPRESSION | λ
bool Parser::opt_expr()
{
    if (expression())
    {
        ++pos;
        return true;
    }

    return true; // λ
}

// WHILE_STMT → while ( EXPRESSION ) STATEMENT
bool Parser::while_stmt()
{
    save();

    if (equals("while"))
    {
        ++pos;
        if (equals("("))
        {
            ++pos;
            if (expression())
            {
                ++pos;
                if (equals(")"))
                {
                    ++pos;
                    if (statement())
                    {
                        ++pos;
                        return true;
                    }
                }
            }
        }
    }

    return failure();
}

// IF_STMT → if ( EXPRESSION ) STATEMENT ELSEPART
bool Parser::if_stmt()
{
    save();

    if (equals("if"))
    {
        ++pos;
        if (equals("("))
        {
            ++pos;
            if (expression())
            {
                ++pos;
                if (equals(")"))
                {
                    ++pos;
                    if (statement())
                    {
                        ++pos;
                        if (elsepart())
                        {
                            ++pos;
                            return true;
                        }
                    }
                }
            }
        }
    }

    return failure();
}

// ELSEPART → else STATEMENT | λ
bool Parser::elsepart()
{
    save();

    if (equals("else"))
    {
        ++pos;
        if (statement())
        {
            ++pos;
            return true;
        }
        return failure();
    }

    return true; // λ
}

// COMPOUND_STMT → { STMT_LIST }
bool Parser::compound_stmt()
{
    save();

    if (equals("{"))
    {
        ++pos;
        if (stmt_list())
        {
            ++pos;
            if (equals("}"))
            {
                ++pos;
                return true;
            }
        }
    }

    return failure();
}

// STMT_LIST → STATEMENT STMT_LIST | λ
bool Parser::stmt_list()
{
    save();

    if (statement())
    {
        ++pos;
        if (stmt_list())
        {
            ++pos;
            return true;
        }
        return failure();
    }

    return true; // λ
}

// EXPRESSION → Identifier = EXPRESSION | RVALUE
bool Parser::expression()
{
    save();

    if (equals(Tokenizer::Identifier))
    {
        ++pos;
        if (equals("="))
        {
            ++pos;
            if (expression())
            {
                ++pos;
                return true;
            }
        }
    }
    else if (rvalue())
    {
        ++pos;
        return true;
    }

    return failure();
}

// RVALUE → MAG RVALUE2
bool Parser::rvalue()
{
    save();

    if (mag())
    {
        ++pos;
        if (rvalue2())
        {
            ++pos;
            return true;
        }
    }

    return failure();
}

// RVALUE2 → COMPARE MAG RVALUE2 | λ
bool Parser::rvalue2()
{
    save();

    if (compare())
    {
        ++pos;
        if (mag())
        {
            ++pos;
            if (rvalue2())
            {
                ++pos;
                return true;
            }
        }
        return failure();
    }

    return true; // λ
}

// COMPARE → == | < | > | <= | >= | !=
bool Parser::compare()
{
    save();

    if (equals("=="))
    {
        ++pos;
        return true;
    }
    else if (equals("<"))
    {
        ++pos;
        return true;
    }
    else if (equals(">"))
    {
        ++pos;
        return true;
    }
    else if (equals("<="))
    {
        ++pos;
        return true;
    }
    else if (equals(">="))
    {
        ++pos;
        return true;
    }
    else if (equals("!="))
    {
        ++pos;
        return true;
    }

    return failure();
}

// MAG → TERM MAG2
bool Parser::mag()
{
    save();

    if (term())
    {
        ++pos;
        if (mag2())
        {
            ++pos;
            return true;
        }
    }

    return failure();
}

// MAG2 → + TERM MAG2 | - TERM MAG2 | λ
bool Parser::mag2()
{
    save();

    if (equals("+") || equals("-"))
    {
        ++pos;
        if (term())
        {
            ++pos;
            if (mag2())
            {
                ++pos;
                return true;
            }
        }
        return failure();
    }

    return true; // λ
}

// TERM → FACTOR TERM2
bool Parser::term()
{
    save();

    if (factor())
    {
        ++pos;
        if (term2())
        {
            ++pos;
            return true;
        }
    }

    return failure();
}

// TERM2 → * FACTOR TERM2 | / FACTOR TERM2 | λ
bool Parser::term2()
{
    save();

    if (equals("*") || equals("/"))
    {
        ++pos;
        if (factor())
        {
            ++pos;
            if (term2())
            {
                ++pos;
                return true;
            }
        }
        return failure();
    }

    return true; // λ
}

// FACTOR → ( EXPRESSION ) | - FACTOR | + FACTOR | Identifier | TYPE
bool Parser::factor()
{
    save();

    if (equals("("))
    {
        ++pos;
        if (expression())
        {
            ++pos;
            if (equals(")"))
            {
                ++pos;
                return true;
            }
        }
    }
    else if (equals("-") || equals("+"))
    {
        ++pos;
        if (factor())
        {
            ++pos;
            return true;
        }
    }
    else if (equals(Tokenizer::Identifier))
    {
        ++pos;
        return true;
    }
    else if (type())
    {
        ++pos;
        return true;
    }

    return failure();
}