#pragma once
#include <string>
#include <vector>
#include <map>
#include <regex>

struct Token {
    std::string type;
    std::string value;

    Token(const std::string& type, const std::string& value) : type(type), value(value) {}
};

void removeCommentsAndPrintTokens(const std::string& filePath, const std::map<std::string, std::regex>& regexMap);
std::vector<Token> removeCommentsAndStoreTokens(const std::string& filePath, const std::map<std::string, std::regex>& regexMap);
void generateHTMLWithTokens(const std::vector<Token>& tokens, const std::string& inputFilePath, const std::string& outputFilePath);
