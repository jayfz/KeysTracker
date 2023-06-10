#ifndef ARGUMENT_PARSER_H
#define ARGUMENT_PARSER_H

#include <cstdint>
#include <string>
#include <unordered_map>
class ArgumentParser
{
public:
    ArgumentParser(uint32_t argumentCount, char **argumentList);
    template <typename T, typename std::enable_if<std::is_same<T, std::string>::value ||
                                                      std::is_same<T, double>::value ||
                                                      std::is_same<T, int>::value,
                                                  int>::type = 0>
    T getOptionOrDefault(std::string option, T defaultTo) const;

private:
    std::unordered_map<std::string, std::string> argumentMap;
};
#endif