// Lexer.h
#ifndef LEXER_H
#define LEXER_H

#include <map>
#include <regex>
#include <string>
#include <vector>

// Estructura para almacenar los tokens
struct Token {
    std::string type;
    std::string value;
};

void removeCommentsAndPrintTokens(const std::string& filePath, const std::map<std::string, std::regex>& regexMap);
std::vector<Token> removeCommentsAndStoreTokens(const std::string& filePath, const std::map<std::string, std::regex>& regexMap);

#endif // LEXER_H
