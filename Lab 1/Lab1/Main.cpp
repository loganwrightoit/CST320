/***********************************************************
* Author:                   Logan Wright
* Date Created:             01/16/2015
* Last Modification Date:   01/16/2015
* Lab Number:               CST 320 Lab 1
* Filename:                 Main.cpp
*
* Overview:
*   A lexical analyzer and preprocessor used to remove
*   comments and replace preprocessor directives from a
*   code file based on C++.
*
* Input:
*   Used inputs name of a text file containing C++ code.
*
* Output:
*   Not sure yet.
************************************************************/

//#include "SymbolTable.h" // Includes Symbol.h

#include "Tokenizer.h"
#include "Preprocessor.h"
#include "StateMachine.h"

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

//
// Forward declarations
//
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

    //printTokens();

    return 0;
}

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

/**************************************************************
*   Entry:  None.
*
*    Exit:  None.
*
* Purpose:  Creates a symbol table and performs simple tests
*           with basic console output.
***************************************************************/
void runSymbolTableTest()
{
    /*
    // Stores symbols
    SymbolTable symbolTable;

    cout << "Beginning symbol table test.\n\n";

    // Add symbols to table
    cout << "- Adding symbols to table.\n";
    symbolTable.add(Symbol("isReady", Symbol::Bool, Symbol::VariableName, "true"));
    symbolTable.add(Symbol("foo", Symbol::Double, Symbol::FunctionName, ""));

    cout << "Table size: " << symbolTable.size() << endl;

    for (int idx = 0; idx < 3; ++idx)
    {
        string name;

        switch (idx)
        {
        case 0:
            name = "isReady";
            break;
        case 1:
            name = "someVariable";
            break;
        case 2:
            name = "foo";
            break;
        }

        cout << "- Attempting to find symbol: " << name << endl;
        Symbol* symbol = symbolTable.find(name);
        if (symbol != nullptr)
        {
            cout << "- Found symbol in table, attempting to remove.\n";
            symbolTable.remove(name);
            cout << "- Symbol removed, new table size: " << symbolTable.size() << endl;
        }
        else
        {
            cout << "ERROR: Could not find symbol in table.\n";
        }
    }
    */
}