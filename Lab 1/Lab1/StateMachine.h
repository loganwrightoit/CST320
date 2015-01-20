/***********************************************************
* Author:                   Logan Wright
* Date Created:             12/01/2014
* Last Modification Date:   01/20/2015
* Lab Number:               CST 320 Lab 1
* Filename:                 StateMachine.h
*
* Overview:
*   A state machine for loading language text files and
*   creating transition maps to identify whether a string
*   of text fits a language.
*
* Input:
*   None.
*
* Output:
*   None.
************************************************************/

#pragma once

#include "Tokenizer.h"

class StateMachine
{

    public:

        StateMachine();
        ~StateMachine();

        /**********************************************************************
        *   Tokenizer::TokenType GetTokenType(std::string token)
        *
        *      Purpose: Gets the TokenType that represents the given std::string.
        *
        *		 Entry:	A std::string token.
        *
        *		  Exit:	A TokenType enum.
        ***********************************************************************/

        Tokenizer::TokenType getTokenType(std::string token);

    private:

        /**********************************************************************
        *	bool Isstd::stringValid(std::string token)
        *
        *      Purpose: Validates a std::string against the loaded language.
        *
        *		 Entry:	A std::string token.
        *
        *		  Exit:	a boolean for whether input std::string satisfies machine
        *				algorithm loaded into program.
        *
        *	void ResetStateMachine()
        *
        *	   Purpose:	Purges transition tables and variables.
        *
        *		 Entry:	None.
        *
        *		  Exit:	None.
        *
        *	void PrepareStateMachine(std::string fileName)
        *
        *	   Purpose:	Translates a machine definition into algorithms.
        *
        *		 Entry:	The filename of the language definition file to
        *				load.
        *
        *		  Exit:	None.
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

