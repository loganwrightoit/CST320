/***********************************************************
* Author:                   Logan Wright
* Date Created:             01/19/2015
* Last Modification Date:   01/20/2015
* Lab Number:               CST 320 Lab 1
* Filename:                 Token.cpp
*
* Overview:
*   Stores information about tokens used with Tokenizer.cpp.
*   Information includes the starting position of the token
*   in the file line, the token type, and a value.
*
* Input:
*   None.
*
* Output:
*   None.
************************************************************/

#include "Tokenizer.h"

Tokenizer::Token::Token(size_t pos, Tokenizer::TokenType type, std::string value)
    : _pos(pos), _type(type), _value(value) {}

Tokenizer::Token::~Token()
{
}

/**************************************************************
*   Entry:  None.
*
*    Exit:  A size_t position in originating line of code text
*           file.
*
* Purpose:  Returns the position relative to beginning of
*           tokenized line.
***************************************************************/
size_t Tokenizer::Token::pos()
{
    return _pos;
}

/**************************************************************
*   Entry:  A TokenType value.
*
*    Exit:  None.
*
* Purpose:  Sets the token type.
***************************************************************/
void Tokenizer::Token::setType(Tokenizer::TokenType type)
{
    _type = type;
}

/**************************************************************
*   Entry:  None.
*
*    Exit:  A TokenType value.
*
* Purpose:  Returns the TokenType.
***************************************************************/
Tokenizer::TokenType Tokenizer::Token::type()
{
    return _type;
}

/**************************************************************
*   Entry:  None.
*
*    Exit:  A std::string.
*
* Purpose:  Returns the token value.
***************************************************************/
std::string Tokenizer::Token::value()
{
    return _value;
}