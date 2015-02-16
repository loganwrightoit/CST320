/***********************************************************
* Author:                   Logan Wright
* Date Created:             02/06/2015
* Last Modification Date:   02/08/2015
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
    doDebug = false;
}

Parser::~Parser()
{
}

void Parser::debug(char* msg)
{
    if (doDebug)
    {
        cout << "[PARSER]: " << msg << " (" << (token - 1)->value().c_str() << ")" << endl;
    }
}

void Parser::expected(char* expected)
{
    cout << "Syntax error at line " << token->line() << " position " << token->pos() << ", expected '" << expected << "'" << endl;
    exit(1);
}

void Parser::error(char* error)
{
    cout << "Syntax error at line " << token->line() << " position " << token->pos() << ", reason '" << error << "'" << endl;
    exit(1);
}

void Parser::addSymbol(Symbol::Use use)
{
    auto current = token - 1;

    // Symbol must be declared before use
    if (lastType == Symbol::UnknownType)
    {
        if (symbolTable.find(current->value()) == nullptr)
        {
            error("Symbol Undefined");
        }
    }
    else
    {
        // If symbol exists, skip
        auto result = symbolTable.find(current->value());
        if (result == nullptr)
        {
            symbolTable.add(Symbol(current->value().c_str(), lastType, use, ""));
        }
        else
        {
            error("Symbol Redefinition");
        }
    }

    lastType = Symbol::UnknownType;
}

bool Parser::equals(Tokenizer::TokenType type)
{
    if (type == token->type())
    {
        if ((token + 1) == end)
        {
            return true;
        }

        ++token;
        return true;
    }

    return false;
}

bool Parser::equals(char* input)
{
    if (input == token->value())
    {
        if ((token + 1) == end)
        {
            return true;
        }

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

    bool result = function();

    // Print structures
    auto iter = results.begin();
    while (iter != results.end())
    {
        char* result = *iter;
        std::cout << result << endl;
        ++iter;
    }

    // Print symbol table
    symbolTable.print();

    return result;
}

// FUNCTION → TYPE Identifier ( ARG_LIST ) COMPOUND_STMT
bool Parser::function()
{
    if (token == end) { return false; }
    auto temp = token;    
    
    Tokenizer::TokenType __type = token->type();
    if (type())
    {
        std::string __val = token->value();
        debug("FUNCTION -> TYPE");
        if (token == end) { return false; }
        if (equals(Tokenizer::Identifier))
        {
            addSymbol(Symbol::FunctionName);

            debug("FUNCTION -> TYPE Identifier");
            if (token == end) { return false; }
            if (equals("("))
            {
                debug("FUNCTION -> TYPE Identifier (");
                if (token == end) { return false; }
                if (arg_list())
                {
                    debug("FUNCTION -> TYPE Identifier ( ARG_LIST");
                    if (token == end) { return false; }
                    if (equals(")"))
                    {
                        debug("FUNCTION -> TYPE Identifier ( ARG_LIST )");
                        results.push_back("Function");
                        if (token == end) { return false; }
                        if (compound_stmt())
                        {
                            debug("FUNCTION -> TYPE Identifier ( ARG_LIST ) COMPOUND_STMT");
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
        debug("ARG_LIST -> lambda");
        return true;
    }
    auto temp = token;
    
    if (arg())
    {
        debug("ARG_LIST -> ARG");
        if (token == end) { return false; }
        if (arg_list2())
        {
            debug("ARG_LIST -> ARG ARG_LIST2");
            results.push_back("ArgumentList");
            return true;
        }
    }

    debug("ARG_LIST -> lambda");
    token = temp;
    return true;
}

// ARG_LIST2 → , ARG ARG_LIST2 | λ
bool Parser::arg_list2()
{
    if (token == end)
    {
        debug("ARG_LIST2 -> lambda");
        return true;
    }
    auto temp = token;
    
    if (equals(","))
    {
        debug("ARG_LIST2 -> ,");
        if (token == end) { return false; }
        if (arg())
        {
            debug("ARG_LIST2 -> , ARG");
            if (token == end) { return false; }
            if (arg_list2())
            {
                debug("ARG_LIST2 -> , ARG ARG_LIST2");
                return true;
            }
        }
    }

    debug("ARG_LIST2 -> lambda");
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
        debug("ARG -> TYPE");
        if (token == end) { return false; }
        if (equals(Tokenizer::Identifier))
        {
            addSymbol(Symbol::VariableName);

            debug("ARG -> TYPE Identifier");
            return true;
        }
        else
        {
            expected("Identifier");
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
        debug("DECLARATION -> TYPE");
        if (token == end) { return false; }
        if (ident_list())
        {
            debug("DECLARATION -> TYPE IDENT_LIST");
            if (token == end) { return false; }
            if (equals(";"))
            {
                debug("DECLARATION -> TYPE IDENT_LIST ;");
                return true;
            }
            else
            {
                expected(";");
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
        lastType = Symbol::Integer;
        debug("TYPE -> int");
        return true;
    }
    else if (equals("float"))
    {
        lastType = Symbol::Float;
        debug("TYPE -> float");
        return true;
    }
    else if (equals("bool"))
    {
        lastType = Symbol::Bool;
        debug("TYPE -> bool");
        return true;
    }
    else if (equals("string"))
    {
        lastType = Symbol::String;
        debug("TYPE -> string");
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
        addSymbol(Symbol::VariableName);

        debug("IDENT_LIST -> Identifier");
        if (token == end) { return false; }
        if (ident_list2())
        {
            debug("IDENT_LIST -> Identifier IDENT_LIST2");
            return true;
        }
    }

    token = temp;
    return false;
}

// IDENT_LIST2 → = EXPRESSION IDENT_LIST3 | IDENT_LIST3
bool Parser::ident_list2()
{
    if (token == end) { return false; }
    auto temp = token;

    if (equals("="))
    {
        debug("IDENT_LIST2 -> =");
        if (token == end) { return false; }
        if (expression())
        {
            debug("IDENT_LIST2 -> = EXPRESSION");
            if (token == end) { return false; }
            if (ident_list3())
            {
                debug("IDENT_LIST2 -> = EXPRESSION IDENT_LIST3");
                return true;
            }
        }
    }
    else if (ident_list3())
    {
        debug("IDENT_LIST2 -> IDENT_LIST3");
        return true;
    }

    token = temp;
    return false;
}

// IDENT_LIST3 → , IDENT_LIST | λ
bool Parser::ident_list3()
{
    if (token == end)
    {
        debug("IDENT_LIST3 -> lambda");
        return true;
    }
    auto temp = token;

    if (equals(","))
    {
        debug("IDENT_LIST3 -> ,");
        if (token == end) { return false; }
        if (ident_list())
        {
            debug("IDENT_LIST3 -> , IDENT_LIST");
            return true;
        }
    }

    debug("IDENT_LIST3 -> lambda");
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
        debug("STATEMENT -> FOR_STMT");
        return true;
    }
    else if (while_stmt())
    {
        debug("STATEMENT -> WHILE_STMT");
        return true;
    }
    else if (expression())
    {
        debug("STATEMENT -> EXPRESSION");
        if (token == end) { return false; }
        if (equals(";"))
        {
            debug("STATEMENT -> EXPRESSION ;");
            return true;
        }
        else
        {
            expected(";");
        }
    }
    else if (if_stmt())
    {
        debug("STATEMENT -> IF_STMT");
        return true;
    }
    else if (compound_stmt())
    {
        debug("STATEMENT -> COMPOUND_STMT");
        return true;
    }
    else if (declaration())
    {
        debug("STATEMENT -> DECLARATION");
        return true;
    }
    else if (equals(";"))
    {
        debug("STATEMENT -> ;");
        return true;
    }

    token = temp;
    return false;
}

// FOR_STMT → for ( DECLARATION OPT_EXPR ; OPT_EXPR ) STATEMENT
bool Parser::for_stmt()
{
    if (token == end) { return false; }
    auto temp = token;

    if (equals("for"))
    {
        debug("FOR_STMT -> for");
        if (token == end) { return false; }
        if (equals("("))
        {
            debug("FOR_STMT -> for (");
            if (token == end) { return false; }
            if (declaration())
            {
                debug("FOR_STMT -> for ( DECLARATION");
                if (opt_expr())
                {
                    debug("FOR_STMT -> for ( DECLARATION ; OPT_EXPR");
                    if (token == end) { return false; }
                    if (equals(";"))
                    {
                        debug("FOR_STMT -> for ( DECLARATION ; OPT_EXPR ;");
                        if (token == end) { return false; }
                        if (opt_expr())
                        {
                            debug("FOR_STMT -> for ( DECLARATION ; OPT_EXPR ; OPT_EXPR");
                            if (token == end) { return false; }
                            if (equals(")"))
                            {
                                debug("FOR_STMT -> for ( DECLARATION ; OPT_EXPR ; OPT_EXPR )");
                                if (token == end) { return false; }
                                if (statement())
                                {
                                    debug("FOR_STMT -> for ( DECLARATION ; OPT_EXPR ; OPT_EXPR ) STATEMENT");
                                    results.push_back("ForStatement");
                                    return true;
                                }
                            }
                            else
                            {
                                expected(")");
                            }
                        }
                    }
                    else
                    {
                        expected(";");
                    }
                }
            }
        }
        else
        {
            expected("(");
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
        debug("OPT_EXPR -> lambda");
        return true;
    }
    auto temp = token;

    if (expression())
    {
        debug("OPT_EXPR -> EXPRESSION");
        return true;
    }

    debug("OPT_EXPR -> lambda");
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
        debug("WHILE_STMT -> while");
        if (token == end) { return false; }
        if (equals("("))
        {
            debug("WHILE_STMT -> while (");
            if (token == end) { return false; }
            if (expression())
            {
                debug("WHILE_STMT -> while ( EXPRESSION");
                if (token == end) { return false; }
                if (equals(")"))
                {
                    debug("WHILE_STMT -> while ( EXPRESSION )");
                    if (token == end) { return false; }
                    if (statement())
                    {
                        debug("WHILE_STMT -> while ( EXPRESSION ) STATEMENT");
                        results.push_back("WhileStatement");
                        return true;
                    }
                }
                else
                {
                    expected(")");
                }
            }
        }
        else
        {
            expected("(");
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
        debug("IF_STMT -> if");
        if (token == end) { return false; }
        if (equals("("))
        {
            debug("IF_STMT -> if (");
            if (token == end) { return false; }
            if (expression())
            {
                debug("IF_STMT -> if ( EXPRESSION");
                if (token == end) { return false; }
                if (equals(")"))
                {
                    debug("IF_STMT -> if ( EXPRESSION )");
                    if (token == end) { return false; }
                    if (statement())
                    {
                        debug("IF_STMT -> if ( EXPRESSION ) STATEMENT");
                        if (token == end) { return false; }
                        if (elsepart())
                        {
                            debug("IF_STMT -> if ( EXPRESSION ) STATEMENT ELSEPART");
                            results.push_back("IfStatement");
                            return true;
                        }
                    }
                }
                else
                {
                    expected(")");
                }
            }
        }
        else
        {
            expected("(");
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
        debug("ELSEPART -> lambda");
        return true;
    }
    auto temp = token;

    if (equals("else"))
    {
        debug("ELSEPART -> else");
        if (token == end) { return false; }
        if (statement())
        {
            debug("ELSEPART -> else STATEMENT");
            results.push_back("ElseStatement");
            return true;
        }
    }

    debug("ELSEPART -> lambda");
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
        debug("COMPOUND_STMT -> {");
        if (token == end) { return false; }
        if (stmt_list())
        {
            debug("COMPOUND_STMT -> { STMT_LIST");
            if (token == end) { return false; }
            if (equals("}"))
            {
                debug("COMPOUND_STMT -> { STMT_LIST }");
                return true;
            }
            else
            {
                expected("}");
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
        debug("STMT_LIST -> lambda");
        return true;
    }
    auto temp = token;

    if (statement())
    {
        debug("STMT_LIST -> STATEMENT");
        if (token == end) { return false; }
        if (stmt_list())
        {
            debug("STMT_LIST -> STATEMENT STMT_LIST");
            return true;
        }
    }

    debug("STMT_LIST -> lambda");
    token = temp;
    return true; // λ
}

// EXPRESSION → Identifier = EXPRESSION | RVALUE
bool Parser::expression()
{
    if (token == end) { return false; }
    auto temp = token;

    if (equals(Tokenizer::Identifier))
    {
        addSymbol(Symbol::VariableName);

        debug("EXPRESSION -> Identifier");
        if (token == end) { return false; }
        if (equals("="))
        {
            debug("EXPRESSION -> Identifier =");
            if (token == end) { return false; }
            if (expression())
            {
                debug("EXPRESSION -> Identifier = EXPRESSION");
                return true;
            }
        }
    }

    // Reset token since I can't remove the unit production from RVALUE
    token = temp;

    if (rvalue())
    {
        debug("EXPRESSION -> RVALUE");
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
        debug("RVALUE -> MAG");
        if (token == end) { return false; }
        if (rvalue2())
        {
            debug("RVALUE -> MAG RVALUE2");
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
        debug("RVALUE2 -> lambda");
        return true;
    }
    auto temp = token;

    if (compare())
    {
        debug("RVALUE2 -> COMPARE");
        if (token == end) { return false; }
        if (mag())
        {
            debug("RVALUE2 -> COMPARE MAG");
            if (token == end) { return false; }
            if (rvalue2())
            {
                debug("RVALUE2 -> COMPARE MAG RVALUE2");
                return true;
            }
        }
    }

    debug("RVALUE2 -> lambda");
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
        debug("COMPARE -> ==");
        return true;
    }
    else if (equals("<"))
    {
        debug("COMPARE -> <");
        return true;
    }
    else if (equals(">"))
    {
        debug("COMPARE -> >");
        return true;
    }
    else if (equals("<="))
    {
        debug("COMPARE -> <=");
        return true;
    }
    else if (equals(">="))
    {
        debug("COMPARE -> >=");
        return true;
    }
    else if (equals("!="))
    {
        debug("COMPARE -> !=");
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
        debug("MAG -> TERM");
        if (token == end) { return false; }
        if (mag2())
        {
            debug("MAG -> TERM MAG2");
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
        debug("MAG2 -> lambda");
        return true;
    }
    auto temp = token;

    if (equals("+"))
    {
        debug("MAG2 -> +");
        if (token == end) { return false; }
        if (term())
        {
            debug("MAG2 -> + TERM");
            if (token == end) { return false; }
            if (mag2())
            {
                debug("MAG2 -> + TERM MAG2");
                return true;
            }
        }
    }
    else if (equals("-"))
    {
        debug("MAG2 -> -");
        if (token == end) { return false; }
        if (term())
        {
            debug("MAG2 -> - TERM");
            if (token == end) { return false; }
            if (mag2())
            {
                debug("MAG2 -> - TERM MAG2");
                return true;
            }
        }
    }

    debug("MAG2 -> lambda");
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
        debug("TERM -> FACTOR");
        if (token == end) { return false; }
        if (term2())
        {
            debug("TERM -> FACTOR TERM2");
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
        debug("TERM2 -> lambda");
        return true;
    }
    auto temp = token;

    if (equals("*"))
    {
        debug("TERM -> *");
        if (token == end) { return false; }
        if (factor())
        {
            debug("TERM -> * FACTOR");
            if (token == end) { return false; }
            if (term2())
            {
                debug("TERM -> * FACTOR TERM2");
                return true;
            }
        }
    }
    else if (equals("/"))
    {
        debug("TERM -> /");
        if (token == end) { return false; }
        if (factor())
        {
            debug("TERM -> / FACTOR");
            if (token == end) { return false; }
            if (term2())
            {
                debug("TERM -> / FACTOR TERM2");
                return true;
            }
        }
    }
    
    debug("TERM2 -> lambda");
    token = temp;
    return true; // λ
}

// FACTOR → ( EXPRESSION ) | - FACTOR | + FACTOR | Identifier | VALUE
bool Parser::factor()
{
    if (token == end) { return false; }
    auto temp = token;

    if (equals("("))
    {
        debug("FACTOR -> (");
        if (token == end) { return false; }
        if (expression())
        {
            debug("FACTOR -> ( EXPRESSION");
            if (token == end) { return false; }
            if (equals(")"))
            {
                debug("FACTOR -> ( EXPRESSION )");
                return true;
            }
        }
    }
    else if (equals("-"))
    {
        debug("FACTOR -> -");
        if (token == end) { return false; }
        if (factor())
        {
            debug("FACTOR -> - FACTOR");
            return true;
        }
    }
    else if (equals("+"))
    {
        debug("FACTOR -> +");
        if (token == end) { return false; }
        if (factor())
        {
            debug("FACTOR -> + FACTOR");
            return true;
        }
    }
    else if (equals(Tokenizer::Identifier))
    {
        debug("FACTOR -> Identifier");
        return true;
    }
    else if (value())
    {
        debug("FACTOR -> VALUE");
        return true;
    }

    token = temp;
    return false;
}

// VALUE → Integer | Float | String | Boolean
bool Parser::value()
{
    if (token == end) { return false; }
    auto temp = token;

    if (equals(Tokenizer::Integer))
    {
        debug("VALUE -> Integer");
        return true;
    }
    else if (equals(Tokenizer::Float))
    {
        debug("VALUE -> Float");
        return true;
    }
    else if (equals(Tokenizer::String))
    {
        debug("VALUE -> String");
        return true;

    }
    else if (equals(Tokenizer::Boolean))
    {
        debug("VALUE -> Boolean");
        return true;
    }

    token = temp;
    return false;
}