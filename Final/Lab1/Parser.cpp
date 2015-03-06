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
* _output:
*   None.
************************************************************/

#include "Parser.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>

#define NOMINMAX

using namespace std;

Parser::Parser()
{
    _definition = false;
    _scope = 0;
    int numCompoundStmts = 0;
    int numStmts = 0;
    int numIfStmts = 0;
    int numWhileStmts = 0;
    int numInOutStmts = 0;
}

Parser::~Parser()
{
}

void Parser::expected(char* expected)
{
    cout << "Syntax error at line " << _token->line() << " position " << _token->pos() << ", expected '" << expected << "'" << endl;
    exit(1);
}

void Parser::error(char* error)
{
    cout << "Syntax error at line " << _token->line() << " position " << _token->pos() << ", reason '" << error << "'" << endl;
    exit(1);
}

void Parser::addSymbol(Symbol::Use use)
{
    auto current = _token - 1;

    // Symbol must be declared before use
    if (!_definition && _lastType == Symbol::UnknownType)
    {
        if (!_symbolTable.contains(current->value()))
        {
            error("Symbol Undefined");
        }
    }
    else
    {
        // Check scope of all symbols with same name in table
        // If current scope is lower than symbol scope, it can't be used
        auto result = _symbolTable.find(current->value());
        if (result != NULL) // Grabs vector all symbols in table with name (differentiated by scope)
        {
            int minScope = 0;
            auto iter = result->begin();
            while (iter != result->end()) // Check each symbol for scope that will allow it's use
            {
                // Determine lowest scope available for symbol use
                minScope = std::min(_scope, iter->getScope());
                ++iter;
            }

            // Check if lowest scope is less than or equal to current scope
            // If so, it can be used
            if (minScope <= _scope)
            {
                error("Symbol Undefined (Out-of-Scope)");
            }
        }

        if (!_symbolTable.add(Symbol(current->value().c_str(), _lastType, use, "", _scope)))
        {
            error("Symbol Redefinition");
        }
    }

    _lastType = Symbol::UnknownType;
}

bool Parser::equals(Tokenizer::TokenType type)
{
    if (type == _token->type())
    {
        if ((_token + 1) == _end)
        {
            return true;
        }

        ++_token;
        return true;
    }

    return false;
}

bool Parser::equals(char* input)
{
    if (input == _token->value())
    {
        if ((_token + 1) == _end)
        {
            return true;
        }

        // Adjust _scope for bracketed code blocks
        if (input == "{")
        {
            ++_scope;
        }
        else if (input == "}")
        {
            --_scope;
        }

        ++_token;
        return true;
    }

    return false;
}

bool Parser::parse(std::vector<Tokenizer::Token> _tokens)
{
    _token = _tokens.begin();
    _end = _tokens.end();

    // Assume int main() { COMPOUND_STMT } comes first
    if (_token == _end) { return false; }

    bool result = function(0);

    // Print symbol table
    _symbolTable.print();

    // Save parse tree to file
    std::ofstream outfile;
    outfile.open("parseTree.txt", std::fstream::out);
    if (outfile.is_open())
    {
        auto iter = _output.begin();
        while (iter != _output.end())
        {
            // Indent using first param
            for (int count = 0; count < iter->first; ++count)
            {
                outfile.write("\t", 1);
            }

            // Write argument
            outfile.write(iter->second.c_str(), iter->second.size());

            // _end line
            outfile.write("\n", 1);
                
            ++iter;
        }
    }

    // Print system metrics to console
    printSystemMetrics();

    return result;
}

