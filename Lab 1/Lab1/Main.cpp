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

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include "Tokenizer.h"
#include "StateMachine.h"
#include "SymbolTable.h" // Includes Symbol.h

using namespace std;

// Program code file
ifstream file;

// Stores token and type
vector<std::pair<string, Tokenizer::TokenType>> tokens;

// Forward declarations
void runTokenizer(char* fileName);
void runSymbolTable();

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

    // Tokenize file
    runTokenizer(argv[1]);

    // Test symbol table
    //runSymbolTable();

    return 0;
}

/**************************************************************
*   Entry:  fileName - the name of the file that contains the
*                      code to tokenize.
*
*    Exit:  None.
*
* Purpose:  Tokenizes a c++ code file.*
***************************************************************/
void runTokenizer(char* fileName)
{
    // Open file
    file.open(fileName);
    if (!file.is_open()) {
        cerr << "ERROR: could not open file " << fileName << ", exiting." << endl;
        return;
    }

    // Create tokenizer
    Tokenizer tokenizer("tokens.txt");

    // Create state machine
    StateMachine stateMachine;

    // Begin analyzing file
    std::string line;
    while (std::getline(file, line))
    {
        std::vector<std::pair<string, Tokenizer::TokenType>> result = tokenizer.Tokenize(line);

        auto iter = result.begin();
        while (iter != result.end())
        {
            // Check state machine for the following token types:
            // - Integer
            // - Float
            // - Identifier
            if (iter->second == tokenizer.Invalid)
            {
                iter->second = stateMachine.GetTokenType(iter->first);
            }

            // Store token and type pair
            tokens.push_back(*iter);
            ++iter;
        }
    }

    // Output results to console
    cout << endl << right << setw(20) << "Token" << setw(20) << "Token Type" << endl;
    cout << setw(40) << setfill('-') << "-" << setfill(' ') << endl;
    auto iter = tokens.begin();
    while (iter != tokens.end())
    {
        cout << setw(20) << iter->first << setw(20) << tokenizer.EnumToString(iter->second) << endl;
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
void runSymbolTable()
{
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
        std::string name;

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
}