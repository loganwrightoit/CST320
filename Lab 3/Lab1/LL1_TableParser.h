#pragma once

#include "Tokenizer.h"
#include <map>
#include <string>
#include <vector>

class LL1_TableParser
{

    public:

        LL1_TableParser(std::string grammar);
        ~LL1_TableParser();

        // Returns true if token string parses
        bool parse(std::vector<Tokenizer::Token>);

    private:

        //  S -> aSa
        //  S -> b
        //
        // In table:
        // S  a  a S a
        // S  b  b
        std::map<std::string, std::map<std::string, std::vector<std::string>>> table;

};

