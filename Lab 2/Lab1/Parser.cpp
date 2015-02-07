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
    return type == token->type();
}

bool Parser::equals(char* input)
{
    return input == token->value();
}

bool Parser::parse(std::vector<Tokenizer::Token> tokens)
{
    token = tokens.begin();
    end = tokens.end();

    // Assume int main() { COMPOUND_STMT } comes first
    if (token == end) { return false; }
    return function();
}

// FUNCTION → TYPE Identifier ( ARG_LIST ) COMPOUND_STMT
bool Parser::function()
{
    auto temp = token;
    
    if (token == end) { return false; }
    if (type())
    {
        cout << "[DEBUG]: FUNCTION -> TYPE" << endl;
        if (token == end) { return false; }
        if (equals(Tokenizer::Identifier))
        {
            cout << "[DEBUG]: FUNCTION -> TYPE Identifier" << endl;
            ++token;
            if (token == end) { return false; }
            if (equals("("))
            {
                cout << "[DEBUG]: FUNCTION -> TYPE Identifier (" << endl;
                ++token;
                if (token == end) { return false; }
                if (arg_list())
                {
                    cout << "[DEBUG]: FUNCTION -> TYPE Identifier ( ARG_LIST" << endl;
                    if (token == end) { return false; }
                    if (equals(")"))
                    {
                        cout << "[DEBUG]: FUNCTION -> TYPE Identifier ( ARG_LIST )" << endl;
                        ++token;
                        if (token == end) { return false; }
                        if (compound_stmt())
                        {
                            cout << "[DEBUG]: FUNCTION -> TYPE Identifier ( ARG_LIST ) COMPOUND_STMT" << endl;
                            return true;
                        }
                    }
                }
            }
        }
    }

    token = temp;
    return false;
}

// ARG_LIST → ARG ARG_LIST2
bool Parser::arg_list()
{
    auto temp = token;

    if (token == end) { return false; }
    if (arg())
    {
        ++token;
        if (token == end) { return false; }
        if (arg_list2())
        {
            ++token;
            return true;
        }
    }

    token = temp;
    return false;
}

// ARG_LIST2 → , ARG ARG_LIST2 | λ
bool Parser::arg_list2()
{
    auto temp = token;

    if (token == end) { return true; }
    if (equals(","))
    {
        ++token;
        if (token == end) { return false; }
        if (arg())
        {
            ++token;
            if (token == end) { return false; }
            if (arg_list2())
            {
                ++token;
                return true;
            }
        }
        return false;
    }

    token = temp;
    return true; // λ
}

// ARG → TYPE Identifier
bool Parser::arg()
{
    auto temp = token;

    if (token == end) { return false; }
    if (type())
    {
        ++token;
        if (token == end) { return false; }
        if (equals(Tokenizer::Identifier))
        {
            ++token;
            return true;
        }
    }

    token = temp;
    return false;
}

// DECLARATION → TYPE IDENT_LIST ;
bool Parser::declaration()
{
    auto temp = token;

    if (token == end) { return false; }
    if (type())
    {
        ++token;
        if (token == end) { return false; }
        if (ident_list())
        {
            ++token;
            if (token == end) { return false; }
            if (equals(";"))
            {
                ++token;
                return true;
            }
        }
    }

    token = temp;
    return false;
}

// TYPE → int | float | string | true | false
bool Parser::type()
{
    auto temp = token;

    if (token == end) { return false; }
    if (equals("int") || equals("float") || equals("true") || equals("false") || equals("string"))
    {
        cout << "[DEBUG]: TYPE resolved: " << token->value().c_str() << endl;
        ++token;
        return true;
    }

    token = temp;
    return false;
}

// IDENT_LIST → Identifier , IDENT_LIST Identifier | λ
bool Parser::ident_list()
{
    auto temp = token;

    if (token == end) { return true; }
    if (equals(Tokenizer::Identifier))
    {
        ++token;
        if (token == end) { return false; }
        if (equals(","))
        {
            ++token;
            if (token == end) { return false; }
            if (ident_list())
            {
                ++token;
                if (token == end) { return false; }
                if (equals(Tokenizer::Identifier))
                {
                    ++token;
                    return true;
                }
            }
        }
    }

    token = temp;
    return true; // λ
}

