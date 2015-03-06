/***********************************************************
* Author:                   Logan Wright
* Date Created:             01/16/2015
* Last Modification Date:   03/04/2015
* Lab Number:               CST 320 Final Project
* Filename:                 Main.cpp
*
* Overview:
*   A lexical analyzer, preprocesser, and parser used to
*   process C++ code and build a parse tree.
*
* Input:
*   User inputs name of a text file containing C++ code.
*
* Output:
*   Saves the parse tree to a .txt file.
************************************************************/

#include "Tokenizer.h"
#include "StateMachine.h"
#include "Parser.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

// Program code file
std::ifstream file;

// Tokenizer object
static Tokenizer tokenizer;

// State machine object
static StateMachine stateMachine;

// Stores token and type
std::vector<Tokenizer::Token> tokens;

// Recursive descent parser object
static Parser parser;

//
// Forward declarations
//
void parseUsingRecursiveDescent();

//
// Main program thread.
//
int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cout << "ERROR: Must supply filename for code.txt" << endl;
        return 1;
    }

    // Open file
    file.open(argv[1]);
    if (!file.is_open()) {
        cerr << "ERROR: could not open file " << argv[2] << ", exiting." << endl;
        return 1;
    }

    // Begin analyzing code

    std::string line;
    int lineNum = 0;
    while (std::getline(file, line))
    {
        ++lineNum;
        size_t initLen = line.length();

        // Create TokenType pairs with preprocessed line
        auto result = tokenizer.tokenize(lineNum, line);
        auto iter = result.begin();
        while (iter != result.end())
        {
            // Store token and type pair
            tokens.push_back(*iter);
            ++iter;
        }
    }

    parseUsingRecursiveDescent();

    return 0;
}

/**************************************************************
*   Entry:  None.
*
*    Exit:  None.
*
* Purpose:  Parses tokens contained in global token vector using
*           recursive descent implementation.
***************************************************************/
void parseUsingRecursiveDescent()
{
    bool result = parser.parse(tokens);
}