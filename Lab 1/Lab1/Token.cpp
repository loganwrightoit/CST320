#include "Tokenizer.h"

//#include <string>

Tokenizer::Token::Token(size_t pos, Tokenizer::TokenType type, std::string value)
    : _pos(pos), _type(type), _value(value) {}

Tokenizer::Token::~Token()
{
}

// Returns start position of token in line
size_t Tokenizer::Token::pos()
{
    return _pos;
}

void Tokenizer::Token::setType(Tokenizer::TokenType type)
{
    _type = type;
}

// Returns token type
Tokenizer::TokenType Tokenizer::Token::type()
{
    return _type;
}

// Returns token value
std::string Tokenizer::Token::value()
{
    return _value;
}