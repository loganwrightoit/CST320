#pragma once

#include <map>
#include "Symbol.h"

class SymbolTable
{

    public:

        SymbolTable();
        ~SymbolTable();

        bool add(Symbol symbol);
        Symbol* find(std::string name);
        bool remove(std::string name);
        int size();

    private:

        std::map<std::string, Symbol> _table;

};

