#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <map>
#include <regex>

struct Token {
    std::string type;
    std::string value;

    Token(const std::string& type, const std::string& value) : type(type), value(value) {}
};

std::vector<Token> removeCommentsAndStoreTokens(const std::string& filePath, const std::map<std::string, std::regex>& regexMap, std::vector<std::pair<int, std::string>>& syntaxErrors);

void generateHTMLWithTokens(const std::vector<Token>& tokens, const std::string& inputFilePath, const std::string& outputFilePath, const std::vector<std::pair<int, std::string>>& syntaxErrors);

#endif // LEXER_H
