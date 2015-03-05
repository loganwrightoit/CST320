/***********************************************************
* Author:                   Logan Wright
* Date Created:             02/20/2015
* Last Modification Date:   03/04/2015
* Lab Number:               CST 320 Final Project
* Filename:                 LL1_TableParser.h
*
* Overview:
*   Parses a vector of tokens using an LL1 table
*   implementation.
*
* Input:
*   No input.
*
* Output:
*   Prints the contents of the stack and remaining input
*   tokens after every operation.
************************************************************/

#pragma once

#include "Tokenizer.h"
#include <map>
#include <string>
#include <vector>
#include <stack>
#include <assert.h>

class LL1_TableParser
{

    public:

        LL1_TableParser(std::string grammar);
        ~LL1_TableParser();

        /**********************************************************************
        *	bool parse(std::vector<Tokenizer::Token> tokens)
        *
        *      Purpose: Parses a vector of tokens using LL1 table implementation.
        *
        *		 Entry:	A std::vector<Tokenizer::Token> container.
        *
        *		  Exit:	True if tokens parse.
        ***********************************************************************/

        bool parse(std::vector<Tokenizer::Token> tokens);

    private:

        /**********************************************************************
        *	bool isNonTerminal()
        *
        *      Purpose: Checks if currently loaded token is a non-terminal.
        *
        *		 Entry:	None.
        *
        *		  Exit:	True if first token is non-terminal.
        ***********************************************************************/

        bool isNonTerminal();

        /**************************************************************
        *   Entry:  A std::vector<T> object reference.
        *
        *    Exit:  None.
        *
        * Purpose:  Pops the first element from a vector.
        ***************************************************************/
        template<typename T>
        void pop_front(std::vector<T>& vec)
        {
            assert(!vec.empty());
            vec.erase(vec.begin());
        }

        /**************************************************************
        *   Entry:  A std::vector<std::string> object reference.
        *
        *    Exit:  A std::vector<std::string> with elements flipped.
        *
        * Purpose:  Flips the elements of a vector and returns a new
        *           vector to mimic the structure of a stack.
        ***************************************************************/
        std::vector<std::string> reverse(std::vector<std::string> &v1)
        {
            std::vector<std::string> v2;
            size_t i = v1.size();
            while (i > 0)
                v2.push_back(v1[--i]);
            return v2;
        }

        // Used to store tokens to be matched to input
        std::stack<std::string> _stack;

        // The LL1 table object
        std::map<std::string, std::map<std::string, std::vector<std::string>>> _table;

};

