#include "LL1_TableParser.h"

#include <fstream>
#include <iostream>
#include <sstream>

LL1_TableParser::LL1_TableParser(std::string fileName)
{
    std::ifstream file;

    // Open file
    file.open(fileName);
    if (!file.is_open()) {
        std::cerr << "LL1_TableParser: could not open file " << fileName << std::endl;
        return;
    }

    // Build table
    std::string line;
    while (std::getline(file, line))
    {
        if (line.size() > 1 && line.substr(0, 2) == "//")
        {
            std::cout << "LL1_TableParser: skipping comment line" << std::endl;
            continue;
        }
        else if (std::count(line.begin(), line.end(), ' ') > 1)
        {
            // Split line into tokens
            std::istringstream buf(line);
            std::istream_iterator<std::string> beg(buf), end;
            std::vector<std::string> tokens(beg, end);

            if (tokens.size() > 2)
            {
                // Extract rule and id
                std::string rule = tokens.at(0);
                std::string id = tokens.at(1);

                // Extract tokens for parsing
                std::vector<std::string> tkns(tokens.begin() + 2, tokens.end());

                auto rule_map = table.find(rule);
                if (rule_map != table.end())
                {
                    // Rule exists, just need to add token and tokens
                    auto result = rule_map->second.insert(std::make_pair(id, tkns));
                    if (!result.second)
                    {
                        std::cerr << "LL1_TableParser: table cannot contain two entries per cell" << std::endl;
                        return;
                    }
                }
                else
                {
                    // Rule does not exist, need to add rule token and tokens together
                    std::map<std::string, std::vector<std::string>> new_map;
                    new_map.insert(std::make_pair(id, tkns));
                    table.insert(std::make_pair(rule, new_map));
                }

                // Output results of table insertion
                std::cout << "LL1_TableParser: " << tokens.at(0).c_str() << " -> " << tokens.at(1).c_str() << " -> ";
                for (int idx = 0; idx < tkns.size(); ++idx)
                {
                    std::cout << tkns.at(idx).c_str() << " ";
                }
                std::cout << std::endl;
            }
            else
            {
                std::cerr << "LL1_TableParser: encountered malformed grammar rule" << std::endl;
                return;
            }
        }
        else
        {
            std::cout << "LL1_TableParser: skipping line of invalid length" << std::endl;
        }        
    }
}

LL1_TableParser::~LL1_TableParser()
{
}

bool LL1_TableParser::parse(std::vector<Tokenizer::Token>)
{

    return true;
}