#pragma once

#include "SymbolTable.h" // Includes Symbol.h

#include "Tokenizer.h"

class Preprocessor
{

    public:

        enum Directive
        {
            Define,
            IfNotDefined,
            IfDefined,
            Else,
            EndIf
        };

        Preprocessor();
        ~Preprocessor();

        // Runs preprocessor routines on line and returns new line
        std::string run(std::string line);

    private:

        // Adds symbol and returns true if successful
        bool addSymbol(Tokenizer::Token token, std::string value = "");

        // Tests whether line starts with given token
        bool stringStartsWith(std::string line, std::string token);

        // Removes comments from line and returns new line
        std::string cleanComments(std::string line);

        // Replaces symbols in line
        std::string replaceSymbols(std::string line);

        // Handles directives
        void processDirective(std::string line);

        // Gets directive of line
        Directive getDirective(std::string token);

        // Indicates preprocessor is within a comment block
        bool inCommentBlock = false;

        // Indicates a line without a directive needs to be emptied
        bool emptyLine = false;

        // Whether preprocessor should handle next #else directive
        bool doElse = false;

        SymbolTable symbolTable;

};

