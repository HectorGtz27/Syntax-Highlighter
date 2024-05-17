#include "Lexer.h"
#include <map>
#include <regex>
#include <iostream>

int main() {
    // Define regex patterns
    std::map<std::string, std::regex> regexMap;

    try {
        // Multiline comments
        regexMap["comment_multiline"] = std::regex(R"((\/\*[\s\S]*?\*\/)|(\#\|[\s\S]*?\|\#)|(\"\"\"[\s\S]*?\"\"\"))");

        // Single-line comments
        regexMap["comment_singleline"] = std::regex(R"(\/\/.*|#.*)");

        // Identifiers
        regexMap["identifier"] = std::regex(R"(\b[a-zA-Z_][a-zA-Z0-9_]*\b)");

        // Literals (including True, False, None, #t, #f, NULL, integers, and floating-point numbers)
        regexMap["literal"] = std::regex(R"((\".*?\")|('.*?')|\b\d+\.\d+\b|\b\d+\b|True|False|None|true|false|NULL|#t|#f)");  // Corrected

        // Keywords
        regexMap["keyword"] = std::regex(R"(\b(if|else|while|for|def|class|import|return|void|define|list|display|print)\b)");

        // Operators
        regexMap["operator"] = std::regex(R"([+\-*/=<>!&|^%~]+)");

        // Special characters
        regexMap["special_character"] = std::regex(R"([\(\)\{\}\[\],;:])");  // Removed .

        // Remove comments and store tokens
        std::vector<Token> tokens = removeCommentsAndStoreTokens("ejemplos.txt", regexMap);

        // Generate HTML with highlighted tokens
        generateHTMLWithTokens(tokens, "ejemplos.txt", "output.html");
    }
    catch (const std::regex_error& e) {
        std::cerr << "Regex error: " << e.what() << std::endl;
    }

    return 0;
}
