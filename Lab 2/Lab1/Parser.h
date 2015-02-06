/***********************************************************
* Author:                   Logan Wright
* Date Created:             02/06/2015
* Last Modification Date:   02/06/2015
* Lab Number:               CST 320 Lab 2
* Filename:                 Parser.h
*
* Overview:
*   Parses a file using a Mini-C grammar.
*
* Input:
*   None.
*
* Output:
*   None.
************************************************************/

#pragma once
#include "Tokenizer.h"
#include <vector>

class Parser
{

    public:

        Parser();
        ~Parser();

        bool parse(std::vector<Tokenizer::Token> tokens);

};

