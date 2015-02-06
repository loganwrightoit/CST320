/***********************************************************
* Author:                   Logan Wright
* Date Created:             01/16/2015
* Last Modification Date:   01/20/2015
* Lab Number:               CST 320 Lab 1
* Filename:                 Tokenizer.h
*
* Overview:
*   Tokenizes a code file using C++ token types.
*
* Input:
*   None.
*
* Output:
*   None.
************************************************************/

#pragma once

#include <map>
#include <vector>

class Tokenizer
{

    public:

        enum TokenType
        {
            begin = 0,
            Directive,
            Integer,
            Float,
            Keyword,
            Identifier,
            Symbol,
            SymbolPair,
            Input,
            Output,
            Operator,
            OperatorPair,
            Boolean,
            String,
            Invalid,
            end
        };

        class Token
        {

            public:

                Token(size_t pos, TokenType type, std::string value);
                ~Token();

                /**********************************************************************
                *   size_t Tokenizer::Token::pos()
                *
                *      Purpose: Returns the position relative to beginning of
                *               tokenized line.
                *
                *        Entry: None.
                *
                *         Exit: A size_t position in originating line of code text
                *               file.
                *
                *   void Tokenizer::Token::setType(Tokenizer::TokenType type)
                *
                *      Purpose: Sets the token type.
                *
                *        Entry: A TokenType value.
                *
                *         Exit: None.
                *
                *   Tokenizer::TokenType Tokenizer::Token::type()
                *
                *      Purpose: Returns the TokenType.
                *
                *        Entry: None.
                *
                *         Exit: A TokenType value.
                *
                *   std::string Tokenizer::Token::value()
                *
                *      Purpose: Returns the token value.
                *
                *        Entry: None.
                *
                *         Exit: A std::string.
                ***********************************************************************/

                size_t pos();
                void setType(Tokenizer::TokenType type);
                TokenType type();
                std::string value();

            private:

                // Start position of token in line
                size_t _pos;

                // Type of token
                TokenType _type;

                // Value of token
                std::string _value;

        };

        Tokenizer();
        ~Tokenizer();

        /**********************************************************************
        *   vector<pair<string, TokenType>> Tokenize(string inStr)
        *
        *      Purpose: This function takes an input string, which ordinarily
        *               will be a line from a file, and splits it into tokens.
        *
        *        Entry: inStr is a line of text containing keywords, identifiers,
        *               and other C++ language components.
        *
        *         Exit: A vector containing pairs of token strings and TokenType
        *               enums.
        *
        *   string EnumToString(TokenType type)
        *
        *      Purpose: This function is mainly for debug purposes. It prints
        *               the enum TokenType as a string representation.
        *
        *        Entry: A TokenType.
        *
        *         Exit: A string representation of the TokenType.
        *
        *   pair<string, TokenType> GetPair(string inToken)
        *
        *      Purpose: Turns a string token into a string-TokenType pair.
        *
        *        Entry: A string token.
        *
        *         Exit: A new string-TokenType pair.
        ***********************************************************************/

        std::vector<Token> tokenize(std::string inStr);
        Token getToken(size_t pos, std::string inToken);

        std::string enumToString(TokenType type);

    private:

        // Contains predefined string tokens that are common to language, with an associated TokenType.
        std::map<std::string, Tokenizer::TokenType> tokenMatcher;

        // Used to split code strings and discover tokens in language files
        std::string delimiters = " \t\n";

};