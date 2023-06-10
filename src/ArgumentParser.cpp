#include "ArgumentParser.h"
#include <regex>
#include <iostream>

ArgumentParser::ArgumentParser(uint32_t argumentCount, char **argumentList)
{
    std::regex longOptionRegex(R"(--(\S+)=(\S+))");

    for (uint32_t i = 1; i < argumentCount; ++i)
    {
        std::string argument = argumentList[i];
        std::smatch match;
        if (std::regex_match(argument, match, longOptionRegex))
        {
            this->argumentMap.insert({match[1], match[2]});
        }
        else
        {
            std::cout << "not recognized argument: " << argument << std::endl;
        }
    }
}
