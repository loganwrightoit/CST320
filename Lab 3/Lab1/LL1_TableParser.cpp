#include "LL1_TableParser.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <cctype>
#include <iomanip>

using namespace std;

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

                auto rule_map = table.find(nonTerminal);
                if (rule_map != table.end())
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
                    table.insert(std::make_pair(nonTerminal, new_map));
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

    cout << left << setw(10) << "Stack" << setw(20) << "Input Buffer" << endl;

    while (!_stack.empty())
    {
        // Print stack
        size_t sz = 0;
        for (auto temp = _stack; !temp.empty(); temp.pop())
        {
            ++sz;
            cout << setw(1) << temp.top().c_str();
        }
        cout << setw(10 - sz) << setfill(' ') << ' ';

        // Print input
        for (int idx = 0; idx < tokens.size(); ++idx)
        {
            cout << tokens.at(idx).value().c_str();
        }
        cout << endl;

        if (isTerminal())
        {
            // Check for terminal in table
            auto _rule = table.find(_stack.top());
            if (_rule != table.end())
            {
                // Check for tokens or lambda, otherwise default failure applies
                auto _toks = _rule->second.find(tokens.begin()->value());
                if (_toks != _rule->second.end())
                {
                    // Pop non-terminal
                    _stack.pop();

                    // Push matched tokens in place of non-terminal
                    auto iter = _toks->second.begin();
                    while (iter != _toks->second.end())
                    {
                        _stack.push(*iter);
                        ++iter;
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
        else // top of stack is terminal, check against first token
        {
            // Check for empty input and end terminator
            if (tokens.empty() && _stack.top() == "$")
            {
                return true;
            }
            else if (!_stack.empty() && tokens.size() > 0 && _stack.top() == tokens.at(0).value())
            {
                // Pop token and stack
                pop_front(tokens);
                _stack.pop();
            }
            else
            {
                // There is no entry in the parser table for[a, $]

                std::cerr << "There is no entry in the parser table for [" << _stack.top().c_str() << ", " << tokens.begin()->value() << "]" << std::endl;
                return false;
            }
        }
    }

    return true;
}

bool LL1_TableParser::isTerminal()
{
    return !_stack.empty() && !_stack.top().empty() && std::isupper(_stack.top()[0]);
}