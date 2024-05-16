#ifndef LEXER_H
#define LEXER_H

#include <map>
#include <regex>
#include <string>

void printTokens(const std::string& filePath, const std::map<std::string, std::regex>& regexMap);
void removeCommentsAndPrintTokens(const std::string& filePath, const std::map<std::string, std::regex>& regexMap);

#endif // LEXER_H
