#pragma once

#include "Tokenizer.h"
#include <map>
#include <string>
#include <vector>
#include <stack>
#include <assert.h>

class LL1_TableParser
{

    public:

        LL1_TableParser(std::string grammar);
        ~LL1_TableParser();

        // Returns true if token string parses
        bool parse(std::vector<Tokenizer::Token> tokens);

    private:

        bool isTerminal();

        template<typename T>
        void pop_front(std::vector<T>& vec)
        {
            assert(!vec.empty());
            vec.erase(vec.begin());
        }

        std::stack<std::string> _stack;

        //  S -> aSa
        //  S -> b
        //
        // In table:
        // S  a  a S a
        // S  b  b
        std::map<std::string, std::map<std::string, std::vector<std::string>>> table;

};

