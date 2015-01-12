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

        bool add(Symbol symbol);
        Symbol* find(std::string name);
        bool remove(std::string name);
        int size();

    private:

        // The symbol table
        std::map<std::string, Symbol> _table;

};

