#include "Preprocessor.h"
#include "Tokenizer.h"

#include <iostream>
#include <string>
#include <sstream>
#include <assert.h>

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

    // Check whether line should be tossed due to conditional state
    // This isn't complete, but it works well enough
    if (stringStartsWith(line, "#define") || !stringStartsWith(line, "#"))
    {
        if (emptyLine)
            return "";
    }

    if (line.substr(0, 1) == "#")
    {
        processDirective(line);
        return "";
    }

    line = replaceSymbols(line);

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
            auto symbol = symbolTable.find(token.value());
            if (symbol != nullptr)
            {
                if (!symbol->getValue().empty())
                {
                    std::string value = symbol->getValue();

                    // If value is string, put in quotes
                    if (value.find(" ") != string::npos)
                    {
                        value = "\"" + value + "\"";
                    }
                    
                    // Replace symbol with value
                    line.replace(token.pos(), token.value().length(), value);
                }
            }

        }
    }

    return line;
}

void Preprocessor::processDirective(std::string line)
{
    Tokenizer tokenizer;
    auto tokens = tokenizer.tokenize(line);

    switch (getDirective(tokens.at(0).value()))
    {
        case Define:
            assert(tokens.size() == 2 || tokens.size() == 3);
            if (tokens.size() == 3)
                assert(addSymbol(tokens.at(1), tokens.at(2).value()));
            else
                assert(addSymbol(tokens.at(1)));
            break;
        case IfNotDefined:
            assert(tokens.size() == 2);
            if (symbolTable.contains(tokens.at(1).value()))
                doElse = emptyLine = true;
            break;
        case IfDefined:
            assert(tokens.size() == 2);
            if (!symbolTable.contains(tokens.at(1).value()))
                doElse = emptyLine = true;
            break;
        case Else:
            emptyLine = !doElse;
            break;
        case EndIf:
            doElse = emptyLine = false;
            break;
    }
}

bool Preprocessor::addSymbol(Tokenizer::Token token, std::string value)
{
    assert(token.type() == Tokenizer::Identifier);
    Symbol symbol(token.value(), Symbol::String, Symbol::Use::VariableName, value);
    return symbolTable.add(symbol);
}

bool Preprocessor::stringStartsWith(std::string line, std::string token)
{
    return line.compare(0, token.length(), token) == 0;
}

Preprocessor::Directive Preprocessor::getDirective(std::string name)
{
    if (name == "#define")
        return Define;
    else if (name == "#ifndef")
        return IfNotDefined;
    else if (name == "#ifdef")
        return IfDefined;
    else if (name == "#else")
        return Else;
    else
        return EndIf;
}