// STATEMENT → FOR_STMT | WHILE_STMT | EXPRESSION ; | IF_STMT | COMPOUND_STMT | DECLARATION | ;
bool Parser::statement()
{
    auto temp = token;

    if (token == end) { return false; }
    if (for_stmt())
    {
        ++token;
        return true;
    }
    else if (while_stmt())
    {
        ++token;
        return true;
    }
    else if (expression())
    {
        ++token;
        if (token == end) { return false; }
        if (equals(";"))
        {
            ++token;
            return true;
        }
    }
    else if (if_stmt())
    {
        ++token;
        return true;
    }
    else if (compound_stmt())
    {
        ++token;
        return true;
    }
    else if (declaration())
    {
        ++token;
        return true;
    }
    else if (equals(";"))
    {
        ++token;
        return true;
    }

    token = temp;
    return false;
}

// FOR_STMT → for ( EXPRESSION ; OPT_EXPR ; OPT_EXPR ) STATEMENT
bool Parser::for_stmt()
{
    auto temp = token;

    if (token == end) { return false; }
    if (equals("for"))
    {
        ++token;
        if (token == end) { return false; }
        if (equals("("))
        {
            ++token;
            if (token == end) { return false; }
            if (expression())
            {
                ++token;
                if (token == end) { return false; }
                if (equals(";"))
                {
                    ++token;
                    if (token == end) { return false; }
                    if (opt_expr())
                    {
                        ++token;
                        if (token == end) { return false; }
                        if (equals(";"))
                        {
                            ++token;
                            if (token == end) { return false; }
                            if (opt_expr())
                            {
                                ++token;
                                if (token == end) { return false; }
                                if (equals(")"))
                                {
                                    ++token;
                                    if (token == end) { return false; }
                                    if (statement())
                                    {
                                        ++token;
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

    token = temp;
    return false;
}

// OPT_EXPR → EXPRESSION | λ
bool Parser::opt_expr()
{
    auto temp = token;

    if (token == end) { return true; }
    if (expression())
    {
        ++token;
        return true;
    }

    token = temp;
    return true; // λ
}

// WHILE_STMT → while ( EXPRESSION ) STATEMENT
bool Parser::while_stmt()
{
    auto temp = token;

    if (token == end) { return false; }
    if (equals("while"))
    {
        ++token;
        if (token == end) { return false; }
        if (equals("("))
        {
            ++token;
            if (token == end) { return false; }
            if (expression())
            {
                ++token;
                if (token == end) { return false; }
                if (equals(")"))
                {
                    ++token;
                    if (token == end) { return false; }
                    if (statement())
                    {
                        ++token;
                        return true;
                    }
                }
            }
        }
    }

    token = temp;
    return false;
}

// IF_STMT → if ( EXPRESSION ) STATEMENT ELSEPART
bool Parser::if_stmt()
{
    auto temp = token;

    if (token == end) { return false; }
    if (equals("if"))
    {
        ++token;
        if (token == end) { return false; }
        if (equals("("))
        {
            ++token;
            if (token == end) { return false; }
            if (expression())
            {
                ++token;
                if (token == end) { return false; }
                if (equals(")"))
                {
                    ++token;
                    if (token == end) { return false; }
                    if (statement())
                    {
                        ++token;
                        if (token == end) { return false; }
                        if (elsepart())
                        {
                            ++token;
                            return true;
                        }
                    }
                }
            }
        }
    }

    token = temp;
    return false;
}

// ELSEPART → else STATEMENT | λ
bool Parser::elsepart()
{
    auto temp = token;

    if (token == end) { return true; }
    if (equals("else"))
    {
        ++token;
        if (token == end) { return false; }
        if (statement())
        {
            ++token;
            return true;
        }
        token = temp;
        return false;
    }

    token = temp;
    return true; // λ
}

// COMPOUND_STMT → { STMT_LIST }
bool Parser::compound_stmt()
{
    auto temp = token;

    if (token == end) { return false; }
    if (equals("{"))
    {
        ++token;
        if (token == end) { return false; }
        if (stmt_list())
        {
            ++token;
            if (token == end) { return false; }
            if (equals("}"))
            {
                ++token;
                return true;
            }
        }
    }

    token = temp;
    return false;
}

// STMT_LIST → STATEMENT STMT_LIST | λ
bool Parser::stmt_list()
{
    auto temp = token;

    if (token == end) { return true; }
    if (statement())
    {
        ++token;
        if (token == end) { return false; }
        if (stmt_list())
        {
            ++token;
            return true;
        }
    }

    token = temp;
    return true; // λ
}

// EXPRESSION → Identifier = EXPRESSION | RVALUE
bool Parser::expression()
{
    auto temp = token;

    if (token == end) { return false; }
    if (equals(Tokenizer::Identifier))
    {
        ++token;
        if (token == end) { return false; }
        if (equals("="))
        {
            ++token;
            if (token == end) { return false; }
            if (expression())
            {
                ++token;
                return true;
            }
        }
    }

    token = temp;

    if (rvalue())
    {
        ++token;
        return true;
    }

    token = temp;
    return false;
}

// RVALUE → MAG RVALUE2
bool Parser::rvalue()
{
    auto temp = token;

    if (token == end) { return false; }
    if (mag())
    {
        ++token;
        if (token == end) { return false; }
        if (rvalue2())
        {
            ++token;
            return true;
        }
    }

    token = temp;
    return false;
}

// RVALUE2 → COMPARE MAG RVALUE2 | λ
bool Parser::rvalue2()
{
    auto temp = token;

    if (token == end) { return true; }
    if (compare())
    {
        ++token;
        if (token == end) { return false; }
        if (mag())
        {
            ++token;
            if (token == end) { return false; }
            if (rvalue2())
            {
                ++token;
                return true;
            }
        }
    }

    token = temp;
    return true; // λ
}

// COMPARE → == | < | > | <= | >= | !=
bool Parser::compare()
{
    auto temp = token;

    if (token == end) { return false; }
    if (equals("=="))
    {
        ++token;
        return true;
    }
    else if (equals("<"))
    {
        ++token;
        return true;
    }
    else if (equals(">"))
    {
        ++token;
        return true;
    }
    else if (equals("<="))
    {
        ++token;
        return true;
    }
    else if (equals(">="))
    {
        ++token;
        return true;
    }
    else if (equals("!="))
    {
        ++token;
        return true;
    }

    token = temp;
    return false;
}

// MAG → TERM MAG2
bool Parser::mag()
{
    auto temp = token;

    if (token == end) { return false; }
    if (term())
    {
        ++token;
        if (token == end) { return false; }
        if (mag2())
        {
            ++token;
            return true;
        }
    }

    token = temp;
    return false;
}

// MAG2 → + TERM MAG2 | - TERM MAG2 | λ
bool Parser::mag2()
{
    auto temp = token;

    if (token == end) { return true; }
    if (equals("+") || equals("-"))
    {
        ++token;
        if (token == end) { return false; }
        if (term())
        {
            ++token;
            if (token == end) { return false; }
            if (mag2())
            {
                ++token;
                return true;
            }
        }
    }

    token = temp;
    return true; // λ
}

// TERM → FACTOR TERM2
bool Parser::term()
{
    auto temp = token;

    if (token == end) { return false; }
    if (factor())
    {
        ++token;
        if (token == end) { return false; }
        if (term2())
        {
            ++token;
            return true;
        }
    }

    token = temp;
    return false;
}

// TERM2 → * FACTOR TERM2 | / FACTOR TERM2 | λ
bool Parser::term2()
{
    auto temp = token;

    if (token == end) { return false; }
    if (equals("*") || equals("/"))
    {
        ++token;
        if (token == end) { return false; }
        if (factor())
        {
            ++token;
            if (token == end) { return false; }
            if (term2())
            {
                ++token;
                return true;
            }
        }
    }
    
    token = temp;
    return true; // λ
}

// FACTOR → ( EXPRESSION ) | - FACTOR | + FACTOR | Identifier | TYPE
bool Parser::factor()
{
    auto temp = token;

    if (token == end) { return false; }
    if (equals("("))
    {
        ++token;
        if (token == end) { return false; }
        if (expression())
        {
            ++token;
            if (token == end) { return false; }
            if (equals(")"))
            {
                ++token;
                return true;
            }
        }
    }

    token = temp;

    if (equals("-") || equals("+"))
    {
        ++token;
        if (token == end) { return false; }
        if (factor())
        {
            ++token;
            return true;
        }
    }

    token = temp;

    if (equals(Tokenizer::Identifier))
    {
        ++token;
        return true;
    }

    token = temp;

    if (type())
    {
        ++token;
        return true;
    }

    token = temp;
    return false;
}

//////////////////// SAMPLE CODE BELOW

// S → a S b | A
bool Parser::S()
{
    vector<Tokenizer::Token>::iterator temp, i;
    temp = token;

    if (token == end) // λ
    {
        return false;
    }
    else
    {
        if (token->value() == "a")
        {
            ++token;
            if (S())
            {
                if (token == end)
                {
                    return false;
                }
                else
                {
                    if (token->value() == "b")
                    {
                        ++token;
                        return true;
                    }
                    else
                    {
                        return false;
                    }

                }
            }

            else
            {
                return false;
            }
        }
        else if (A())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

// A → c | λ
bool Parser::A()
{
    vector<Tokenizer::Token>::iterator temp, i;
    temp = token;

    if (token == end)
    {
        return true; // λ
    }
    else
    {
        if (token->value() == "c")
        {
            ++token;
            return true;
        }
        else
        {
            return true; // λ
        }
    }
}