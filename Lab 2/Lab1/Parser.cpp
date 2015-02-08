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
    if (type == token->type())
    {
        ++token;
        return true;
    }

    return false;
}

bool Parser::equals(char* input)
{
    if (input == token->value())
    {
        ++token;
        return true;
    }

    return false;
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
    if (token == end) { return false; }
    auto temp = token;    
    
    if (type())
    {
        cout << "[DEBUG]: FUNCTION -> TYPE | " << token->value().c_str() << endl;
        if (token == end) { return false; }
        if (equals(Tokenizer::Identifier))
        {
            cout << "[DEBUG]: FUNCTION -> TYPE Identifier | " << token->value().c_str() << endl;
            if (token == end) { return false; }
            if (equals("("))
            {
                cout << "[DEBUG]: FUNCTION -> TYPE Identifier ( | " << token->value().c_str() << endl;
                if (token == end) { return false; }
                if (arg_list())
                {
                    cout << "[DEBUG]: FUNCTION -> TYPE Identifier ( ARG_LIST | " << token->value().c_str() << endl;
                    if (token == end) { return false; }
                    if (equals(")"))
                    {
                        cout << "[DEBUG]: FUNCTION -> TYPE Identifier ( ARG_LIST ) | " << token->value().c_str() << endl;
                        if (token == end) { return false; }
                        if (compound_stmt())
                        {
                            cout << "[DEBUG]: FUNCTION -> TYPE Identifier ( ARG_LIST ) COMPOUND_STMT | " << token->value().c_str() << endl;
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

// ARG_LIST → ARG ARG_LIST2 | λ
bool Parser::arg_list()
{
    if (token == end)
    {
        cout << "[DEBUG]: ARG_LIST -> lamdba" << endl;
        return true;
    }
    auto temp = token;
    
    if (arg())
    {
        cout << "[DEBUG]: ARG_LIST -> ARG | " << token->value().c_str() << endl;
        if (token == end) { return false; }
        if (arg_list2())
        {
            cout << "[DEBUG]: ARG_LIST -> ARG ARG_LIST2 | " << token->value().c_str() << endl;
            return true;
        }
    }

    cout << "[DEBUG]: ARG_LIST -> lamdba" << endl;
    token = temp;
    return true;
}

// ARG_LIST2 → , ARG ARG_LIST2 | λ
bool Parser::arg_list2()
{
    if (token == end)
    {
        cout << "[DEBUG]: ARG_LIST2 -> lamdba" << endl;
        return true;
    }
    auto temp = token;
    
    if (equals(","))
    {
        cout << "[DEBUG]: ARG_LIST2 -> , | " << token->value().c_str() << endl;
        if (++token == end) { return false; }
        if (arg())
        {
            cout << "[DEBUG]: ARG_LIST2 -> , ARG | " << token->value().c_str() << endl;
            if (token == end) { return false; }
            if (arg_list2())
            {
                cout << "[DEBUG]: ARG_LIST2 -> , ARG ARG_LIST2 | " << token->value().c_str() << endl;
                return true;
            }
        }
    }

    cout << "[DEBUG]: ARG_LIST2 -> lamdba" << endl;
    token = temp;
    return true; // λ
}

// ARG → TYPE Identifier
bool Parser::arg()
{
    if (token == end) { return false; }
    auto temp = token;

    if (type())
    {
        cout << "[DEBUG]: ARG -> TYPE | " << token->value().c_str() << endl;
        if (token == end) { return false; }
        if (equals(Tokenizer::Identifier))
        {
            cout << "[DEBUG]: ARG -> TYPE Identifier | " << token->value().c_str() << endl;
            return true;
        }
    }

    token = temp;
    return false;
}

// DECLARATION → TYPE IDENT_LIST ;
bool Parser::declaration()
{
    if (token == end) { return false; }
    auto temp = token;

    if (type())
    {
        cout << "[DEBUG]: DECLARATION -> TYPE | " << token->value().c_str() << endl;
        if (token == end) { return false; }
        if (ident_list())
        {
            cout << "[DEBUG]: DECLARATION -> TYPE IDENT_LIST | " << token->value().c_str() << endl;
            if (token == end) { return false; }
            if (equals(";"))
            {
                cout << "[DEBUG]: DECLARATION -> TYPE IDENT_LIST ; | " << token->value().c_str() << endl;
                return true;
            }
        }
    }

    token = temp;
    return false;
}

// TYPE → int | float | bool | string
bool Parser::type()
{
    if (token == end) { return false; }
    auto temp = token;

    if (equals("int"))
    {
        cout << "[DEBUG]: TYPE -> int | " << token->value().c_str() << endl;
        return true;
    }
    else if (equals("float"))
    {
        cout << "[DEBUG]: TYPE -> float | " << token->value().c_str() << endl;
        return true;
    }
    else if (equals("bool"))
    {
        cout << "[DEBUG]: TYPE -> bool | " << token->value().c_str() << endl;
        return true;
    }
    else if (equals("string"))
    {
        cout << "[DEBUG]: TYPE -> string | " << token->value().c_str() << endl;
        return true;
    }

    token = temp;
    return false;
}

// IDENT_LIST → Identifier IDENT_LIST2
bool Parser::ident_list()
{
    if (token == end) { return false; }
    auto temp = token;

    if (equals(Tokenizer::Identifier))
    {
        cout << "[DEBUG]: IDENT_LIST -> Identifier | " << token->value().c_str() << endl;
        if (token == end) { return false; }
        if (ident_list2())
        {
            cout << "[DEBUG]: IDENT_LIST -> Identifier IDENT_LIST2 | " << token->value().c_str() << endl;
            return true;
        }
    }

    token = temp;
    return false;
}

// IDENT_LIST2 → , IDENT_LIST | λ
bool Parser::ident_list2()
{
    if (token == end)
    {
        cout << "[DEBUG]: IDENT_LIST2 -> lambda" << endl;
        return true;
    }
    auto temp = token;

    if (equals(","))
    {
        cout << "[DEBUG]: IDENT_LIST2 -> , | " << token->value().c_str() << endl;
        if (token == end) { return false; }
        if (ident_list())
        {
            cout << "[DEBUG]: IDENT_LIST2 -> , IDENT_LIST | " << token->value().c_str() << endl;
            return true;
        }
    }

    cout << "[DEBUG]: IDENT_LIST2 -> lambda" << endl;
    token = temp;
    return true; // λ
}

// STATEMENT → FOR_STMT | WHILE_STMT | EXPRESSION ; | IF_STMT | COMPOUND_STMT | DECLARATION | ;
bool Parser::statement()
{
    if (token == end) { return false; }
    auto temp = token;

    if (for_stmt())
    {
        cout << "[DEBUG]: STATEMENT -> FOR_STMT | " << token->value().c_str() << endl;
        return true;
    }
    else if (while_stmt())
    {
        cout << "[DEBUG]: STATEMENT -> WHILE_STMT | " << token->value().c_str() << endl;
        return true;
    }
    else if (expression())
    {
        cout << "[DEBUG]: STATEMENT -> EXPRESSION | " << token->value().c_str() << endl;
        if (token == end) { return false; }
        if (equals(";"))
        {
            cout << "[DEBUG]: STATEMENT -> EXPRESSION ; | " << token->value().c_str() << endl;
            return true;
        }
    }
    else if (if_stmt())
    {
        cout << "[DEBUG]: STATEMENT -> IF_STMT | " << token->value().c_str() << endl;
        return true;
    }
    else if (compound_stmt())
    {
        cout << "[DEBUG]: STATEMENT -> COMPOUND_STMT | " << token->value().c_str() << endl;
        return true;
    }
    else if (declaration())
    {
        cout << "[DEBUG]: STATEMENT -> DECLARATION | " << token->value().c_str() << endl;
        return true;
    }
    else if (equals(";"))
    {
        cout << "[DEBUG]: STATEMENT -> ; | " << token->value().c_str() << endl;
        return true;
    }

    token = temp;
    return false;
}

// FOR_STMT → for ( EXPRESSION ; OPT_EXPR ; OPT_EXPR ) STATEMENT
bool Parser::for_stmt()
{
    if (token == end) { return false; }
    auto temp = token;

    if (equals("for"))
    {
        cout << "[DEBUG]: FOR_STMT -> for | " << token->value().c_str() << endl;
        if (token == end) { return false; }
        if (equals("("))
        {
            cout << "[DEBUG]: FOR_STMT -> for ( | " << token->value().c_str() << endl;
            if (token == end) { return false; }
            if (expression())
            {
                cout << "[DEBUG]: FOR_STMT -> for ( EXPRESSION | " << token->value().c_str() << endl;
                if (token == end) { return false; }
                if (equals(";"))
                {
                    cout << "[DEBUG]: FOR_STMT -> for ( EXPRESSION ; | " << token->value().c_str() << endl;
                    if (token == end) { return false; }
                    if (opt_expr())
                    {
                        cout << "[DEBUG]: FOR_STMT -> for ( EXPRESSION ; OPT_EXPR | " << token->value().c_str() << endl;
                        if (token == end) { return false; }
                        if (equals(";"))
                        {
                            cout << "[DEBUG]: FOR_STMT -> for ( EXPRESSION ; OPT_EXPR ; | " << token->value().c_str() << endl;
                            if (token == end) { return false; }
                            if (opt_expr())
                            {
                                cout << "[DEBUG]: FOR_STMT -> for ( EXPRESSION ; OPT_EXPR ; OPT_EXPR | " << token->value().c_str() << endl;
                                if (token == end) { return false; }
                                if (equals(")"))
                                {
                                    cout << "[DEBUG]: FOR_STMT -> for ( EXPRESSION ; OPT_EXPR ; OPT_EXPR ) | " << token->value().c_str() << endl;
                                    if (token == end) { return false; }
                                    if (statement())
                                    {
                                        cout << "[DEBUG]: FOR_STMT -> for ( EXPRESSION ; OPT_EXPR ; OPT_EXPR ) STATEMENT | " << token->value().c_str() << endl;
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
    if (token == end)
    {
        cout << "[DEBUG]: OPT_EXPR -> lambda" << endl;
        return true;
    }
    auto temp = token;

    if (expression())
    {
        cout << "[DEBUG]: OPT_EXPR -> EXPRESSION | " << token->value().c_str() << endl;
        return true;
    }

    cout << "[DEBUG]: OPT_EXPR -> lambda" << endl;
    token = temp;
    return true; // λ
}

// WHILE_STMT → while ( EXPRESSION ) STATEMENT
bool Parser::while_stmt()
{
    if (token == end) { return false; }
    auto temp = token;

    if (equals("while"))
    {
        cout << "[DEBUG]: WHILE_STMT -> while | " << token->value().c_str() << endl;
        if (token == end) { return false; }
        if (equals("("))
        {
            cout << "[DEBUG]: WHILE_STMT -> while ( | " << token->value().c_str() << endl;
            if (token == end) { return false; }
            if (expression())
            {
                cout << "[DEBUG]: WHILE_STMT -> while ( EXPRESSION | " << token->value().c_str() << endl;
                if (token == end) { return false; }
                if (equals(")"))
                {
                    cout << "[DEBUG]: WHILE_STMT -> while ( EXPRESSION ) | " << token->value().c_str() << endl;
                    if (token == end) { return false; }
                    if (statement())
                    {
                        cout << "[DEBUG]: WHILE_STMT -> while ( EXPRESSION ) STATEMENT | " << token->value().c_str() << endl;
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
    if (token == end) { return false; }
    auto temp = token;

    if (equals("if"))
    {
        cout << "[DEBUG]: IF_STMT -> if | " << token->value().c_str() << endl;
        if (token == end) { return false; }
        if (equals("("))
        {
            cout << "[DEBUG]: IF_STMT -> if ( | " << token->value().c_str() << endl;
            if (token == end) { return false; }
            if (expression())
            {
                cout << "[DEBUG]: IF_STMT -> if ( EXPRESSION | " << token->value().c_str() << endl;
                if (token == end) { return false; }
                if (equals(")"))
                {
                    cout << "[DEBUG]: IF_STMT -> if ( EXPRESSION ) | " << token->value().c_str() << endl;
                    if (token == end) { return false; }
                    if (statement())
                    {
                        cout << "[DEBUG]: IF_STMT -> if ( EXPRESSION ) STATEMENT | " << token->value().c_str() << endl;
                        if (token == end) { return false; }
                        if (elsepart())
                        {
                            cout << "[DEBUG]: IF_STMT -> if ( EXPRESSION ) STATEMENT ELSEPART | " << token->value().c_str() << endl;
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
    if (token == end)
    {
        cout << "[DEBUG]: ELSEPART -> lambda" << endl;
        return true;
    }
    auto temp = token;

    if (equals("else"))
    {
        cout << "[DEBUG]: ELSEPART -> else | " << token->value().c_str() << endl;
        if (token == end) { return false; }
        if (statement())
        {
            cout << "[DEBUG]: ELSEPART -> else STATEMENT | " << token->value().c_str() << endl;
            return true;
        }
    }

    cout << "[DEBUG]: ELSEPART -> lambda" << endl;
    token = temp;
    return true; // λ
}

// COMPOUND_STMT → { STMT_LIST }
bool Parser::compound_stmt()
{
    if (token == end) { return false; }
    auto temp = token;

    if (equals("{"))
    {
        cout << "[DEBUG]: COMPOUND_STMT -> { | " << token->value().c_str() << endl;
        if (token == end) { return false; }
        if (stmt_list())
        {
            cout << "[DEBUG]: COMPOUND_STMT -> { STMT_LIST | " << token->value().c_str() << endl;
            if (token == end) { return false; }
            if (equals("}"))
            {
                cout << "[DEBUG]: COMPOUND_STMT -> { STMT_LIST } | " << token->value().c_str() << endl;
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
    if (token == end)
    {
        cout << "[DEBUG]: STMT_LIST -> lambda" << endl;
        return true;
    }
    auto temp = token;

    if (statement())
    {
        cout << "[DEBUG]: STMT_LIST -> STATEMENT | " << token->value().c_str() << endl;
        if (token == end) { return false; }
        if (stmt_list())
        {
            cout << "[DEBUG]: STMT_LIST -> STATEMENT STMT_LIST | " << token->value().c_str() << endl;
            return true;
        }
    }

    cout << "[DEBUG]: STMT_LIST -> lambda" << endl;
    token = temp;
    return true; // λ
}

// EXPRESSION → Identifier = EXPRESSION | RVALUE | true | 1
bool Parser::expression()
{
    if (token == end) { return false; }
    auto temp = token;

    if (equals(Tokenizer::Identifier))
    {
        cout << "[DEBUG]: EXPRESSION -> Idenfitier | " << token->value().c_str() << endl;
        if (token == end) { return false; }
        if (equals("="))
        {
            cout << "[DEBUG]: EXPRESSION -> Idenfitier = | " << token->value().c_str() << endl;
            if (token == end) { return false; }
            if (expression())
            {
                cout << "[DEBUG]: EXPRESSION -> Idenfitier = EXPRESSION | " << token->value().c_str() << endl;
                return true;
            }
        }
    }
    else if (rvalue())
    {
        cout << "[DEBUG]: EXPRESSION -> RVALUE | " << token->value().c_str() << endl;
        return true;
    }
    else if (equals("true"))
    {
        cout << "[DEBUG]: EXPRESSION -> true | " << token->value().c_str() << endl;
        return true;
    }
    else if (equals("1"))
    {
        cout << "[DEBUG]: EXPRESSION -> 1 | " << token->value().c_str() << endl;
        return true;
    }

    token = temp;
    return false;
}

// RVALUE → MAG RVALUE2
bool Parser::rvalue()
{
    if (token == end) { return false; }
    auto temp = token;

    if (mag())
    {
        cout << "[DEBUG]: RVALUE -> MAG | " << token->value().c_str() << endl;
        if (token == end) { return false; }
        if (rvalue2())
        {
            cout << "[DEBUG]: RVALUE -> MAG RVALUE2 | " << token->value().c_str() << endl;
            return true;
        }
    }

    token = temp;
    return false;
}

// RVALUE2 → COMPARE MAG RVALUE2 | λ
bool Parser::rvalue2()
{
    if (token == end)
    {
        cout << "[DEBUG]: RVALUE2 -> lambda" << endl;
        return true;
    }
    auto temp = token;

    if (compare())
    {
        cout << "[DEBUG]: RVALUE2 -> COMPARE | " << token->value().c_str() << endl;
        if (token == end) { return false; }
        if (mag())
        {
            cout << "[DEBUG]: RVALUE2 -> COMPARE MAG | " << token->value().c_str() << endl;
            if (token == end) { return false; }
            if (rvalue2())
            {
                cout << "[DEBUG]: RVALUE2 -> COMPARE MAG RVALUE2 | " << token->value().c_str() << endl;
                return true;
            }
        }
    }

    cout << "[DEBUG]: RVALUE2 -> lambda" << endl;
    token = temp;
    return true; // λ
}

// COMPARE → == | < | > | <= | >= | !=
bool Parser::compare()
{
    if (token == end) { return false; }
    auto temp = token;

    if (equals("=="))
    {
        cout << "[DEBUG]: COMPARE -> == | " << token->value().c_str() << endl;
        return true;
    }
    else if (equals("<"))
    {
        cout << "[DEBUG]: COMPARE -> < | " << token->value().c_str() << endl;
        return true;
    }
    else if (equals(">"))
    {
        cout << "[DEBUG]: COMPARE -> > | " << token->value().c_str() << endl;
        return true;
    }
    else if (equals("<="))
    {
        cout << "[DEBUG]: COMPARE -> <= | " << token->value().c_str() << endl;
        return true;
    }
    else if (equals(">="))
    {
        cout << "[DEBUG]: COMPARE -> >= | " << token->value().c_str() << endl;
        return true;
    }
    else if (equals("!="))
    {
        cout << "[DEBUG]: COMPARE -> != | " << token->value().c_str() << endl;
        return true;
    }

    token = temp;
    return false;
}

// MAG → TERM MAG2
bool Parser::mag()
{
    if (token == end) { return false; }
    auto temp = token;

    if (term())
    {
        cout << "[DEBUG]: MAG -> TERM | " << token->value().c_str() << endl;
        if (token == end) { return false; }
        if (mag2())
        {
            cout << "[DEBUG]: MAG -> TERM MAG2 | " << token->value().c_str() << endl;
            return true;
        }
    }

    token = temp;
    return false;
}

// MAG2 → + TERM MAG2 | - TERM MAG2 | λ
bool Parser::mag2()
{
    if (token == end)
    {
        cout << "[DEBUG]: MAG2 -> lambda" << endl;
        return true;
    }
    auto temp = token;

    if (equals("+"))
    {
        cout << "[DEBUG]: MAG2 -> + | " << token->value().c_str() << endl;
        if (token == end) { return false; }
        if (term())
        {
            cout << "[DEBUG]: MAG2 -> + TERM | " << token->value().c_str() << endl;
            if (token == end) { return false; }
            if (mag2())
            {
                cout << "[DEBUG]: MAG2 -> + TERM MAG2 | " << token->value().c_str() << endl;
                return true;
            }
        }
    }
    else if (equals("-"))
    {
        cout << "[DEBUG]: MAG2 -> - | " << token->value().c_str() << endl;
        if (token == end) { return false; }
        if (term())
        {
            cout << "[DEBUG]: MAG2 -> - TERM | " << token->value().c_str() << endl;
            if (token == end) { return false; }
            if (mag2())
            {
                cout << "[DEBUG]: MAG2 -> - TERM MAG2 | " << token->value().c_str() << endl;
                return true;
            }
        }
    }

    cout << "[DEBUG]: MAG2 -> lambda" << endl;
    token = temp;
    return true; // λ
}

// TERM → FACTOR TERM2
bool Parser::term()
{
    if (token == end) { return false; }
    auto temp = token;

    if (factor())
    {
        cout << "[DEBUG]: TERM -> FACTOR | " << token->value().c_str() << endl;
        if (token == end) { return false; }
        if (term2())
        {
            cout << "[DEBUG]: TERM -> FACTOR TERM2 | " << token->value().c_str() << endl;
            return true;
        }
    }

    token = temp;
    return false;
}

// TERM2 → * FACTOR TERM2 | / FACTOR TERM2 | λ
bool Parser::term2()
{
    if (token == end)
    {
        cout << "[DEBUG]: TERM2 -> lambda" << endl;
        return true;
    }
    auto temp = token;

    if (equals("*"))
    {
        cout << "[DEBUG]: TERM -> * | " << token->value().c_str() << endl;
        if (token == end) { return false; }
        if (factor())
        {
            cout << "[DEBUG]: TERM -> * FACTOR | " << token->value().c_str() << endl;
            if (token == end) { return false; }
            if (term2())
            {
                cout << "[DEBUG]: TERM -> * FACTOR TERM2 | " << token->value().c_str() << endl;
                return true;
            }
        }
    }
    else if (equals("/"))
    {
        cout << "[DEBUG]: TERM -> / | " << token->value().c_str() << endl;
        if (token == end) { return false; }
        if (factor())
        {
            cout << "[DEBUG]: TERM -> / FACTOR | " << token->value().c_str() << endl;
            if (token == end) { return false; }
            if (term2())
            {
                cout << "[DEBUG]: TERM -> / FACTOR TERM2 | " << token->value().c_str() << endl;
                return true;
            }
        }
    }
    
    cout << "[DEBUG]: TERM2 -> lambda" << endl;
    token = temp;
    return true; // λ
}

// FACTOR → ( EXPRESSION ) | - FACTOR | + FACTOR | Identifier | TYPE
bool Parser::factor()
{
    if (token == end) { return false; }
    auto temp = token;

    if (equals("("))
    {
        cout << "[DEBUG]: FACTOR -> ( | " << token->value().c_str() << endl;
        if (token == end) { return false; }
        if (expression())
        {
            cout << "[DEBUG]: FACTOR -> ( EXPRESSION | " << token->value().c_str() << endl;
            if (token == end) { return false; }
            if (equals(")"))
            {
                cout << "[DEBUG]: FACTOR -> ( EXPRESSION ) | " << token->value().c_str() << endl;
                return true;
            }
        }
    }
    else if (equals("-"))
    {
        cout << "[DEBUG]: FACTOR -> - | " << token->value().c_str() << endl;
        if (token == end) { return false; }
        if (factor())
        {
            cout << "[DEBUG]: FACTOR -> - FACTOR | " << token->value().c_str() << endl;
            return true;
        }
    }
    else if (equals("+"))
    {
        cout << "[DEBUG]: FACTOR -> + | " << token->value().c_str() << endl;
        if (token == end) { return false; }
        if (factor())
        {
            cout << "[DEBUG]: FACTOR -> + FACTOR | " << token->value().c_str() << endl;
            return true;
        }
    }
    else if (equals(Tokenizer::Identifier))
    {
        cout << "[DEBUG]: FACTOR -> Identifier | " << token->value().c_str() << endl;
        return true;
    }
    else if (type())
    {
        cout << "[DEBUG]: FACTOR -> TYPE | " << token->value().c_str() << endl;
        return true;
    }

    token = temp;
    return false;
}