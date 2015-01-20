/***********************************************************
* Author:                   Logan Wright
* Date Created:             01/19/2015
* Last Modification Date:   01/19/2015
* Lab Number:               CST 320 Lab 1
* Filename:                 Preprocessor.h
*
* Overview:
*   This handles preprocessor routines such as parsing
*   directives, adding symbols to the symbol table, removing
*   comments, and anything else necessary before a line of
*   code is passed to the tokenizer.
*
* Input:
*   None.
*
* Output:
*   None.
************************************************************/

#pragma once

#include "SymbolTable.h" // Includes Symbol.h
#include "Tokenizer.h"

class Preprocessor
{

    public:

        enum Directive
        {
            Define,
            IfNotDefined,
            IfDefined,
            Else,
            EndIf
        };

        Preprocessor();
        ~Preprocessor();

        /**********************************************************************
        *	std::string run(std::string line)
        *
        *      Purpose: Processes a line by replacing symbols, removing
        *               comments, and handling preprocessor directives.
        *
        *		 Entry:	A std::string line of code text to be tokenized.
        *
        *		  Exit:	The resulting std::string line after running
        *               preprocessing tasks on it.
        ***********************************************************************/

        std::string run(std::string line);

    private:

        /**********************************************************************
        *	bool addSymbol(Tokenizer::Token token, std::string value = "")
        *
        *      Purpose: Adds symbol to symbol table.
        *
        *		 Entry: A Token object.
        *               A std::string value.
        *
        *		  Exit: Boolean true if symbol did not exist in table.
        *
        *   bool stringStartsWith(std::string line, std::string token)
        *
        *      Purpose: Returns true if a string line starts with the
        *               provided token.
        *
        *		 Entry: A std::string line, and a std::string token.
        *
        *		  Exit: Boolean true if line starts begins with token.
        *
        *   std::string cleanComments(std::string line)
        *
        *      Purpose: Removes comments from line.
        *
        *		 Entry: A std::string line of code text.
        *
        *		  Exit: A std::string line with comments removed.
        *
        *   std::string replaceSymbols(std::string line)
        *
        *      Purpose: Replaces symbols in the line of code text.
        *
        *		 Entry: A std::string line of code text.
        *
        *		  Exit: The resulting std::string line after replacing
        *               symbols with values stored in the symbol table.
        *
        *   void processDirective(std::string line)
        *
        *      Purpose: Handles directive tasks such as defining symbols,
        *               and running conditional directives.
        *
        *		 Entry: A std::string line of code text.
        *
        *		  Exit: None.
        *
        *   Directive getDirective(std::string token)
        *
        *      Purpose: Parses a token string into a directive type.
        *
        *		 Entry: A std::string token value.
        *
        *		  Exit: A preprocessor directive type.
        ***********************************************************************/

        bool addSymbol(Tokenizer::Token token, std::string value = "");
        bool stringStartsWith(std::string line, std::string token);
        std::string cleanComments(std::string line);
        std::string replaceSymbols(std::string line);
        void processDirective(std::string line);
        Directive getDirective(std::string token);

        // Indicates preprocessor is within a comment block
        bool inCommentBlock = false;

        // Indicates a line without a directive needs to be emptied
        bool emptyLine = false;

        // Whether preprocessor should handle next #else directive
        bool doElse = false;

        // The primary symbol table instance
        SymbolTable symbolTable;

};

