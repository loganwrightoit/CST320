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

        bool isNonTerminal();

        template<typename T>
        void pop_front(std::vector<T>& vec)
        {
            assert(!vec.empty());
            vec.erase(vec.begin());
        }

        // For reversing tokens so they input to stack in correct order
        std::vector<std::string> reverse(std::vector<std::string> &v1)
        {
            std::vector<std::string> v2;
            size_t i = v1.size();
            while (i > 0)
                v2.push_back(v1[--i]);
            return v2;
        }

        std::stack<std::string> _stack;
        std::map<std::string, std::map<std::string, std::vector<std::string>>> _table;

};

