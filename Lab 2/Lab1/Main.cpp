/***********************************************************
* Author:                   Logan Wright
* Date Created:             01/16/2015
* Last Modification Date:   02/06/2015
* Lab Number:               CST 320 Lab 2
* Filename:                 Main.cpp
*
* Overview:
*   A lexical analyzer, preprocessoer, and parser used to
*   process a C++ style language.
*
* Input:
*   User inputs name of a text file containing C++ code.
*
* Output:
*   Prints results of tokenizer and parse result.
************************************************************/

#include "Tokenizer.h"
#include "Preprocessor.h"
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

// Preprocessor object
static Preprocessor preprocessor;

// State machine object
static StateMachine stateMachine;

// Stores token and type
std::vector<Tokenizer::Token> tokens;

// Parser object
static Parser parser;

//
// Forward declarations
//
void parseTokens();
void printTokens();

//
//  Main program thread.
//
int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cout << "ERROR: Must supply filename for code sample." << endl;
        return 1;
    }

    // Open file
    file.open(argv[1]);
    if (!file.is_open()) {
        cerr << "ERROR: could not open file " << argv[1] << ", exiting." << endl;
        return 1;
    }

    // Begin analyzing code

    std::string line;
    while (std::getline(file, line))
    {
        line = preprocessor.run(line);
        cout << line << endl;

        // Create TokenType pairs with preprocessed line
        auto result = tokenizer.tokenize(line);
        auto iter = result.begin();
        while (iter != result.end())
        {
            // Store token and type pair
            tokens.push_back(*iter);
            ++iter;
        }
    }

    parseTokens();

    printTokens();

    return 0;
}

/**************************************************************
*   Entry:  None.
*
*    Exit:  None.
*
* Purpose:  Parses tokens contained in global token vector.
***************************************************************/
void parseTokens()
{
    bool result = parser.parse(tokens);
    cout << "[PARSER]: Parse result: " << (result ? "SUCCESS" : "FAILURE") << endl;
}

/**************************************************************
*   Entry:  None.
*
*    Exit:  None.
*
* Purpose:  Prints contents of the tokens found using the
*           inputted code text file.
***************************************************************/
void printTokens()
{
    cout << endl << right << setw(20) << "Token" << setw(25) << "Token Type" << endl;
    cout << setw(40) << setfill('-') << "-" << setfill(' ') << endl;
    auto iter = tokens.begin();
    while (iter != tokens.end())
    {
        cout << setw(20) << iter->value() << setw(25) << tokenizer.enumToString(iter->type()) << endl;
        ++iter;
    }
}