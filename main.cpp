#include "SExpr.h"
#include "RegexConverter.h"
#include "Lexer.h"
#include <map>
#include <regex>
#include <iostream>

bool validateRegex(const std::string& regexStr) {
    try {
        std::regex regex(regexStr);
    }
    catch (const std::regex_error& e) {
        std::cerr << "Invalid regex: " << regexStr << " - " << e.what() << std::endl;
        return false;
    }
    return true;
}

int main() {
    SExpr reservedWords("or", {
        SExpr("lit", "if"),
        SExpr("lit", "else"),
        SExpr("lit", "while"),
        SExpr("lit", "for"),
        SExpr("lit", "def"),
        SExpr("lit", "class"),
        SExpr("lit", "import"),
        SExpr("lit", "return")
        });

    SExpr operators("or", {
        SExpr("lit", "\\+"),
        SExpr("lit", "-"),
        SExpr("lit", "\\*"),
        SExpr("lit", "/"),
        SExpr("lit", "%"),
        SExpr("lit", "=="),
        SExpr("lit", "!="),
        SExpr("lit", ">"),
        SExpr("lit", "<"),
        SExpr("lit", ">="),
        SExpr("lit", "<="),
        SExpr("lit", "and"),
        SExpr("lit", "or"),
        SExpr("lit", "not"),
        SExpr("lit", "="),
        SExpr("lit", "\\+="),
        SExpr("lit", "-="),
        SExpr("lit", "\\*="),
        SExpr("lit", "/="),
        SExpr("lit", "%="),
        SExpr("lit", "//"),
        SExpr("lit", "//="),
        SExpr("lit", "\\*\\*"),
        SExpr("lit", "\\*\\*=")
        });

    SExpr literals("or", {
        SExpr("lit", "123"),
        SExpr("lit", "3\\.14"),
        SExpr("lit", "\"hello\""),
        SExpr("lit", "'world'"),
        SExpr("lit", "True"),
        SExpr("lit", "False"),
        SExpr("lit", "\\[1, 2, 3\\]"),
        SExpr("lit", "\\{\\\"key\\\": \\\"value\\\"\\}")
        });

    SExpr comments("or", {
        SExpr("lit", "#.*"),  // Single-line comment in Python
        SExpr("lit", "\"\"\"[\\s\\S]*?\"\"\"")  // Multi-line comment in Python
        });

    SExpr identifiers("lit", "[a-zA-Z_][a-zA-Z0-9_]*");  // Identificadores

    std::map<std::string, std::regex> regexMap;

    // Convert and validate regex
    std::string reservedWordsRegex = translateSExprToRegex(reservedWords);
    if (validateRegex(reservedWordsRegex)) {
        regexMap["reserved-word"] = std::regex(reservedWordsRegex);
    }

    std::string operatorsRegex = translateSExprToRegex(operators);
    if (validateRegex(operatorsRegex)) {
        regexMap["operator"] = std::regex(operatorsRegex);
    }

    std::string literalsRegex = translateSExprToRegex(literals);
    if (validateRegex(literalsRegex)) {
        regexMap["literal"] = std::regex(literalsRegex);
    }

    std::string commentsRegex = translateSExprToRegex(comments);
    if (validateRegex(commentsRegex)) {
        regexMap["comment"] = std::regex(commentsRegex);
    }

    std::string identifiersRegex = translateSExprToRegex(identifiers);
    if (validateRegex(identifiersRegex)) {
        regexMap["identifier"] = std::regex(identifiersRegex);
    }

    // Imprimir los tokens en consola
    printTokens("test.py", regexMap);

    return 0;
}
