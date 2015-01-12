#include "Symbol.h"


Symbol::Symbol(std::string name, Type type, Use use, std::string value) :
    _name(name), _type(type), _use(use), _value(value) {}

Symbol::~Symbol()
{
}

std::string Symbol::getName() const
{
    return _name;
};

Symbol::Type Symbol::getType() const
{
    return _type;
};

Symbol::Use Symbol::getUse() const
{
    return _use;
};

std::string Symbol::getValue() const
{
    return _value;
};