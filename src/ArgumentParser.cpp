#include "ArgumentParser.h"
#include <regex>
#include <iostream>

ArgumentParser::ArgumentParser(uint32_t argumentCount, char **argumentList)
{
    std::regex longOptionRegex(R"(--(\w+)=([\w-]+))");

    for (int i = 1; i < argumentCount; ++i)
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

template <typename T, typename std::enable_if<std::is_same<T, std::string>::value ||
                                                  std::is_same<T, double>::value ||
                                                  std::is_same<T, int>::value,
                                              int>::type>

T ArgumentParser::getOptionOrDefault(std::string option, T defaultTo) const
{
    auto resultIterator = this->argumentMap.find(option);

    if (resultIterator != myMap.end())
    {
        if (std::is_same<T, std::string>::value)
        {
            return resultIterator->second;
        }
        if (std::is_same<T, int>::value)
        {
            return std::stoi(resultIterator->second);
        }
        if (std::is_same<T, double>::value)
        {
            return std::stod(resultIterator->second);
        }
    }

    return defaultTo;
}