void Parser::printSystemMetrics()
{
    cout << endl << setw(30) << "Program Analysis:" << endl;
    cout << setw(35) << setfill('-') << '-' << setfill(' ') << endl;

    // Grab values from symbol table
    int numSymbols = 0;
    int totalIdentLen = 0;
    int maxScopeLevel = 0;
    int numFunctions = 0;
    auto table = _symbolTable.getTable();
    auto iter = table.begin();
    while (iter != table.end())
    {
        numSymbols += iter->second.size();

        // Add length of identifiers for next step
        auto iter2 = iter->second.begin();
        while (iter2 != iter->second.end())
        {
            maxScopeLevel = max(maxScopeLevel, iter2->getScope());
            totalIdentLen += iter2->getName().size();

            if (iter2->getUse() == Symbol::Use::FunctionName)
            {
                numFunctions++;
            }

            ++iter2;
        }

        ++iter;
    }

    // Total number of symbols in the program
    cout << setw(30) << "Number of symbols" << numSymbols << endl;

    // Average length of identifiers in the program
    cout << setw(30) << "Avg Length of Identifiers" << totalIdentLen / numSymbols << endl;

    // Average number of statements for compound statements
    cout << setw(30) << "Avg stmts in compound stmts" << (numStmts / numCompoundStmts) << endl;

    // Maximum number of scope levels for identifiers
    cout << setw(30) << "Maximum scope level" << maxScopeLevel << endl;

    // Number of if statements in the program
    cout << setw(30) << "Number of IF stmts" << numIfStmts << endl;

    // Number of while loops in the program
    cout << setw(30) << "Number of WHILE stmts" << numWhileStmts << endl;

    // Number of input/output statements in the program
    cout << setw(30) << "Number of Input/Output stmts" << numInOutStmts << endl;

    // Number of functions in the program
    cout << setw(30) << "Number of functions" << numFunctions << endl;
}

void Parser::addBranch(int level, std::string inStr)
{
    _output.push_back(std::make_pair(level, inStr));
}

void Parser::popBranch()
{
    _output.pop_back();
}

