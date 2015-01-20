/***********************************************************
* Author:                   Logan Wright
* Date Created:             01/09/2015
* Last Modification Date:   01/12/2015
* Lab Number:               CST 320 Lab 1
* Filename:                 Symbol.h
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

#pragma once

#include <string>

class Symbol
{

    public:

        // The type of data the symbol represents
        enum Type
        {
            Bool,
            Int,
            Float,
            String
        };

        // How the symbol is used
        enum Use
        {
            VariableName,
            Constant,
            FunctionName,
            ClassName,
            TypeName
        };

        Symbol(std::string name, Type type, Use use, std::string value);
        ~Symbol();

        /**********************************************************************
        *	std::string getName() const
        *
        *      Purpose: Returns the name of the symbol used for the symbol
        *               table.
        *
        *		 Entry:	None.
        *
        *		  Exit:	A string identifier.
        *
        *	Type getType() const
        *
        *      Purpose: Returns the type of the symbol, such as Int, Float,
        *               String, etc.
        *
        *		 Entry:	None.
        *
        *		  Exit:	A Type enumerator.
        *
        *	Use getUse() const
        *
        *      Purpose: Returns the use of the symbol, such as whether it
        *               is a function name, variable name, constant, etc.
        *
        *		 Entry:	None.
        *
        *		  Exit:	A Use enumerator.
        *
        *	std::string getValue() const
        *
        *      Purpose: Returns the string value of the symbol, such as when
        *               it represents a constant.
        *
        *		 Entry:	None.
        *
        *		  Exit:	A string representing a value.
        ***********************************************************************/

        std::string getName() const;
        Type getType() const;
        Use getUse() const;
        std::string getValue() const;

    private:

        // The symbol name
        std::string _name;

        // The symbol type
        Type _type;

        // The symbol use
        Use _use;

        // A value representation if needed, for constants mostly
        std::string _value;

};