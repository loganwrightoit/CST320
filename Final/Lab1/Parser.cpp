/***********************************************************
* Author:                   Logan Wright
* Date Created:             02/06/2015
* Last Modification Date:   03/04/2015
* Lab Number:               CST 320 Final Project
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
#include <iomanip>
#include <fstream>

using namespace std;

Parser::Parser()
{
}

Parser::~Parser()
{
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

bool Parser::equals(int level, Tokenizer::TokenType type)
{
    if (type == token->type())
    {
        if ((token + 1) == end)
        {
            return true;
        }

        addBranch(level, "<Identifier>");
        addBranch(level + 1, token->value());

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

    bool result = function(0);

    // Print symbol table
    symbolTable.print();

    // Save parse tree to file
    std::ofstream outfile;
    outfile.open("parseTree.txt", std::fstream::out);
    if (outfile.is_open())
    {
        auto iter = output.begin();
        while (iter != output.end())
        {
            // Indent using first param
            for (int count = 0; count < iter->first; ++count)
            {
                outfile.write("\t", 1);
            }

            // Write argument
            outfile.write(iter->second.c_str(), iter->second.size());

            // End line
            outfile.write("\n", 1);
                
            ++iter;
        }
    }

    return result;
}

void Parser::addBranch(int level, std::string inStr)
{
    output.push_back(std::make_pair(level, inStr));
}

void Parser::popBranch()
{
    output.pop_back();
}

// FUNCTION → TYPE Identifier ( ARG_LIST ) COMPOUND_STMT
bool Parser::function(int level)
{
    if (token == end) { return false; }
    auto temp = token;

    addBranch(level++, "<FUNCTION>");
    if (type(level))
    {
        if (token == end) { return false; }
        if (equals(level, Tokenizer::Identifier))
        {
            addSymbol(Symbol::FunctionName);
            if (token == end) { return false; }
            if (equals("("))
            {
                if (token == end) { return false; }
                if (arg_list(level))
                {
                    if (token == end) { return false; }
                    if (equals(")"))
                    {
                        if (token == end) { return false; }
                        if (compound_stmt(level))
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }
    else
    {
        popBranch();
    }
    
    token = temp;
    return false;
}

// ARG_LIST → ARG ARG_LIST2 | λ
bool Parser::arg_list(int level)
{
    if (token == end)
    {
        return true;
    }
    auto temp = token;
    
    addBranch(level++, "<ARGUMENT LIST>");
    if (arg(level))
    {
        if (token == end) { return false; }
        if (arg_list2(level))
        {
            return true;
        }
    }
    else
    {
        popBranch();
    }

    token = temp;
    return true;
}

// ARG_LIST2 → , ARG ARG_LIST2 | λ
bool Parser::arg_list2(int level)
{
    if (token == end)
    {
        return true;
    }
    auto temp = token;
    
    addBranch(level++, "<ARGUMENT>");
    if (equals(","))
    {
        if (token == end) { return false; }
        if (arg(level))
        {
            if (token == end) { return false; }
            if (arg_list2(level))
            {
                return true;
            }
        }
    }

    addBranch(level, "lambda");
    token = temp;
    return true; // λ
}

// ARG → TYPE Identifier
bool Parser::arg(int level)
{
    if (token == end) { return false; }
    auto temp = token;

    addBranch(level++, "<ARGUMENT>");
    if (type(level))
    {
        if (token == end) { return false; }
        if (equals(level, Tokenizer::Identifier))
        {
            addSymbol(Symbol::VariableName);
            return true;
        }
        else
        {
            expected("Identifier");
        }        
    }
    else
    {
        popBranch();
    }

    token = temp;
    return false;
}

// DECLARATION → TYPE IDENT_LIST ;
bool Parser::declaration(int level)
{
    if (token == end) { return false; }
    auto temp = token;

    addBranch(level++, "<DECLARATION>");
    if (type(level))
    {
        if (token == end) { return false; }
        if (ident_list(level))
        {
            if (token == end) { return false; }
            if (equals(";"))
            {
                return true;
            }
            else
            {
                expected(";");
            }
        }
    }
    else
    {
        popBranch();
    }

    token = temp;
    return false;
}

// TYPE → int | float | bool | string
bool Parser::type(int level)
{
    if (token == end) { return false; }
    auto temp = token;

    addBranch(level++, "<TYPE>");
    if (equals("int"))
    {
        lastType = Symbol::Integer;
        addBranch(level, "Integer");
        return true;
    }
    else if (equals("float"))
    {
        lastType = Symbol::Float;
        addBranch(level, "Float");
        return true;
    }
    else if (equals("bool"))
    {
        lastType = Symbol::Bool;
        addBranch(level, "Boolean");
        return true;
    }
    else if (equals("string"))
    {
        lastType = Symbol::String;
        addBranch(level, "String");
        return true;
    }
    else
    {
        popBranch();
    }
    
    token = temp;
    return false;
}

// IDENT_LIST → Identifier IDENT_LIST2
bool Parser::ident_list(int level)
{
    if (token == end) { return false; }
    auto temp = token;

    addBranch(level++, "<IDENTIFIER LIST>");
    if (equals(level, Tokenizer::Identifier))
    {
        addSymbol(Symbol::VariableName);

        if (token == end) { return false; }
        if (ident_list2(level))
        {
            return true;
        }
    }
    else
    {
        popBranch();
    }

    token = temp;
    return false;
}

// IDENT_LIST2 → = EXPRESSION IDENT_LIST3 | IDENT_LIST3
bool Parser::ident_list2(int level)
{
    if (token == end) { return false; }
    auto temp = token;

    addBranch(level++, "<IDENT_LIST2>");
    if (equals("="))
    {
        if (token == end) { return false; }
        if (expression(level))
        {
            if (token == end) { return false; }
            if (ident_list3(level))
            {
                return true;
            }
        }
    }
    else if (ident_list3(level))
    {
        return true;
    }
    else
    {
        popBranch();
    }

    token = temp;
    return false;
}

// IDENT_LIST3 → , IDENT_LIST | λ
bool Parser::ident_list3(int level)
{
    if (token == end)
    {
        return true;
    }
    auto temp = token;

    addBranch(level++, "<IDENT_LIST3>");
    if (equals(","))
    {
        if (token == end) { return false; }
        if (ident_list(level))
        {
            return true;
        }
    }

    addBranch(level, "lambda");
    token = temp;
    return true; // λ
}

// STATEMENT → FOR_STMT | WHILE_STMT | EXPRESSION ; | IF_STMT | COMPOUND_STMT | DECLARATION | ;
bool Parser::statement(int level)
{
    if (token == end) { return false; }
    auto temp = token;

    addBranch(level++, "<STATEMENT>");
    if (for_stmt(level))
    {
        return true;
    }
    else if (while_stmt(level))
    {
        return true;
    }
    else if (expression(level))
    {
        if (token == end) { return false; }
        if (equals(";"))
        {
            return true;
        }
        else
        {
            expected(";");
        }
    }
    else if (if_stmt(level))
    {
        return true;
    }
    else if (compound_stmt(level))
    {
        return true;
    }
    else if (declaration(level))
    {
        return true;
    }
    else if (equals(";"))
    {
        return true;
    }
    else
    {
        popBranch();
    }

    token = temp;
    return false;
}

// FOR_STMT → for ( DECLARATION OPT_EXPR ; OPT_EXPR ) STATEMENT
bool Parser::for_stmt(int level)
{
    if (token == end) { return false; }
    auto temp = token;

    addBranch(level++, "<FOR STATEMENT>");
    if (equals("for"))
    {
        if (token == end) { return false; }
        if (equals("("))
        {
            if (token == end) { return false; }
            if (declaration(level))
            {
                if (opt_expr(level))
                {
                    if (token == end) { return false; }
                    if (equals(";"))
                    {
                        if (token == end) { return false; }
                        if (opt_expr(level))
                        {
                            if (token == end) { return false; }
                            if (equals(")"))
                            {
                                if (token == end) { return false; }
                                if (statement(level))
                                {
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
    else
    {
        popBranch();
    }

    token = temp;
    return false;
}

// OPT_EXPR → EXPRESSION | λ
bool Parser::opt_expr(int level)
{
    if (token == end)
    {
        return true;
    }
    auto temp = token;

    addBranch(level++, "<OPTIONAL EXPRESSION>");
    if (expression(level))
    {
        return true;
    }

    addBranch(level, "lambda");
    token = temp;
    return true; // λ
}

// WHILE_STMT → while ( EXPRESSION ) STATEMENT
bool Parser::while_stmt(int level)
{
    if (token == end) { return false; }
    auto temp = token;

    addBranch(level++, "<WHILE STATEMENT>");
    if (equals("while"))
    {
        if (token == end) { return false; }
        if (equals("("))
        {
            if (token == end) { return false; }
            if (expression(level))
            {
                if (token == end) { return false; }
                if (equals(")"))
                {
                    if (token == end) { return false; }
                    if (statement(level))
                    {
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
    else
    {
        popBranch();
    }

    token = temp;
    return false;
}

// IF_STMT → if ( EXPRESSION ) STATEMENT ELSEPART
bool Parser::if_stmt(int level)
{
    if (token == end) { return false; }
    auto temp = token;

    addBranch(level++, "<IF STATEMENT>");
    if (equals("if"))
    {
        if (token == end) { return false; }
        if (equals("("))
        {
            if (token == end) { return false; }
            if (expression(level))
            {
                if (token == end) { return false; }
                if (equals(")"))
                {
                    if (token == end) { return false; }
                    if (statement(level))
                    {
                        if (token == end) { return false; }
                        if (elsepart(level))
                        {
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
    else
    {
        popBranch();
    }

    token = temp;
    return false;
}

// ELSEPART → else STATEMENT | λ
bool Parser::elsepart(int level)
{
    if (token == end)
    {
        return true;
    }
    auto temp = token;

    addBranch(level++, "<ELSE STATEMENT>");
    if (equals("else"))
    {
        if (token == end) { return false; }
        if (statement(level))
        {
            return true;
        }
    }

    addBranch(level, "lambda");
    token = temp;
    return true; // λ
}

// COMPOUND_STMT → { STMT_LIST }
bool Parser::compound_stmt(int level)
{
    if (token == end) { return false; }
    auto temp = token;

    addBranch(level++, "<COMPOUND STATEMENT>");
    if (equals("{"))
    {
        if (token == end) { return false; }
        if (stmt_list(level))
        {
            if (token == end) { return false; }
            if (equals("}"))
            {
                return true;
            }
            else
            {
                expected("}");
            }
        }
    }
    else
    {
        popBranch();
    }

    token = temp;
    return false;
}

// STMT_LIST → STATEMENT STMT_LIST | λ
bool Parser::stmt_list(int level)
{
    if (token == end)
    {
        return true;
    }
    auto temp = token;

    addBranch(level++, "<STATEMENT LIST>");
    if (statement(level))
    {
        if (token == end) { return false; }
        if (stmt_list(level))
        {
            return true;
        }
    }
    else
    {
        popBranch();
    }

    addBranch(level, "lambda");
    token = temp;
    return true; // λ
}

// EXPRESSION → Identifier = EXPRESSION | RVALUE
bool Parser::expression(int level)
{
    if (token == end) { return false; }
    auto temp = token;

    addBranch(level++, "<EXPRESSION>");
    if (equals(level, Tokenizer::Identifier))
    {
        addSymbol(Symbol::VariableName);
        if (token == end) { return false; }
        if (equals("="))
        {
            if (token == end) { return false; }
            if (expression(level))
            {
                return true;
            }
        }
    }
    else
    {
        popBranch();
    }

    // Reset token since I can't remove the unit production from RVALUE
    token = temp;

    addBranch(level++, "<EXPRESSION>");
    if (rvalue(level))
    {
        return true;
    }
    else
    {
        popBranch();
    }

    token = temp;
    return false;
}

// RVALUE → MAG RVALUE2
bool Parser::rvalue(int level)
{
    if (token == end) { return false; }
    auto temp = token;

    addBranch(level++, "<RVALUE>");
    if (mag(level))
    {
        if (token == end) { return false; }
        if (rvalue2(level))
        {
            return true;
        }
    }
    else
    {
        popBranch();
    }

    token = temp;
    return false;
}

// RVALUE2 → COMPARE MAG RVALUE2 | λ
bool Parser::rvalue2(int level)
{
    if (token == end)
    {
        return true;
    }
    auto temp = token;

    addBranch(level++, "<RVALUE2>");
    if (compare(level))
    {
        if (token == end) { return false; }
        if (mag(level))
        {
            if (token == end) { return false; }
            if (rvalue2(level))
            {
                return true;
            }
        }
    }

    addBranch(level, "lambda");
    token = temp;
    return true; // λ
}

// COMPARE → == | < | > | <= | >= | !=
bool Parser::compare(int level)
{
    if (token == end) { return false; }
    auto temp = token;

    addBranch(level++, "<COMPARE>");
    if (equals("=="))
    {
        addBranch(level, "==");
        return true;
    }
    else if (equals("<"))
    {
        addBranch(level, "<");
        return true;
    }
    else if (equals(">"))
    {
        addBranch(level, ">");
        return true;
    }
    else if (equals("<="))
    {
        addBranch(level, "<=");
        return true;
    }
    else if (equals(">="))
    {
        addBranch(level, ">=");
        return true;
    }
    else if (equals("!="))
    {
        addBranch(level, "!=");
        return true;
    }
    else
    {
        popBranch();
    }

    token = temp;
    return false;
}

// MAG → TERM MAG2
bool Parser::mag(int level)
{
    if (token == end) { return false; }
    auto temp = token;

    addBranch(level++, "<MAG>");
    if (term(level))
    {
        if (token == end) { return false; }
        if (mag2(level))
        {
            return true;
        }
    }
    else
    {
        popBranch();
    }

    token = temp;
    return false;
}

// MAG2 → + TERM MAG2 | - TERM MAG2 | λ
bool Parser::mag2(int level)
{
    if (token == end)
    {
        return true;
    }
    auto temp = token;

    addBranch(level++, "<MAG2>");
    if (equals("+"))
    {
        if (token == end) { return false; }
        if (term(level))
        {
            if (token == end) { return false; }
            if (mag2(level))
            {
                return true;
            }
        }
    }
    else if (equals("-"))
    {
        if (token == end) { return false; }
        if (term(level))
        {
            if (token == end) { return false; }
            if (mag2(level))
            {
                return true;
            }
        }
    }

    addBranch(level, "lambda");
    token = temp;
    return true; // λ
}

// TERM → FACTOR TERM2
bool Parser::term(int level)
{
    if (token == end) { return false; }
    auto temp = token;

    addBranch(level++, "<TERM>");
    if (factor(level))
    {
        if (token == end) { return false; }
        if (term2(level))
        {
            return true;
        }
    }
    else
    {
        popBranch();
    }

    token = temp;
    return false;
}

// TERM2 → * FACTOR TERM2 | / FACTOR TERM2 | λ
bool Parser::term2(int level)
{
    if (token == end)
    {
        return true;
    }
    auto temp = token;

    addBranch(level++, "<TERM2>");
    if (equals("*"))
    {
        if (token == end) { return false; }
        if (factor(level))
        {
            if (token == end) { return false; }
            if (term2(level))
            {
                return true;
            }
        }
    }
    else if (equals("/"))
    {
        if (token == end) { return false; }
        if (factor(level))
        {
            if (token == end) { return false; }
            if (term2(level))
            {
                return true;
            }
        }
    }
    
    addBranch(level, "lambda");
    token = temp;
    return true; // λ
}

// FACTOR → ( EXPRESSION ) | - FACTOR | + FACTOR | Identifier | VALUE
bool Parser::factor(int level)
{
    if (token == end) { return false; }
    auto temp = token;

    addBranch(level++, "<FACTOR>");
    if (equals("("))
    {
        if (token == end) { return false; }
        if (expression(level))
        {
            if (token == end) { return false; }
            if (equals(")"))
            {
                return true;
            }
        }
    }
    else if (equals("-"))
    {
        if (token == end) { return false; }
        if (factor(level))
        {
            return true;
        }
    }
    else if (equals("+"))
    {
        if (token == end) { return false; }
        if (factor(level))
        {
            return true;
        }
    }
    else if (equals(level, Tokenizer::Identifier))
    {
        return true;
    }
    else if (value(level))
    {
        return true;
    }
    else
    {
        popBranch();
    }

    token = temp;
    return false;
}

// VALUE → Integer | Float | String | Boolean
bool Parser::value(int level)
{
    if (token == end) { return false; }
    auto temp = token;

    addBranch(level++, "<VALUE>");
    if (equals(level, Tokenizer::Integer))
    {
        return true;
    }
    else if (equals(level, Tokenizer::Float))
    {
        return true;
    }
    else if (equals(level, Tokenizer::String))
    {
        return true;

    }
    else if (equals(level, Tokenizer::Boolean))
    {
        return true;
    }
    else
    {
        popBranch();
    }

    token = temp;
    return false;
}