// FUNCTION → TYPE Identifier ( ARG_LIST ) COMPOUND_STMT
bool Parser::function(int level)
{
    if (_token == _end) { return false; }
    auto temp = _token;

    addBranch(level++, "<FUNCTION>");
    if (type(level))
    {
        if (_token == _end) { return false; }
        if (equals(Tokenizer::Identifier))
        {
            addBranch(level, "<Identifier>");
            addBranch(level + 1, (_token - 1)->value());
            addSymbol(Symbol::FunctionName);
            if (_token == _end) { return false; }
            if (equals("("))
            {
                if (_token == _end) { return false; }
                if (arg_list(level))
                {
                    if (_token == _end) { return false; }
                    if (equals(")"))
                    {
                        if (_token == _end) { return false; }
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
    
    _token = temp;
    return false;
}

// ARG_LIST → ARG ARG_LIST2 | λ
bool Parser::arg_list(int level)
{
    if (_token == _end)
    {
        return true;
    }
    auto temp = _token;
    
    addBranch(level++, "<ARGUMENT LIST>");
    if (arg(level))
    {
        if (_token == _end) { return false; }
        if (arg_list2(level))
        {
            return true;
        }
    }
    else
    {
        popBranch();
    }

    _token = temp;
    return true;
}

// ARG_LIST2 → , ARG ARG_LIST2 | λ
bool Parser::arg_list2(int level)
{
    if (_token == _end)
    {
        return true;
    }
    auto temp = _token;
    
    if (equals(","))
    {
        if (_token == _end) { return false; }
        if (arg(level))
        {
            if (_token == _end) { return false; }
            if (arg_list2(level))
            {
                return true;
            }
        }
    }

    _token = temp;
    return true; // λ
}

// ARG → TYPE Identifier
bool Parser::arg(int level)
{
    if (_token == _end) { return false; }
    auto temp = _token;

    addBranch(level++, "<ARGUMENT>");
    if (type(level))
    {
        if (_token == _end) { return false; }
        if (equals(Tokenizer::Identifier))
        {
            addBranch(level, "<Identifier>");
            addBranch(level + 1, (_token - 1)->value());
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

    _token = temp;
    return false;
}

// DECLARATION → TYPE IDENT_LIST ;
bool Parser::declaration(int level)
{
    if (_token == _end) { return false; }
    auto temp = _token;

    addBranch(level++, "<DECLARATION>");
    if (type(level))
    {
        if (_token == _end) { return false; }
        if (ident_list(level))
        {
            if (_token == _end) { return false; }
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

    _token = temp;
    return false;
}

// TYPE → int | float | bool | string
bool Parser::type(int level)
{
    if (_token == _end) { return false; }
    auto temp = _token;

    addBranch(level++, "<TYPE>");
    if (equals("int"))
    {
        _lastType = Symbol::Integer;
        addBranch(level, "Integer");
        return true;
    }
    else if (equals("float"))
    {
        _lastType = Symbol::Float;
        addBranch(level, "Float");
        return true;
    }
    else if (equals("bool"))
    {
        _lastType = Symbol::Bool;
        addBranch(level, "Boolean");
        return true;
    }
    else if (equals("string"))
    {
        _lastType = Symbol::String;
        addBranch(level, "String");
        return true;
    }
    else
    {
        popBranch();
    }
    
    _token = temp;
    return false;
}

// IDENT_LIST → Identifier IDENT_LIST2
bool Parser::ident_list(int level)
{
    if (_token == _end) { return false; }
    auto temp = _token;

    // Change state for defining identifiers
    _definition = true;

    addBranch(level++, "<IDENTIFIER LIST>");
    if (equals(Tokenizer::Identifier))
    {

        // Add identifier and value to symbol table
        addBranch(level, "<Identifier>");
        addBranch(level + 1, (_token - 1)->value());
        addSymbol(Symbol::VariableName);

        if (_token == _end) { return false; }
        if (ident_list2(level))
        {
            return true;
        }
    }
    else
    {
        popBranch();
    }

    _token = temp;
    return false;
}

// IDENT_LIST2 → ( EXPRESSION ) IDENT_LIST3 | = EXPRESSION IDENT_LIST3 | IDENT_LIST3
bool Parser::ident_list2(int level)
{
    if (_token == _end) { return false; }
    auto temp = _token;

    if (equals("("))
    {
        if (_token == _end) { return false; }
        if (expression(level))
        {
            if (_token == _end) { return false; }
            if (equals(")"))
            {
                if (_token == _end) { return false; }
                if (ident_list3(level))
                {
                    return true;
                }
            }
        }
    }
    else if (equals("="))
    {
        if (_token == _end) { return false; }
        if (expression(level))
        {
            if (_token == _end) { return false; }
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

    _token = temp;
    return false;
}

// IDENT_LIST3 → , IDENT_LIST | λ
bool Parser::ident_list3(int level)
{
    if (_token == _end)
    {
        return true;
    }
    auto temp = _token;

    if (equals(","))
    {
        if (_token == _end) { return false; }
        if (ident_list(level))
        {
            return true;
        }
    }

    _definition = false;
    _token = temp;
    return true; // λ
}

// STATEMENT → FOR_STMT | WHILE_STMT | EXPRESSION ; | STREAM_STMT ; | IF_STMT | COMPOUND_STMT | DECLARATION | ;
bool Parser::statement(int level)
{
    if (_token == _end) { return false; }
    auto temp = _token;

    addBranch(level++, "<STATEMENT>");
    if (for_stmt(level))
    {
        return true;
    }
    else if (while_stmt(level))
    {
        ++numWhileStmts;
        return true;
    }
    else if (expression(level))
    {
        if (_token == _end) { return false; }
        if (equals(";"))
        {
            return true;
        }
        else
        {
            expected(";");
        }
    }
    else if (stream_stmt(level))
    {
        if (_token == _end) { return false; }
        if (equals(";"))
        {
            ++numInOutStmts;
            return true;
        }
        else
        {
            expected(";");
        }
    }
    else if (if_stmt(level))
    {
        ++numIfStmts;
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

    _token = temp;
    return false;
}

// FOR_STMT → for ( DECLARATION OPT_EXPR ; OPT_EXPR ) STATEMENT
bool Parser::for_stmt(int level)
{
    if (_token == _end) { return false; }
    auto temp = _token;

    addBranch(level++, "<FOR STATEMENT>");
    if (equals("for"))
    {
        if (_token == _end) { return false; }
        if (equals("("))
        {
            if (_token == _end) { return false; }
            if (declaration(level))
            {
                if (opt_expr(level))
                {
                    if (_token == _end) { return false; }
                    if (equals(";"))
                    {
                        if (_token == _end) { return false; }
                        if (opt_expr(level))
                        {
                            if (_token == _end) { return false; }
                            if (equals(")"))
                            {
                                if (_token == _end) { return false; }
                                if (statement(level))
                                {
                                    ++numStmts;
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

    _token = temp;
    return false;
}

// OPT_EXPR → EXPRESSION | λ
bool Parser::opt_expr(int level)
{
    if (_token == _end)
    {
        return true;
    }
    auto temp = _token;

    addBranch(level++, "<OPTIONAL EXPRESSION>");
    if (expression(level))
    {
        return true;
    }
    else
    {
        popBranch();
    }

    _token = temp;
    return true; // λ
}

// WHILE_STMT → while ( EXPRESSION ) STATEMENT
bool Parser::while_stmt(int level)
{
    if (_token == _end) { return false; }
    auto temp = _token;

    addBranch(level++, "<WHILE STATEMENT>");
    if (equals("while"))
    {
        if (_token == _end) { return false; }
        if (equals("("))
        {
            if (_token == _end) { return false; }
            if (expression(level))
            {
                if (_token == _end) { return false; }
                if (equals(")"))
                {
                    if (_token == _end) { return false; }
                    if (statement(level))
                    {
                        ++numStmts;
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

    _token = temp;
    return false;
}

// IF_STMT → if ( EXPRESSION ) STATEMENT ELSEPART
bool Parser::if_stmt(int level)
{
    if (_token == _end) { return false; }
    auto temp = _token;

    addBranch(level++, "<IF STATEMENT>");
    if (equals("if"))
    {
        if (_token == _end) { return false; }
        if (equals("("))
        {
            if (_token == _end) { return false; }
            if (expression(level))
            {
                if (_token == _end) { return false; }
                if (equals(")"))
                {
                    if (_token == _end) { return false; }
                    if (statement(level))
                    {
                        ++numStmts;
                        if (_token == _end) { return false; }
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

    _token = temp;
    return false;
}

// ELSEPART → else STATEMENT | λ
bool Parser::elsepart(int level)
{
    if (_token == _end)
    {
        return true;
    }
    auto temp = _token;

    addBranch(level++, "<ELSE STATEMENT>");
    if (equals("else"))
    {
        if (_token == _end) { return false; }
        if (statement(level))
        {
            ++numStmts;
            return true;
        }
    }
    else
    {
        popBranch();
    }

    _token = temp;
    return true; // λ
}

// COMPOUND_STMT → { STMT_LIST }
bool Parser::compound_stmt(int level)
{
    if (_token == _end) { return false; }
    auto temp = _token;

    addBranch(level++, "<COMPOUND STATEMENT>");
    if (equals("{"))
    {
        if (_token == _end) { return false; }
        if (stmt_list(level))
        {
            if (_token == _end) { return false; }
            if (equals("}"))
            {
                ++numCompoundStmts;
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

    _token = temp;
    return false;
}

// STMT_LIST → STATEMENT STMT_LIST | λ
bool Parser::stmt_list(int level)
{
    if (_token == _end)
    {
        return true;
    }
    auto temp = _token;

    addBranch(level++, "<STATEMENT LIST>");
    if (statement(level))
    {
        ++numStmts;
        if (_token == _end) { return false; }
        if (stmt_list(level - 1))
        {
            return true;
        }
    }
    else
    {
        popBranch();
    }

    _token = temp;
    return true; // λ
}

// EXPRESSION → Identifier = EXPRESSION | RVALUE
bool Parser::expression(int level)
{
    if (_token == _end) { return false; }
    auto temp = _token;

    addBranch(level++, "<EXPRESSION>");
    if (equals(Tokenizer::Identifier))
    {
        addBranch(level, "<Identifier>");
        addBranch(level + 1, (_token - 1)->value());
        addSymbol(Symbol::VariableName);
        if (_token == _end) { return false; }
        if (equals("="))
        {
            if (_token == _end) { return false; }
            if (expression(level))
            {
                return true;
            }
        }
    }
    else
    {
        level--;
        popBranch();
    }

    // Reset _token since I can't remove the unit production from RVALUE
    _token = temp;

    addBranch(level++, "<EXPRESSION>");
    if (rvalue(level))
    {
        return true;
    }
    else
    {
        popBranch();
    }

    _token = temp;
    return false;
}

// STREAM_STMT → cout << EXPRESSION OSTREAM_LIST | cin >> Identifier
bool Parser::stream_stmt(int level)
{
    if (_token == _end) { return false; }
    auto temp = _token;

    addBranch(level++, "<STREAM STATEMENT>");
    if (equals("cout"))
    {
        if (_token == _end) { return false; }
        if (equals("<<"))
        {
            if (_token == _end) { return false; }
            if (expression(level))
            {
                if (_token == _end) { return false; }
                if (ostream_list(level))
                {
                    return true;
                }
            }
        }
    }
    else if (equals("cin"))
    {
        if (_token == _end) { return false; }
        if (equals(">>"))
        {
            if (_token == _end) { return false; }
            if (equals(Tokenizer::Identifier))
            {
                return true;
            }
        }
    }
    else
    {
        popBranch();
    }

    _token = temp;
    return false;
}

// OSTREAM_LIST → << EXPRESSION OSTREAM_LIST | λ
bool Parser::ostream_list(int level)
{
    if (_token == _end)
    {
        return true;
    }
    auto temp = _token;

    // Code here
    addBranch(level++, "<OSTREAM LIST>");
    if (equals("<<"))
    {
        if (_token == _end) { return false; }
        if (expression(level))
        {
            if (_token == _end) { return false; }
            if (ostream_list(level))
            {
                return true;
            }
        }
    }
    else
    {
        popBranch();
    }

    _token = temp;
    return true; // λ
}

// RVALUE → MAG RVALUE2
bool Parser::rvalue(int level)
{
    if (_token == _end) { return false; }
    auto temp = _token;

    addBranch(level++, "<RVALUE>");
    if (mag(level))
    {
        if (_token == _end) { return false; }
        if (rvalue2(level))
        {
            return true;
        }
    }
    else
    {
        popBranch();
    }

    _token = temp;
    return false;
}

// RVALUE2 → COMPARE MAG RVALUE2 | λ
bool Parser::rvalue2(int level)
{
    if (_token == _end)
    {
        return true;
    }
    auto temp = _token;

    if (compare(level))
    {
        if (_token == _end) { return false; }
        if (mag(level))
        {
            if (_token == _end) { return false; }
            if (rvalue2(level))
            {
                return true;
            }
        }
    }

    _token = temp;
    return true; // λ
}

// COMPARE → == | < | > | <= | >= | !=
bool Parser::compare(int level)
{
    if (_token == _end) { return false; }
    auto temp = _token;

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

    _token = temp;
    return false;
}

// MAG → TERM MAG2
bool Parser::mag(int level)
{
    if (_token == _end) { return false; }
    auto temp = _token;

    addBranch(level++, "<MAG>");
    if (term(level))
    {
        if (_token == _end) { return false; }
        if (mag2(level))
        {
            return true;
        }
    }
    else
    {
        popBranch();
    }

    _token = temp;
    return false;
}

// MAG2 → + TERM MAG2 | - TERM MAG2 | λ
bool Parser::mag2(int level)
{
    if (_token == _end)
    {
        return true;
    }
    auto temp = _token;

    if (equals("+"))
    {
        if (_token == _end) { return false; }
        if (term(level))
        {
            if (_token == _end) { return false; }
            if (mag2(level))
            {
                return true;
            }
        }
    }
    else if (equals("-"))
    {
        if (_token == _end) { return false; }
        if (term(level))
        {
            if (_token == _end) { return false; }
            if (mag2(level))
            {
                return true;
            }
        }
    }

    _token = temp;
    return true; // λ
}

// TERM → FACTOR TERM2
bool Parser::term(int level)
{
    if (_token == _end) { return false; }
    auto temp = _token;

    addBranch(level++, "<TERM>");
    if (factor(level))
    {
        if (_token == _end) { return false; }
        if (term2(level))
        {
            return true;
        }
    }
    else
    {
        popBranch();
    }

    _token = temp;
    return false;
}

// TERM2 → * FACTOR TERM2 | / FACTOR TERM2 | % FACTOR TERM2 | λ
bool Parser::term2(int level)
{
    if (_token == _end)
    {
        return true;
    }
    auto temp = _token;

    if (equals("*"))
    {
        if (_token == _end) { return false; }
        if (factor(level))
        {
            if (_token == _end) { return false; }
            if (term2(level))
            {
                return true;
            }
        }
    }
    else if (equals("/"))
    {
        if (_token == _end) { return false; }
        if (factor(level))
        {
            if (_token == _end) { return false; }
            if (term2(level))
            {
                return true;
            }
        }
    }
    else if (equals("%"))
    {
        if (_token == _end) { return false; }
        if (factor(level))
        {
            if (_token == _end) { return false; }
            if (term2(level))
            {
                return true;
            }
        }
    }
    
    _token = temp;
    return true; // λ
}

// FACTOR → ( EXPRESSION ) | - FACTOR | + FACTOR | Identifier FACTOR2 | VALUE
bool Parser::factor(int level)
{
    if (_token == _end) { return false; }
    auto temp = _token;

    addBranch(level++, "<FACTOR>");
    if (equals("("))
    {
        if (_token == _end) { return false; }
        if (expression(level))
        {
            if (_token == _end) { return false; }
            if (equals(")"))
            {
                return true;
            }
        }
    }
    else if (equals("-"))
    {
        if (_token == _end) { return false; }
        if (factor(level))
        {
            return true;
        }
    }
    else if (equals("+"))
    {
        if (_token == _end) { return false; }
        if (factor(level))
        {
            return true;
        }
    }
    else if (equals(Tokenizer::Identifier))
    {
        addBranch(level, "<Identifier>");
        addBranch(level + 1, (_token - 1)->value());

        if (_token == _end) { return false; }
        if (factor2(level))
        {
            return true;
        }
    }
    else if (value(level))
    {
        return true;
    }
    else
    {
        popBranch();
    }

    _token = temp;
    return false;
}

//FACTOR2 → ++ | -- | λ
bool Parser::factor2(int level)
{
    if (_token == _end)
    {
        return true;
    }
    auto temp = _token;

    if (equals("++"))
    {
        return true;
    }
    else if (equals("--"))
    {
        return true;
    }
    
    _token = temp;
    return true; // λ
}

// VALUE → Integer | Float | String | Boolean
bool Parser::value(int level)
{
    if (_token == _end) { return false; }
    auto temp = _token;

    addBranch(level++, "<VALUE>");
    if (equals(Tokenizer::Integer))
    {
        addBranch(level, "<Integer>");
        addBranch(level + 1, (_token - 1)->value());
        return true;
    }
    else if (equals(Tokenizer::Float))
    {
        addBranch(level, "<Float>");
        addBranch(level + 1, (_token - 1)->value());
        return true;
    }
    else if (equals(Tokenizer::String))
    {
        std::string str("\"");
        str.append((_token - 1)->value());
        str.append("\"");

        addBranch(level, "<String>");
        addBranch(level + 1, str);
        return true;
    }
    else if (equals(Tokenizer::Boolean))
    {
        addBranch(level, "<Boolean>");
        addBranch(level + 1, (_token - 1)->value());
        return true;
    }
    else
    {
        popBranch();
    }

    _token = temp;
    return false;
}