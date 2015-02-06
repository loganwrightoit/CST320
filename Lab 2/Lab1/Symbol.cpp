/***********************************************************
* Author:                   Logan Wright
* Date Created:             01/09/2015
* Last Modification Date:   01/19/2015
* Lab Number:               CST 320 Lab 1
* Filename:                 Symbol.cpp
*
* Overview:
*   Provides symbol data that is compatible with the
*   SymbolTable class.  It allows named symbols to specify
*   token type, use, and a given value.
*
* Input:
*   No input.
*
* Output:
*   No output.
************************************************************/

#include "Symbol.h"

Symbol::Symbol(std::string name, Type type, Use use, std::string value) :
    _name(name), _type(type), _use(use), _value(value) {}

Symbol::~Symbol()
{
}

/**************************************************************
*   Entry:  None.
*
*    Exit:  A string identifier.
*
* Purpose:  Returns the name of the symbol used for the symbol
*           table.
***************************************************************/
std::string Symbol::getName() const
{
    return _name;
};

/**************************************************************
*   Entry:  None.
*
*    Exit:  A Type enumerator.
*
* Purpose:  Returns the type of the symbol, such as Int, Float,
*           String, etc.
***************************************************************/
Symbol::Type Symbol::getType() const
{
    return _type;
};

/**************************************************************
*   Entry:  None.
*
*    Exit:  A Use enumerator.
*
* Purpose:  Returns the use of the symbol, such as whether it
*           is a function name, variable name, constant, etc.
***************************************************************/
Symbol::Use Symbol::getUse() const
{
    return _use;
};

/**************************************************************
*   Entry:  None.
*
*    Exit:  A string representing a value.
*
* Purpose:  Returns the string value of the symbol, such as when
*           it represents a constant.
***************************************************************/
std::string Symbol::getValue() const
{
    return _value;
};