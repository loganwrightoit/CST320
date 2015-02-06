/***********************************************************
* Author:                   Logan Wright
* Date Created:             01/19/2015
* Last Modification Date:   01/19/2015
* Lab Number:               CST 320 Lab 1
* Filename:                 Preprocessor.cpp
*
* Overview:
*   This handles preprocessor routines such as parsing
*   directives, adding symbols to the symbol table, removing
*   comments, and anything else necessary before a line of
*   code is passed to the tokenizer.
*
* Input:
*   None.
*
* Output:
*   None.
************************************************************/

#include "Preprocessor.h"
#include "Tokenizer.h"

#include <iostream>
#include <string>
#include <sstream>
#include <assert.h>

using namespace std;

Preprocessor::Preprocessor()
{
}

Preprocessor::~Preprocessor()
{
}

/**************************************************************
*   Entry:  A std::string line of code text to be tokenized.
*
*    Exit:  The resulting std::string line after running
*           preprocessing tasks on it.
*
* Purpose:  Processes a line by replacing symbols, removing
*           comments, and handling preprocessor directives.
***************************************************************/
std::string Preprocessor::run(std::string line)
{
    line = cleanComments(line);
    
    if (line.size() == 0)
        return "";

    // Check whether line should be tossed due to conditional state
    // This isn't complete, but it works well enough
    if (stringStartsWith(line, "#define") || !stringStartsWith(line, "#"))
    {
        if (emptyLine)
            return "";
    }

    if (line.substr(0, 1) == "#")
    {
        processDirective(line);
        return "";
    }

    line = replaceSymbols(line);

    return emptyLine ? "" : line;
}


/**************************************************************
*   Entry:  A std::string line of code text.
*
*    Exit:  A std::string line with comments removed.
*
* Purpose:  Removes comments from line.
***************************************************************/
std::string Preprocessor::cleanComments(std::string line)
{
    // Check for close comment
    if (inCommentBlock)
    {
        size_t pos = line.find("*/");
        if (pos == std::string::npos)
        {
            return "";
        }
        else
        {
            line.erase(0, pos + 2); // Erase commented portion
        }
        inCommentBlock = false;
        cleanComments(line); // Clean remaining comments
    }

    while (line.find("/*") != std::string::npos)
    {
        size_t pos = line.find("/*");
        size_t end = line.find("*/");
        if (end == std::string::npos)
        {
            inCommentBlock = true;
            line.erase(pos, line.find("\n", pos) - pos);
        }
        else
        {
            line.erase(pos, (line.find("*/", pos) - pos) + 2);
        }        
    }
    while (line.find("//") != std::string::npos)
    {
        size_t pos = line.find("//");
        line.erase(pos, line.find("\n", pos) - pos);
    }

    return line;
}

/**************************************************************
*   Entry:  A std::string line of code text.
*
*    Exit:  The resulting std::string line after replacing
*           symbols with values stored in the symbol table.
*
* Purpose:  Replaces symbols in the line of code text.
***************************************************************/
std::string Preprocessor::replaceSymbols(std::string line)
{
    Tokenizer tokenizer;
    auto tokens = tokenizer.tokenize(line);

    // Find and replace symbols in line
    for (auto token : tokens)
    {
        if (token.type() == tokenizer.Identifier)
        {
            auto symbol = symbolTable.find(token.value());
            if (symbol != nullptr)
            {
                if (!symbol->getValue().empty())
                {
                    std::string value = symbol->getValue();

                    // If value is string, put in quotes
                    if (value.find(" ") != string::npos)
                    {
                        value = "\"" + value + "\"";
                    }
                    
                    // Replace symbol with value
                    line.replace(token.pos(), token.value().length(), value);
                }
            }

        }
    }

    return line;
}

/**************************************************************
*   Entry:  A std::string line of code text.
*
*    Exit:  None.
*
* Purpose:  Handles directive tasks such as defining symbols,
*           and running conditional directives.
***************************************************************/
void Preprocessor::processDirective(std::string line)
{
    Tokenizer tokenizer;
    auto tokens = tokenizer.tokenize(line);

    switch (getDirective(tokens.at(0).value()))
    {
        case Define:
            assert(tokens.size() == 2 || tokens.size() == 3);
            if (tokens.size() == 3)
                assert(addSymbol(tokens.at(1), tokens.at(2).value()));
            else
                assert(addSymbol(tokens.at(1)));
            break;
        case IfNotDefined:
            assert(tokens.size() == 2);
            if (symbolTable.contains(tokens.at(1).value()))
                doElse = emptyLine = true;
            break;
        case IfDefined:
            assert(tokens.size() == 2);
            if (!symbolTable.contains(tokens.at(1).value()))
                doElse = emptyLine = true;
            break;
        case Else:
            emptyLine = !doElse;
            break;
        case EndIf:
            doElse = emptyLine = false;
            break;
    }
}

/**************************************************************
*   Entry:  A Token object.
*           A std::string value.
*
*    Exit:  Boolean true if symbol did not exist in table.
*
* Purpose:  Adds symbol to symbol table.
***************************************************************/
bool Preprocessor::addSymbol(Tokenizer::Token token, std::string value)
{
    assert(token.type() == Tokenizer::Identifier);
    Symbol symbol(token.value(), Symbol::String, Symbol::Use::VariableName, value);
    return symbolTable.add(symbol);
}

/**************************************************************
*   Entry:  A std::string line, and a std::string token.
*
*    Exit:  Boolean true if line starts begins with token.
*
* Purpose:  Returns true if a string line starts with the
*           provided token.
***************************************************************/
bool Preprocessor::stringStartsWith(std::string line, std::string token)
{
    return line.compare(0, token.length(), token) == 0;
}

/**************************************************************
*   Entry:  A std::string token value.
*
*    Exit:  A preprocessor directive type.
*
* Purpose:  Parses a token string into a directive type.
***************************************************************/
Preprocessor::Directive Preprocessor::getDirective(std::string name)
{
    if (name == "#define")
        return Define;
    else if (name == "#ifndef")
        return IfNotDefined;
    else if (name == "#ifdef")
        return IfDefined;
    else if (name == "#else")
        return Else;
    else
        return EndIf;
}