/***********************************************************
* Author:                   Logan Wright
* Date Created:             01/16/2015
* Last Modification Date:   01/20/2015
* Lab Number:               CST 320 Lab 1
* Filename:                 Tokenizer.cpp
*
* Overview:
*   Tokenizes a code file using C++ token types.
*
* Input:
*   None.
*
* Output:
*   None.
************************************************************/

#include "Tokenizer.h"
#include "StateMachine.h"

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
            for (TokenType type = TokenType::begin; type < TokenType::end; type = TokenType(type + 1))
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
        if (iter->second == Symbol || iter->second == Operator)
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
std::vector<Tokenizer::Token> Tokenizer::tokenize(size_t line, std::string inStr)
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
            tokens.push_back(Tokenizer::getToken(line, prev, inStr.substr(prev, pos - prev)));
        }

        // If position is before defined symbol, add it
        std::string symbol = inStr.substr(pos, 1);
        std::string symbolPair = inStr.substr(pos, 2);
        if (tokenMatcher.find(symbol) != tokenMatcher.end())
        {
            // If position is before String declarator, add symbols and string and adjust position in line
            if (symbol == "\"")
            {
                size_t endPos = inStr.find_first_of("\"", pos + 1);
                //tokens.push_back(Tokenizer::getToken(pos, inStr.substr(pos, 1))); // Adds opening quote
                tokens.push_back(Token(line, pos + 1, TokenType::String, inStr.substr(pos + 1, endPos - pos - 1)));
                //tokens.push_back(Tokenizer::getToken(endPos, inStr.substr(endPos, 1))); // Adds closing quote
                prev = pos = endPos + 1;
                continue;
            }

            // Check for and add symbol pair
            else if (tokenMatcher.find(symbolPair) != tokenMatcher.end())
            {
                tokens.push_back(Tokenizer::getToken(line, pos, symbolPair));
                prev = pos + 2;
                continue;
            }

            // Add generic symbol type
            else
            {
                tokens.push_back(Tokenizer::getToken(line, pos, symbol));
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
Tokenizer::Token Tokenizer::getToken(size_t line, size_t pos, std::string inToken)
{
    auto iter = tokenMatcher.find(inToken);
    if (iter != tokenMatcher.end())
    {
        return Token(line, pos, iter->second, inToken);
    }
    else
    {
        Token token(line, pos, Invalid, inToken);
        
        // Attempt to match token using language files
        StateMachine stateMachine;
        token.setType(stateMachine.getTokenType(inToken));

        return token;
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
        case Directive:
            return "Directive";
        case Integer:
            return "Integer";
        case Float:
            return "Float";
        case Keyword:
            return "Keyword";
        case Identifier:
            return "Identifier";
        case Symbol:
            return "Symbol";
        case SymbolPair:
            return "SymbolPair";
        case Input:
            return "Input";
        case Output:
            return "Output";
        case Operator:
            return "Operator";
        case OperatorPair:
            return "OperatorPair";
        case Boolean:
            return "Boolean";
        case String:
            return "String";
        default:
            return "Invalid";
    }
}