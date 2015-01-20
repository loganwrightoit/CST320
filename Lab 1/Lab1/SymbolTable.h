/***********************************************************
* Author:                   Logan Wright
* Date Created:             01/09/2015
* Last Modification Date:   01/12/2015
* Lab Number:               CST 320 Lab 1
* Filename:                 SymbolTable.h
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

#pragma once

#include "Symbol.h"

#include <map>

class SymbolTable
{

    public:

        SymbolTable();
        ~SymbolTable();

        /**********************************************************************
        *   bool add(Symbol symbol)
        *
        *      Purpose: Adds a new symbol to the symbol table.
        *
        *        Entry: symbol - the symbol to be added to table
        *
        *         Exit: Boolean true if symbol was able to be inserted into
        *               table.
        *
        *               Boolean false if table already contained symbol.
        *
        *   bool contains(std::string name)
        *
        *      Purpose: Checks to see whether symbol table contains symbol.
        *
        *        Entry: the std::string name of the symbol.
        *
        *         Exit: Returns true if symbol table contains symbol.
        *
        *   Symbol* find(std::string name)
        *
        *      Purpose: Adds a new symbol to the symbol table.
        *
        *        Entry: the std::string name of the symbol.
        *
        *         Exit: Returns a symbol pointer if object was able to be
        *               found in the symbol table.
        *
        *               Returns nullptr if symbol does not exist.
        *
        *   bool remove(std::string name)
        *
        *      Purpose: Removes a symbol from the symbol table.
        *
        *        Entry: The std::string name of the symbol.
        *
        *         Exit: Boolean true if symbol was removed from table.
        *
        *               Boolean false if symbol not in table.
        *
        *   int size()
        *
        *      Purpose: Allows retrieval of number of entries in symbol
        *               table map.
        *
        *        Entry: None.
        *
        *         Exit: Returns number of entries in symbol table.
        ***********************************************************************/

        bool add(Symbol symbol);
        bool contains(std::string name);
        Symbol* find(std::string name);
        bool remove(std::string name);
        int size();

    private:

        // The symbol table
        std::map<std::string, Symbol> _table;

};

