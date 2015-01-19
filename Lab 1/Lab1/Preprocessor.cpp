#include "Tokenizer.h"
#include "Preprocessor.h"

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

Preprocessor::Preprocessor()
{
}


Preprocessor::~Preprocessor()
{
}

std::string Preprocessor::run(std::string line)
{
    line = cleanComments(line);

    if (line.size() == 0)
        return "";

    // Process directives
    if (line.substr(0, 1) == "#")
    {
        
    }

    /*
    Tokenizer tokenizer;
    std::istringstream iss(line);

    // Line will have at least one token, check if it's a preprocessor type
    std::string temp;
    iss >> temp;
    auto iter = tokenizer.getPair(temp);
    if (iter.second == Tokenizer::Directive)
    {
        Directive directive = getDirective(iter.first);

        // Try to match first token to directive
        // if directive, try to match a symbol to second token
        // if #define, add symbol to table
        // if conditional, set conditional = true, continue

        switch (directive)
        {
            case Definition:
                // Add symbol and return "", or stop processing if symbol already exists

                if (symbolTable.find())


                    break;
            case IfNotDefined:
                if (!ignoreBlocks)
                {
                    // If not defined, inClearMode = false, and return ""
                }
                break;
            case IfDefined:
                if (!ignoreBlocks)
                {

                }
                ignoreBlock = ignoreBlocks = false;
                // If defined, inClearMode = false, and return ""
                break;
            case Else:
                if (!ignoreBlock && !ignoreBlocks)
                {

                }

                // Always process this
                break;
            case EndIf:
                ignoreBlock = ignoreBlocks = false;
                break;
            default:
            {
                // Remove inline comments
                       size_t pos = line.find_first_of("//");
                       if (pos != line.end())
                       {
                           line = line.substr(0, pos);
                       }

                // Tokenize line and replace symbols, detect //'s and remove remainder of tokens
                auto tokenized = tokenizer.tokenize(result);


                       // Check for symbol token that needs to be replaced
                       // Also check for //'s and remove what needs to be removed
            }
        }
    }
    */

    return line;
}

Preprocessor::Directive Preprocessor::getDirective(std::string token)
{
    if (token == "#define")
        return Preprocessor::Definition;
    else if (token == "#ifndef")
        return Preprocessor::IfNotDefined;
    else if (token == "#ifdef")
        return Preprocessor::IfDefined;
    else if (token == "#else")
        return Preprocessor::Else;
    else // (token == "#endif")
        return Preprocessor::EndIf;
}

std::string Preprocessor::cleanComments(std::string line)
{
    while (line.find("/*") != std::string::npos)
    {
        size_t pos = line.find("/*");
        line.erase(pos, (line.find("*/", pos) - pos) + 2);
    }
    while (line.find("//") != std::string::npos)
    {
        size_t pos = line.find("//");
        line.erase(pos, line.find("\n", pos) - pos);
    }

    return line;
}