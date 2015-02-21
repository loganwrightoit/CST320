/***********************************************************
* Author:                   Logan Wright
* Date Created:             01/16/2015
* Last Modification Date:   02/20/2015
* Lab Number:               CST 320 Lab 3
* Filename:                 Main.cpp
*
* Overview:
*   A lexical analyzer, preprocesser, and parser used to
*   process a C++ style language.
*
* Input:
*   User inputs name of a text file containing C++ code.
*
* Output:
*   Prints post-preprocessed code and final parse result.
************************************************************/

#include "Tokenizer.h"
#include "Preprocessor.h"
#include "StateMachine.h"
#include "Parser.h"
#include "LL1_TableParser.h"

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

// Recursive descent parser object
static Parser parser;

// LL1 table parser object
static LL1_TableParser tableParser("grammar.txt");

//
// Forward declarations
//
void parseUsingRecursiveDescent();
void parseUsingLL1Table();
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
    int lineNum = 0;
    while (std::getline(file, line))
    {
        ++lineNum;
        size_t initLen = line.length();
        line = preprocessor.run(line);

        // Avoid printing lines preprocessed to empty strings
        if (line.length() >= initLen)
        {
            //cout << line << endl;
        }

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

    //parseUsingRecursiveDescent();
    parseUsingLL1Table();
    //printTokens();

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
    cout << "\n[RD_PARSER]: " << (result ? "Yes" : "No") << endl;
}

/**************************************************************
*   Entry:  None.
*
*    Exit:  None.
*
* Purpose:  Parses tokens contained in global token vector using
*           LL1 table implementation.
***************************************************************/
void parseUsingLL1Table()
{
    bool result = tableParser.parse(tokens);
    cout << "\n[LL1_PARSER]: " << (result ? "Yes" : "No") << endl;
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
    cout << endl << right << setw(20) << "Token" << setw(20) << "Token Type" << endl;
    cout << setw(40) << setfill('-') << "-" << setfill(' ') << endl;
    auto iter = tokens.begin();
    while (iter != tokens.end())
    {
        cout << setw(20) << iter->value() << setw(20) << tokenizer.enumToString(iter->type()) << endl;
        ++iter;
    }
}