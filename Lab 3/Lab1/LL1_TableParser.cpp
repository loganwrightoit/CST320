﻿/***********************************************************
* Author:                   Logan Wright
* Date Created:             02/20/2015
* Last Modification Date:   02/21/2015
* Lab Number:               CST 320 Lab 3
* Filename:                 LL1_TableParser.cpp
*
* Overview:
*   Parses a vector of tokens using an LL1 table
*   implementation.
*
* Input:
*   No input.
*
* Output:
*   Prints the contents of the stack and remaining input
*   tokens after every operation.
************************************************************/

#include "LL1_TableParser.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <cctype>
#include <iomanip>

LL1_TableParser::LL1_TableParser(std::string fileName)
{
    std::ifstream file;

    // Open file
    file.open(fileName);
    if (!file.is_open()) {
        std::cerr << "LL1_TableParser: could not open file " << fileName << std::endl;
        return;
    }

    // Build table
    std::string line;
    while (std::getline(file, line))
    {
        if (line.size() > 1 && line.substr(0, 2) == "//")
        {
            std::cout << "LL1_TableParser: skipping comment line" << std::endl;
            continue;
        }
        else if (std::count(line.begin(), line.end(), ' ') > 1)
        {
            // Split line into tokens
            std::istringstream buf(line);
            std::istream_iterator<std::string> beg(buf), end;
            std::vector<std::string> tokens(beg, end);

            if (tokens.size() > 2)
            {
                // Extract rule and id
                std::string nonTerminal = tokens.at(0);
                std::string terminal = tokens.at(1);

                if (_stack.empty())
                {
                    _stack.push(nonTerminal); // Add first non-terminal as start symbol
                }

                // Extract tokens for parsing
                std::vector<std::string> tkns(tokens.begin() + 2, tokens.end());

                auto rule_map = _table.find(nonTerminal);
                if (rule_map != _table.end())
                {
                    // Rule exists, just need to add token and tokens
                    auto result = rule_map->second.insert(std::make_pair(terminal, tkns));
                    if (!result.second)
                    {
                        std::cerr << "LL1_TableParser: table cannot contain two entries per cell" << std::endl;
                        return;
                    }
                }
                else
                {
                    // Rule does not exist, need to add rule token and tokens together
                    std::map<std::string, std::vector<std::string>> new_map;
                    new_map.insert(std::make_pair(terminal, tkns));
                    _table.insert(std::make_pair(nonTerminal, new_map));
                }

                // Output results of table insertion
                std::cout << "LL1_TableParser: " << tokens.at(0).c_str() << " -> " << tokens.at(1).c_str() << ", ";
                for (int idx = 0; idx < tkns.size(); ++idx)
                {
                    std::cout << tkns.at(idx).c_str() << " ";
                }
                std::cout << std::endl;
            }
            else
            {
                std::cerr << "LL1_TableParser: encountered malformed grammar rule" << std::endl;
                return;
            }
        }
        else
        {
            std::cout << "LL1_TableParser: skipping line of invalid length" << std::endl;
        }        
    }
}

LL1_TableParser::~LL1_TableParser()
{
}

bool LL1_TableParser::parse(std::vector<Tokenizer::Token> tokens)
{
    // Add EOF to input
    tokens.push_back(Tokenizer::Token(0, 0, Tokenizer::Boolean, "$"));

    std::cout << std::endl << std::left << std::setw(30) << "Stack" << std::setw(20) << "Input Buffer" << std::endl;
    std::cout << std::setw(50) << std::setfill('-') << '-' << std::endl;

    while (!_stack.empty())
    {
        // Print stack
        size_t sz = 0;
        std::string str;
        for (auto temp = _stack; !temp.empty(); temp.pop())
        {
            str.append(temp.top());
            ++sz;
        }
        std::cout << std::setw(30) << std::setfill(' ') << str.c_str();

        // Print input
        std::string str2;
        for (int idx = 0; idx < tokens.size(); ++idx)
        {
            str2.append(tokens.at(idx).value());
        }
        std::cout << std::setw(20) << str2.c_str() << std::endl;

        if (isNonTerminal())
        {
            // Check for terminal in table
            auto _rule = _table.find(_stack.top());
            if (_rule != _table.end())
            {
                // Check for tokens or lambda, otherwise default failure applies
                std::string searchTerm = tokens.begin()->type() == Tokenizer::Integer ? "Integer" : tokens.begin()->value();
                auto _toks = _rule->second.find(searchTerm);
                if (_toks != _rule->second.end())
                {
                    // Pop non-terminal
                    _stack.pop();

                    // Check for lambda production (skip pushing tokens into stack)
                    if (_toks->second.at(0) != "lambda")
                    {
                        // Flip tokens in vector for stack insertion
                        std::vector<std::string> flip = reverse(_toks->second);

                        // Push matched tokens in place of non-terminal
                        auto iter = flip.begin();
                        while (iter != flip.end())
                        {
                            _stack.push(*iter);
                            ++iter;
                        }
                    }
                }
                else
                {
                    std::cerr << "LL1_ParserTable: encountered empty cell" << std::endl;
                    return false;
                }
            }
            else
            {
                std::cerr << "LL1_ParserTable: unable to locate rule" << std::endl;
                return false;
            }
        }
        else
        {
            if (!_stack.empty() && tokens.size() > 0)
            {
                bool pop = false;

                switch (tokens.at(0).type())
                {
                    case Tokenizer::Integer:
                        pop = _stack.top() == "Integer";
                        break;
                    case Tokenizer::Identifier:
                        pop = _stack.top() == "Identifier";
                        break;
                }

                if (pop || _stack.top() == tokens.at(0).value())
                {
                    // Pop token and stack
                    pop_front(tokens);
                    _stack.pop();
                }
                else
                {
                    std::cerr << "LL1_ParserTable: unable to match terminal" << std::endl;
                    return false;
                }
            }
        }
    }
    
    // If input string is not empty, and contains more than EOF, it's bad
    if (!tokens.empty() && !(tokens.size() == 1 && tokens.at(0).value() == "$"))
    {
        return false;
    }

    return true;
}

/**********************************************************************
*	bool isNonTerminal()
*
*      Purpose: Checks if currently loaded token is a non-terminal.
*
*		 Entry:	None.
*
*		  Exit:	True if first token is non-terminal.
***********************************************************************/
bool LL1_TableParser::isNonTerminal()
{
    if (!_stack.empty() && !_stack.top().empty())
    {
        if (_stack.top().length() == 1)
        {
            // Check for uppercase single letter non-terminal
            return std::isupper(_stack.top()[0]);
        }
        else
        {
            // Check first two letters for upper casing
            return std::isupper(_stack.top()[0]) && std::isupper(_stack.top()[1]);
        }
    }
}