#ifndef LEXER_H
#define LEXER_H

#include <map>
#include <regex>
#include <string>
#include <vector>

// Ensure that the Token structure is not redefined
#ifndef TOKEN_STRUCT_DEFINED
#define TOKEN_STRUCT_DEFINED
struct Token {
    std::string type;
    std::string value;
    Token(const std::string& t, const std::string& v) : type(t), value(v) {}
};
#endif

// Function to remove comments, print tokens, and store tokens
std::vector<Token> removeCommentsAndStoreTokens(const std::string& filePath, const std::map<std::string, std::regex>& regexMap);

// Function to generate HTML with the tokens and the input file content
void generateHTMLWithTokens(const std::vector<Token>& tokens, const std::string& inputFilePath, const std::string& outputFilePath);

#endif // LEXER_H
