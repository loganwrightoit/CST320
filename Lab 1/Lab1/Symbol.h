#pragma once

#include <string>

class Symbol
{

    public:

        enum Type
        {
            Bool,
            Char,
            Short,
            Int,
            Long,
            Float,
            Double,
            String
        };

        enum Use
        {
            VariableName,
            Constant,
            FunctionName,
            ClassName,
            TypeDef
        };

        Symbol(std::string name, Type type, Use use, std::string value);
        ~Symbol();

        std::string getName() const;
        Type getType() const;
        Use getUse() const;
        std::string getValue() const;

    private:

        std::string _name;
        Type _type;
        Use _use;
        std::string _value; // For constants

};