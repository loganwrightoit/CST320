/***********************************************************
* Author:                   Logan Wright
* Date Created:             01/09/2015
* Last Modification Date:   03/04/2015
* Lab Number:               CST 320 Final Project
* Filename:                 SymbolTable.cpp
*
* Overview:
*   This class works in conjunction with the Symbol object
*   class to store symbols within a table.  The table
*   supports adding symbols, removing symbols by name, and
*   searching for symbols by name.
*
* Input:
*   No input.
*
* Output:
*   No output.
************************************************************/

#include "SymbolTable.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <map>

using namespace std;

SymbolTable::SymbolTable()
{
}

SymbolTable::~SymbolTable()
{
}

std::map<std::string, std::vector<Symbol>> SymbolTable::getTable()
{
    return _table;
}

/**************************************************************
*   Entry:  None.
*
*    Exit:  None.
*
* Purpose:  Prints symbol table contents to console.
***************************************************************/
void SymbolTable::print()
{
    cout << '\n' << setw(20) << left << "Symbol Name" << setw(20) << "Symbol Use" << setw(20) << "Scope" << endl;
    cout << setfill('-') << setw(60) << "-" << setfill(' ') << endl;
    
    auto iter = _table.begin();
    while (iter != _table.end())
    {
        auto iter2 = iter->second.begin();
        while (iter2 != iter->second.end())
        {
            cout << setw(20) << iter2->getName() << setw(20) << (iter2->getUse() == Symbol::FunctionName ? "FunctionName" : "VariableName") << setw(20) << iter2->getScope() << endl;
            ++iter2;
        }
        ++iter;
    }
}

/**************************************************************
*   Entry:  symbol - the symbol to be added to table
*
*    Exit:  Boolean true if symbol was able to be inserted into
*           table.
*
*           Boolean false if table already contained symbol.
*
* Purpose:  Adds a new symbol to the symbol table.
***************************************************************/
bool SymbolTable::add(Symbol symbol)
{
    // Check if table contains vector for symbol
    auto result = _table.find(symbol.getName());
    if (result != _table.end())
    {
        // Return false if symbol already exists at scope level
        auto iter = result->second.begin();
        while (iter != result->second.end())
        {
            if (iter->getScope() == symbol.getScope())
            {
                return false;
            }
            iter++;
        }

        // Insert symbol in vector
        result->second.push_back(symbol);
    }
    else
    {
        // Create table entry and vector list to hold new symbol
        std::vector<Symbol> vec;
        vec.push_back(symbol);
        if (!_table.insert(std::make_pair(symbol.getName(), vec)).second)
        {
            return false;
        }
    }

    return true;
}

/**************************************************************
*   Entry:  name string - the name of the symbol.
*
*    Exit:  Returns true if symbol table contains symbol.
*
* Purpose:  Checks to see whether symbol table contains symbol.
***************************************************************/
bool SymbolTable::contains(std::string name)
{
    return _table.find(name) != _table.end();
}

/**************************************************************
*   Entry:  name string - the name of the symbol.
*
*    Exit:  Returns a vector of symbols if object was able to be
*           found in the symbol table.
*
*           Returns nullptr if symbol does not exist.
*
* Purpose:  Adds a new symbol to the symbol table.
***************************************************************/
std::vector<Symbol>* SymbolTable::find(std::string name)
{
    auto result = _table.find(name);
    if (result != _table.end())
    {
        return &(result->second);
    }

    return NULL;
}

/**************************************************************
*   Entry:  name string - the name of the symbol.
*
*    Exit:  Boolean true if symbol was removed from table.
*
*           Boolean false if symbol not in table.
*
* Purpose:  Removes a symbol from the symbol table.
***************************************************************/
bool SymbolTable::remove(std::string name)
{
    // Returns number of entries removed (max 1)
    return _table.erase(name);
}

/**************************************************************
*   Entry:  None.
*
*    Exit:  Returns number of entries in symbol table.
*
* Purpose:  Allows retrieval of number of entries in symbol
*           table map.
***************************************************************/
int SymbolTable::size()
{
    return _table.size();
}