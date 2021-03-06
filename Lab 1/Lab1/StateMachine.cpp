/***********************************************************
* Author:                   Logan Wright
* Date Created:             01/16/2014
* Last Modification Date:   01/19/2014
* Lab Number:               CST 320 Lab 1
* Filename:                 StateMachine.cpp
*
* Overview:
*   A state machine that can load language files defined in
*   plain text files.  It can match strings of text to
*   identify whether they fit the defined languages.
*
* Input:
*   None.
*
* Output:
*   None.
************************************************************/

#include "StateMachine.h"
#include "Tokenizer.h"

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

StateMachine::StateMachine()
{
    // Add language definitions
    definitions.push_back(std::make_pair("lang_integer.txt", Tokenizer::TokenType::Integer));
    definitions.push_back(std::make_pair("lang_float.txt", Tokenizer::TokenType::Float));
    definitions.push_back(std::make_pair("lang_identifier.txt", Tokenizer::TokenType::Identifier));
}

StateMachine::~StateMachine()
{
    resetStateMachine();
}

/**************************************************************
*   Entry:  token is the std::string to match.
*
*    Exit:  a TokenType for matching language type, or Invalid
*           if no match was found.
*
* Purpose:  Matches a token against a list of language
*           definitions to identify whether it matches a
*           TokenType.
***************************************************************/
Tokenizer::TokenType StateMachine::getTokenType(std::string token)
{
    //  Example language definition file
    //
    //    1       - Numbers of final states (lines to read for final states)
    //    0       - Final state
    //    ...     - More final states...
    //    2       - Number of transitions (lines to read for transition definitions)
    //    0 a 1   - Transition where state 0 goes to 1 if input is 'a' (first state is initial state)
    //    1 a 0   - More transitions...
    //

    // Cycle through language files looking for a match
    auto iter = definitions.begin();
    while (iter != definitions.end())
    {
        resetStateMachine();
        prepareStateMachine(iter->first);
        if (isStringValid(token))
        {
            return iter->second;
        }

        ++iter;
    }

    return Tokenizer::TokenType::Invalid;
}

/**************************************************************
*   Entry:  inStr is a character array.
*
*    Exit:  a boolean for whether input std::string satisfies machine
*           algorithm loaded into program.
*
* Purpose:  Validates a std::string against the loaded language.
***************************************************************/
bool StateMachine::isStringValid(std::string token)
{
    lastStateMap = (stateMaps.find(initState))->second; // Set current state map to start point (usually zero)
    for (unsigned int idx = 0; idx < token.length(); ++idx)
    {
        // Check if char is in language
        if (language.find(token.at(idx)) != std::string::npos)
        {
            auto iter = lastStateMap->find(token.at(idx)); // Skip char if it has no transition, or change state
            if (iter != lastStateMap->end())
            {
                int newState = currState = iter->second; // Grab new state and save it
                lastStateMap = stateMaps.find(newState)->second; // Change to new state map
            }
        }
        else
        {
            return false; // Char is not in language, so std::string is not in language
        }
    }

    // Check if final state value matches 'YES'
    auto iter = yesStates.begin();
    while (iter != yesStates.end())
    {
        if (*iter == currState) return true;
        iter++;
    }

    return false;
}

/**************************************************************
*   Entry:  none.
*
*    Exit:  none.
*
* Purpose:  Purges transition tables and variables.
***************************************************************/
void StateMachine::resetStateMachine()
{
    yesStates.clear();
    language = "";
    initState = currState = -1;
    lastStateMap = NULL;

    stateMaps.clear();

    // Cleanup dynamically-created maps
    auto iter = delMaps.begin();
    while (iter != delMaps.end())
    {
        delete *iter;
        iter++;
    }
    delMaps.clear();
}

/**************************************************************
*   Entry:  The filename of the language definition file to
*           load.
*
*    Exit:  none.
*
* Purpose:  Translates a machine definition into algorithms.
***************************************************************/
void StateMachine::prepareStateMachine(std::string fileName)
{
    // Load file
    std::fstream file;
    file.open(fileName);
    if (!file.is_open()) {
        cout << "ERROR: Could not open language file " << fileName << ", skipping." << endl;
        return;
    }

    std::string line;

    // Get number YES states as lines
    getline(file, line);
    int numYesStates = atoi(line.c_str());

    // Store YES states
    for (int state = 0; state < numYesStates; ++state)
    {
        getline(file, line);
        yesStates.push_back(atoi(line.c_str()));
    }

    // Grab number of transition lines defined
    getline(file, line);
    int numTransitions = atoi(line.c_str());

    // Map the transitions
    for (int count = 0; count < numTransitions; ++count)
    {
        getline(file, line);
        int beginState, endState;
        char byte;

        // Copy std::string to cstd::string for tokenizer
        char buffer[256] = "";
        strcpy_s(buffer, sizeof(buffer), line.c_str());

        char *context = NULL;
        char * pch;
        pch = strtok_s(buffer, " ", &context);
        beginState = atoi(pch);
        pch = strtok_s(NULL, " ", &context);
        byte = pch[0]; // Should only be one character
        pch = strtok_s(NULL, " ", &context);
        endState = atoi(pch);

        // Add char to language
        if (language.find_first_of(byte) == std::string::npos)
        {
            language += byte;
            language.append(",");
        }

        // Create beginState map if it doesn't exist
        if (stateMaps.count(beginState) == 0)
        {
            auto * temp = new std::map<char, int>;
            delMaps.push_back(temp);
            stateMaps.insert(std::pair<int, std::map<char, int>*>(beginState, temp));
        }

        // Create endState map if it doesn't exist
        if (stateMaps.count(endState) == 0)
        {
            auto * temp = new std::map<char, int>;
            delMaps.push_back(temp);
            stateMaps.insert(std::pair<int, std::map<char, int>*>(endState, temp));
        }

        // Add transition to state map
        auto iter = stateMaps.find(beginState);
        if (iter != stateMaps.end()) // Key should always be found
        {
            std::map<char, int> *temp = iter->second;
            (*temp).insert(std::pair<char, int>(byte, endState));
        }

        // Set initial state of machine
        if (count == 0)
        {
            initState = currState = beginState;
        }
    }

    file.close();
}