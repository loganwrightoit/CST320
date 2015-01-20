#pragma once

#include "SymbolTable.h" // Includes Symbol.h

//#include <string>

class Preprocessor
{

    public:

        enum Directive
        {
            Definition,
            IfNotDefined,
            IfDefined,
            Else,
            EndIf,
            None
        };

        Preprocessor();
        ~Preprocessor();

        // Runs preprocessor routines on line and returns new line
        std::string run(std::string line);

    private:

        // Tests whether line starts with given token
        bool stringStartsWith(std::string line, std::string token);

        // Removes comments from line and returns new line
        std::string cleanComments(std::string line);

        // Replaces symbols in line
        std::string Preprocessor::replaceSymbols(std::string line);

        // Handles directives
        void Preprocessor::processDirective(std::string line);

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

