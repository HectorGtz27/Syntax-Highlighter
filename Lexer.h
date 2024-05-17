#pragma once
#include <string>
#include <map>
#include <regex>
#include <vector>

// Define the Token structure
struct Token {
    std::string type;
    std::string value;

    Token(const std::string& t, const std::string& v) : type(t), value(v) {}
};

// Function declarations
std::vector<Token> removeCommentsAndStoreTokens(const std::string& filePath, const std::map<std::string, std::regex>& regexMap);
void generateHTMLWithTokens(const std::vector<Token>& tokens, const std::string& inputFilePath, const std::string& outputFilePath);
