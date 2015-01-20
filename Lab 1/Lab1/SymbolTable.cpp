/***********************************************************
* Author:                   Logan Wright
* Date Created:             01/09/2015
* Last Modification Date:   01/19/2015
* Lab Number:               CST 320 Lab 1
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

#include <string>
#include <map>

SymbolTable::SymbolTable()
{
}

SymbolTable::~SymbolTable()
{
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
    // Second value of pair is if element insertion was successful
    // If element already existed in table, will return false
    return (_table.insert(std::make_pair(symbol.getName(), symbol))).second;
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
*    Exit:  Returns a symbol pointer if object was able to be
*           found in the symbol table.
*
*           Returns nullptr if symbol does not exist.
*
* Purpose:  Adds a new symbol to the symbol table.
***************************************************************/
Symbol* SymbolTable::find(std::string name)
{
    Symbol* symbol = nullptr;

    auto result = _table.find(name);
    if (result != _table.end())
    {
        symbol = &(result->second);
    }

    return symbol;
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