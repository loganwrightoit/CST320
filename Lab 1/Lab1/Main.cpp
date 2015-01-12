/***********************************************************
* Author:                   Logan Wright
* Date Created:             01/09/2015
* Last Modification Date:   01/11/2015
* Lab Number:               CST 320 Lab 1
* Filename:                 Main.cpp
*
* Overview:
*   Description.
*
* Input:
*   Stuff.
*
* Output:
*   Stuff.
************************************************************/

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include "Tokenizer.h"
#include "StateMachine.h"
#include "SymbolTable.h"

using namespace std;

// Program code file
ifstream file;

// Stores token and type
vector<std::pair<string, Tokenizer::TokenType>> tokens;

// Stores symbols
SymbolTable symbolTable;

int main(int argc, char* argv[])
{
    // Tokenize string
    // beginTokenizer(argc, argv);

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

    return 0;
}

int beginTokenizer(int argc, char* argv[])
{
    string fileName;
    switch (argc)
    {
    case 2:
        fileName.append(argv[1]);
        break;
    default:
        cerr << "ERROR: argument <filename> required." << endl;
        return 1;
    }

    // Open file
    file.open(fileName);
    if (!file.is_open()) {
        cerr << "ERROR: could not open file " << fileName << ", exiting." << endl;
        return 1;
    }

    // Create tokenizer
    Tokenizer tokenizer;

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