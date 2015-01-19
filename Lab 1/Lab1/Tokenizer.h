/***********************************************************
* Author:                   Logan Wright
* Date Created:             12/01/2014
* Last Modification Date:   12/XX/2014
* Lab Number:               CST 229 Lab 2
* Filename:                 Tokenizer.h
*
* Overview:
*   This program is an extension to Lab 1's state machine.
*   It tokenizes a text file containing programming
*   language code (C++ in this case), and identifies each
*   token as a symbol, operator, keyword, identifier, etc.
*   A token may not match any expected type, in which case
*   it will be marked as invalid.
*
* Input:
*   The user must provide a text file name containing
*   the program code, such as: code1.txt
*
* Output:
*   The program will analyze the file and display all
*   tokens found with a description.
*
*       Example:    Token               Token Type
*                   for                 Keyword
*                   while               Keyword
*                   _test               Identifier
*                   @                   Invalid
*                   12                  Integer
************************************************************/

#pragma once

#include <map>
#include <vector>

class Tokenizer
{

    public:

        enum TokenType
        {
            start,
            Directive,
            Integer,
            Float,
            Keyword,
            Identifier,
            CharSymbol,
            Operator,
            Boolean,
            Invalid,
            end
        };

        Tokenizer();
        ~Tokenizer();

        /**********************************************************************
        * vector<pair<string, TokenType>> Tokenize(string inStr);
        *
        * Purpose: This function takes an input string, which ordinarily
        * will be a line from a file, and splits it into tokens.
        *
        * Entry: inStr is a line of text containing keywords, identifiers,
        * and other C++ language components.
        *
        * Exit: A vector containing pairs of token strings and TokenType
        * enums.
        *
        * string EnumToString(TokenType type);
        *
        * Purpose: This function is mainly for debug purposes. It prints
        * the enum TokenType as a string representation.
        *
        * Entry: A TokenType.
        *
        * Exit: A string representation of the TokenType.
        *
        * pair<string, TokenType> GetPair(string inToken);
        *
        * Purpose: Turns a string token into a string-TokenType pair.
        *
        * Entry: A string token.
        *
        * Exit: A new string-TokenType pair.
        *
        ***********************************************************************/

        std::vector<std::pair<std::string, TokenType>> tokenize(std::string inStr);
        std::pair<std::string, TokenType> getPair(std::string inToken);

        std::string enumToString(TokenType type);

    private:

        // Contains string tokens that are common to language, with an associated TokenType.
        std::map<std::string, Tokenizer::TokenType> tokenMatcher;

        // Used to split code strings and discover tokens in language files
        std::string delimiters = " \t\n";

};