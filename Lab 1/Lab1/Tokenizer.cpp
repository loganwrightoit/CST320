/***********************************************************
* Author:                   Logan Wright
* Date Created:             12/01/2014
* Last Modification Date:   12/XX/2014
* Lab Number:               CST 229 Lab 2
* Filename:                 Tokenizer.cpp
*
* Overview:
*   This program is an extension to Lab 1's state machine.
*   It tokenizes a text file containing programming
*   language code (C++ in this case), and identifies each
*   token as a symbol, operator, keyword, identifier, etc.
*   A token may not match any expected type, in which case
*   it will be marked as invalid.
*
* Input:
*   The user must provide a text file name containing
*   the program code, such as: code1.txt
*
* Output:
*   The program will analyze the file and display all
*   tokens found with a description.
*
*       Example:    Token               Token Type
*                   for                 Keyword
*                   while               Keyword
*                   _test               Identifier
*                   @                   Invalid
*                   12                  Integer
************************************************************/

#include "Tokenizer.h"

#include <vector>
#include <map>
#include <string>

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

Tokenizer::Tokenizer()
{
    std::string fileName = "tokens.txt";
    std::ifstream file;

    // Open file
    file.open(fileName);
    if (!file.is_open()) {
        std::cerr << "ERROR: could not open file " << fileName << ", exiting." << std::endl;
        return;
    }

    // Begin analyzing file
    std::string line;
    while (std::getline(file, line))
    {
        if (line.size() > 0)
        {
            // Split line into tokens
            std::istringstream buf(line);
            std::istream_iterator<std::string> beg(buf), end;
            std::vector<std::string> tokens(beg, end);

            // First token is TokenType
            TokenType tokenType = TokenType::Invalid;
            for (TokenType type = TokenType::start; type < TokenType::end; type = TokenType(type + 1))
            {
                if (enumToString(type) == *tokens.begin())
                {
                    tokenType = type;
                }
            }

            // Remove TokenType from string
            tokens.erase(tokens.begin());

            // Add remaining tokens
            for (auto& token : tokens) {
                tokenMatcher.insert(std::pair<std::string, TokenType>(token, tokenType));
            }
        }
    }
    
    // Add character symbols and operators as additional delimiters
    auto iter = tokenMatcher.begin();
    while (iter != tokenMatcher.end())
    {
        if (iter->second == CharSymbol || iter->second == Operator)
        {
            delimiters.append(iter->first);
        }
        ++iter;
    }
}

Tokenizer::~Tokenizer()
{
}

/**************************************************************
*   Entry:  inStr is a line of text containing keywords, identifiers,
*			and other C++ language components.
*
*    Exit:  A vector containing pairs of token strings and TokenType
*			enum values.
*
* Purpose:  This function takes an input string, which ordinarily
*			will be a line from a file, and splits it into tokens.
*
***************************************************************/
std::vector<Tokenizer::Token> Tokenizer::tokenize(std::string inStr)
{
    std::vector<Token> tokens;
    size_t pos = 0, prev = 0;

    // Add new-line at end of string to make tokenizing last line simpler
    inStr.append("\n");

    while ((pos = inStr.find_first_of(delimiters, prev)) != std::string::npos)
    {
        // Add whole token types
        if (pos != prev)
        {
            tokens.push_back(Tokenizer::getToken(prev, inStr.substr(prev, pos - prev)));
        }

        // If position is before defined symbol, add it
        std::string symbol = inStr.substr(pos, 1);
        if (tokenMatcher.find(symbol) != tokenMatcher.end())
        {
            // If position is before String declarator, add symbols and string and adjust position in line
            if (symbol == "\"")
            {
                size_t endPos = inStr.find_first_of("\"", pos + 1);
                tokens.push_back(Tokenizer::getToken(pos, inStr.substr(pos, 1)));
                tokens.push_back(Token(pos + 1, TokenType::String, inStr.substr(pos + 1, endPos - pos - 1)));
                tokens.push_back(Tokenizer::getToken(endPos, inStr.substr(endPos, 1)));
                prev = pos = endPos + 1;
                continue;
            }
            else
            {
                tokens.push_back(Tokenizer::getToken(pos, symbol));
            }            
        }

        prev = pos + 1;
    }

    return tokens;
}

/**************************************************************
*   Entry:  A string token.
*
*    Exit:  A Token object.
*
* Purpose:  Turns a string token into a Token object.
*
***************************************************************/
Tokenizer::Token Tokenizer::getToken(size_t pos, std::string inToken)
{
    auto iter = tokenMatcher.find(inToken);
    if (iter != tokenMatcher.end())
    {
        return Token(pos, iter->second, inToken);
    }
    else
    {
        return Token(pos, Invalid, inToken);
    }
}

/**************************************************************
*   Entry:  A TokenType.
*
*    Exit:  A string representation of the TokenType.
*
* Purpose:  This function is mainly for debug purposes.  It
*			prints the enum TokenType as a string representation.
*
***************************************************************/
std::string Tokenizer::enumToString(TokenType type)
{
    switch (type)
    {
        case Integer:
            return "Integer";
        case Float:
            return "Float";
        case Keyword:
            return "Keyword";
        case Identifier:
            return "Identifier";
        case CharSymbol:
            return "CharSymbol";
        case Operator:
            return "Operator";
        case Boolean:
            return "Boolean";
        case String:
            return "String";
        default:
            return "Invalid";
    }
}