
#include "SExpr.h"
#include "RegexConverter.h"
#include "Lexer.h"
#include <map>
#include <regex>
#include <iostream>

// Ensure that the Token structure is not redefined
#ifndef TOKEN_STRUCT_DEFINED
#define TOKEN_STRUCT_DEFINED
struct Token {
    std::string type;
    std::string value;
    Token(const std::string& t, const std::string& v) : type(t), value(v) {}
};
#endif

int main() {
    // Define regex patterns directly
    std::map<std::string, std::regex> regexMap;

    try {
        // Multiline comments
        regexMap["comment_multiline"] = std::regex(R"((\/\*[\s\S]*?\*\/)|(\#\|[\s\S]*?\|\#)|(\"\"\"[\s\S]*?\"\"\"))");

        // Single-line comments (considering ; as a comment only if followed by something)
        regexMap["comment_singleline"] = std::regex(R"(\/\/.*|#.*|;.+)");

        // Identifiers (allowing hyphens in Racket)
        regexMap["identifier"] = std::regex(R"(\b[a-zA-Z_][a-zA-Z0-9_-]*\b)");

        // Literals (including True, False, None, #t, #f, null, integers, and decimal numbers)
        regexMap["literal"] = std::regex(R"((\".*?\"|'.*?'|\b\d+\.\d+\b|\b\d+\b|True|False|None|true|false|NULL|#t|#f|null))");


        // Operators (adjusted to avoid conflicts with identifiers containing hyphens)
        regexMap["operator"] = std::regex(R"([+\-*/=<>!&|^%~]+)");

        // Keywords
        regexMap["keyword"] = std::regex(R"(\b(if|else|while|for|def|class|import|return|void|define|list|display|print|printf)\b)");

        // Print tokens to console and store them in a vector
        std::vector<Token> tokens = removeCommentsAndStoreTokens("ejemplos.txt", regexMap);

        // Generate HTML with tokens and input file content
        generateHTMLWithTokens(tokens, "ejemplos.txt", "output.html");
    }
    catch (const std::regex_error& e) {
        std::cerr << "Regex error: " << e.what() << std::endl;
    }

    return 0;
}
