#include "Preprocessor.h"
#include "Tokenizer.h"
//#include "SymbolTable.h" // Includes Symbol.h

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

    line = replaceSymbols(line);

    if (line.substr(0, 1) == "#")
    {
        processDirective(line);
        return "";
    }

    return emptyLine ? "" : line;
}

std::string Preprocessor::cleanComments(std::string line)
{
    // Check for close comment
    if (inCommentBlock)
    {
        size_t pos = line.find("*/");
        if (pos == std::string::npos)
        {
            return "";
        }
        else
        {
            line.erase(0, pos + 2); // Erase commented portion
        }
        inCommentBlock = false;
        cleanComments(line); // Clean remaining comments
    }

    while (line.find("/*") != std::string::npos)
    {
        size_t pos = line.find("/*");
        size_t end = line.find("*/");
        if (end == std::string::npos)
        {
            inCommentBlock = true;
            line.erase(pos, line.find("\n", pos) - pos);
        }
        else
        {
            line.erase(pos, (line.find("*/", pos) - pos) + 2);
        }        
    }
    while (line.find("//") != std::string::npos)
    {
        size_t pos = line.find("//");
        line.erase(pos, line.find("\n", pos) - pos);
    }

    return line;
}

std::string Preprocessor::replaceSymbols(std::string line)
{
    Tokenizer tokenizer;
    auto tokens = tokenizer.tokenize(line);

    // Find and replace symbols in line
    for (auto token : tokens)
    {
        if (token.type() == tokenizer.Identifier)
        {
            // Check if value exists in symbol table
            auto symbol = symbolTable.find(token.value());

            if (symbol != nullptr)
            {
                // Replace symbol with value
                line.replace(token.pos(), token.value().length(), symbol->getName());
            }

        }
    }

    return line;
}

void Preprocessor::processDirective(std::string line)
{
    Tokenizer tokenizer;
    tokenizer.tokenize(line);

    // We'll tokenize using default space delimiter for simplicity
    std::istringstream iss(line);
    string token[3];
    for (int idx = 0; iss >> token[idx] && idx < 3; ++idx) {}

    if (!token[0].empty()) // Directive
    {
        if (token[0] == "#define")
        {
            if (!token[1].empty()) // Name
            {
                // Verify token is identifier
                Tokenizer::Token temp_token = tokenizer.getToken(line.find(token[1]), line);

                if (temp_token.type() != tokenizer.Identifier)
                {
                    return; // Or terminate program somehow
                }
                else
                {
                    cout << "Found definition for symbol: " << temp_token.value() << endl;
                }
                    

                // Create symbol
                // Symbol(std::string name, Type type, Use use, std::string value);
                // TODO: Identify symbol type using "bool" keyword, int, float, and string identifiers.
                bool result = symbolTable.add(Symbol(token[1], Symbol::String, Symbol::Use::VariableName, token[2]));

                if (!result)
                {
                    // Exit program somehow, no redefinition allowed
                }
            }
        }
        else if (token[0] == "#endif")
        {
            doElse = emptyLine = false;
        }
        else if (token[0] == "#else")
        {
            if (!doElse)
            {
                emptyLine = true;
            } 
        }
        else
        {
            if (!token[1].empty())
            {
                if (token[0] == "#ifndef")
                {
                    // If symbol IS defined, emptyLine = true, doElse = true
                    if (symbolTable.find(token[1]))
                    {
                        emptyLine = doElse = true;
                    }
                }
                else if (token[0] == "#ifdef")
                {
                    // If symbol IS NOT defined, emptyLine = true, doElse = true
                    if (!symbolTable.find(token[1]))
                    {
                        emptyLine = doElse = true;
                    }
                }
            }
            else if (token[0] == "#else")
            {
                if (doElse)
                    emptyLine = false;
            }
        }
    }

    cout << "Processed directive, searching for symbol..." << endl;

    bool hasValue = symbolTable.find(token[2]);
    if (hasValue)
    {
        cout << "Symbol table contains value: " << token[2] << endl;
    }
    else
    {
        cout << "Symbol table has no entries." << endl;
    }
}

bool Preprocessor::stringStartsWith(std::string line, std::string token)
{
    return line.compare(0, token.length(), token) == 0;
}