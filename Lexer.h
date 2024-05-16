#ifndef LEXER_H
#define LEXER_H

#include <map>
#include <regex>
#include <string>

void removeCommentsAndPrintTokens(const std::string& filePath, const std::map<std::string, std::regex>& regexMap);
void generateHTML(const std::string& inputFilePath, const std::string& outputFilePath, const std::map<std::string, std::regex>& regexMap);

#endif // LEXER_H
