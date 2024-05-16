#include "SExpr.h"
#include "RegexConverter.h"
#include "Lexer.h"
#include <map>
#include <regex>
#include <iostream>

int main() {
    std::map<std::string, std::regex> regexMap;

    try {
        regexMap["comment_multiline"] = std::regex(R"((\/\*[\s\S]*?\*\/)|(\#\|[\s\S]*?\|\#)|(\"\"\"[\s\S]*?\"\"\"))");
        regexMap["comment_singleline"] = std::regex(R"(\/\/.*|#.*|;.+)");
        regexMap["identifier"] = std::regex(R"(\b[a-zA-Z_][a-zA-Z0-9_-]*\b)");
        regexMap["literal"] = std::regex(R"((\".*?\")|('.*?')|\b\d+\b|True|False|None|true|false|NULL|#t|#f|null)");
        regexMap["operator"] = std::regex(R"([+\-*/=<>!&|^%~]+)");
        regexMap["keyword"] = std::regex(R"(\b(if|else|while|for|def|class|import|return|void|define|list|display)\b)");

        // Tokenizar y mostrar tokens en consola
        removeCommentsAndPrintTokens("ejemplos.txt", regexMap);

        // Tokenizar y almacenar tokens
        auto tokens = removeCommentsAndStoreTokens("ejemplos.txt", regexMap);

        // Generar archivo HTML con tokens
        generateHTMLWithTokens(tokens, "ejemplos.txt", "output.html");
    }
    catch (const std::regex_error& e) {
        std::cerr << "Regex error: " << e.what() << std::endl;
    }

    return 0;
}
