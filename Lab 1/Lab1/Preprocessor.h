#pragma once

#include <string>

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

        // Removes comments from line and returns new line
        std::string cleanComments(std::string line);

        // Gets directive of line
        Directive getDirective(std::string token);

        // Indicates preprocessor is within a comment block
        bool inCommentBlock;

        // Indicates preprocessor is within a conditional block
        bool inConditional;

        // Indicates conditional block is satisfied, ignore everything until #endif
        bool satisfiedConditional;

        // Indicates conditional block should be ignored, continue until new directive
        bool skipBlock;

};

