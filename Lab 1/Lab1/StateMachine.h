/***********************************************************
* Author:                   Logan Wright
* Date Created:             12/01/2014
* Last Modification Date:   12/XX/2014
* Lab Number:               CST 229 Lab 2
* Filename:                 StateMachine.h
*
* Overview:
*   This program is an extension to Lab 1's state machine.
*   It tokenizes a text file containing programming
*   language code (C++ in this case), and identifies each
*   token as a symbol, operator, keyword, identifier, etc.
*   A token may not match any expected type, in which case
*   it will be marked as invalid.
*
* Input:
*   The user must provide a text file name containing
*   the program code, such as: code1.txt
*
* Output:
*   The program will analyze the file and display all
*   tokens found with a description.
*
*       Example:    Token               Token Type
*                   for                 Keyword
*                   while               Keyword
*                   _test               Identifier
*                   @                   Invalid
*                   12                  Integer
************************************************************/

#pragma once

#include "Tokenizer.h"

//#include <string>

class StateMachine
{

    public:

        StateMachine();
        ~StateMachine();

        /**********************************************************************
        *	Tokenizer::TokenType GetTokenType(std::string token);
        *
        *	  	Purpose:	Gets the TokenType that represents the given std::string.
        *
        *		Entry:		A std::string token.
        *
        *		Exit:		A TokenType enum.
        *
        ***********************************************************************/

        Tokenizer::TokenType getTokenType(std::string token);

    private:

        /**********************************************************************
        *	bool Isstd::stringValid(std::string token);
        *
        *		Purpose:	Validates a std::string against the loaded language.
        *
        *		Entry:		A std::string token.
        *
        *		Exit:		a boolean for whether input std::string satisfies machine
        *					algorithm loaded into program.
        *
        *	void ResetStateMachine();
        *
        *	  	Purpose:	Purges transition tables and variables.
        *
        *		Entry:		none.
        *
        *		Exit:		None.
        *
        *	void PrepareStateMachine(std::string fileName);
        *
        *	  	Purpose:	Translates a machine definition into algorithms.
        *
        *		Entry:		The filename of the language definition file to
        *					load.
        *
        *		Exit:		none.
        *
        ***********************************************************************/

        bool isStringValid(std::string token);
        void resetStateMachine();
        void prepareStateMachine(std::string fileName);

        // Holds <filename,TokenType> pair for language definitions
        std::vector<std::pair<std::string, Tokenizer::TokenType>> definitions;

        // Holds letters defined in language
        std::string language;

        // Holds YES states
        std::vector<int> yesStates;

        // Store map pointers to release later
        std::vector<std::map<char, int>*> delMaps;

        // Holds state maps
        std::map<int, std::map<char, int>*> stateMaps;

        // Current state map
        std::map<char, int> *lastStateMap;

        // The initial state (determined by first state in first transition)
        int initState = -1;

        // Tracks last state for easy retrieval
        int currState = -1;

};

