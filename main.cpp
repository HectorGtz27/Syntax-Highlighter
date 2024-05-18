#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include "Lexer.h"
#include "SExpr.h"
#include "RegexConverter.h"

int main() {
    std::map<std::string, std::regex> regexMap;
    regexMap["comment_multiline"] = std::regex(R"((\/\*[\s\S]*?\*\/)|(\#\|[\s\S]*?\|\#)|(\"\"\"[\s\S]*?\"\"\"))");
    regexMap["comment_singleline"] = std::regex(R"(\/\/.*|#.*)");
    regexMap["identifier"] = std::regex(R"(\b[a-zA-Z_][a-zA-Z0-9_]*\b)");
    regexMap["literal"] = std::regex(R"((\".*?\")|('.*?')|\b\d+\.\d+\b|\b\d+\b|True|False|None|true|false|NULL|#t|#f)");
    regexMap["keyword"] = std::regex(R"(\b(if|else|while|for|def|class|import|return|void|define|list|display|print)\b)");
    regexMap["operator"] = std::regex(R"([+\-*/=<>!&|^%~]+)");
    regexMap["special_character"] = std::regex(R"([\(\)\{\}\[\],;:])");

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<Token> tokens = removeCommentsAndStoreTokens("ejemplos.txt", regexMap);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Time to remove comments and store tokens: " << diff.count() << " s\n";

    start = std::chrono::high_resolution_clock::now();
    generateHTMLWithTokens(tokens, "ejemplos.txt", "output.html");
    end = std::chrono::high_resolution_clock::now();
    diff = end - start;
    std::cout << "Time to generate HTML with tokens: " << diff.count() << " s\n";

    SExpr sexpr("concat", { SExpr("lit", "a"), SExpr("star", {SExpr("lit", "b")}) });
    start = std::chrono::high_resolution_clock::now();
    std::string regex = translateSExprToRegex(sexpr);
    end = std::chrono::high_resolution_clock::now();
    diff = end - start;
    std::cout << "Time to translate SExpr to regex: " << diff.count() << " s\n";

    std::string str = "a*b+c";
    start = std::chrono::high_resolution_clock::now();
    std::string escapedStr = escapeSpecialCharacters(str); // Esta línea usa la función
    end = std::chrono::high_resolution_clock::now();
    diff = end - start;
    std::cout << "Time to escape special characters: " << diff.count() << " s\n";

    return 0;
}
