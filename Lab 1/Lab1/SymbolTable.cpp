#include "SymbolTable.h"

SymbolTable::SymbolTable()
{
}


SymbolTable::~SymbolTable()
{
}

bool SymbolTable::add(Symbol symbol)
{
    // Second value of pair is if element insertion was successful
    // If element already existed in table, will return false
    return (_table.insert(std::make_pair(symbol.getName(), symbol))).second;
};

Symbol* SymbolTable::find(std::string name)
{
    auto result = _table.find(name);
    if (result != _table.end())
    {
        return &(result->second);
    }

    return nullptr;
};

bool SymbolTable::remove(std::string name)
{
    // Returns number of entries removed (max 1)
    return _table.erase(name);
};

int SymbolTable::size()
{
    return _table.size();
}