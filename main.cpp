#include "SExpr.h"
#include "RegexConverter.h"
#include "Lexer.h"
#include <map>
#include <regex>
#include <iostream>

int main() {
    // Definir expresiones regulares directamente
    std::map<std::string, std::regex> regexMap;

    // Comentarios de varias líneas
    regexMap["comment_multiline"] = std::regex("\"\"\"[\\s\\S]*?\"\"\"");

    // Comentarios de una línea
    regexMap["comment_singleline"] = std::regex("#.*");

    // Identificadores
    regexMap["identifier"] = std::regex("\\b[a-zA-Z_][a-zA-Z0-9_]*\\b");

    // Literales
    regexMap["literal"] = std::regex("\".*?\"|'.*?'|\\b\\d+\\b");

    // Operadores
    regexMap["operator"] = std::regex("[+\\-*/=<>!&|^%~]+");

    // Palabras clave
    regexMap["keyword"] = std::regex("\\b(if|else|while|for|def|class|import|return|True|False|None)\\b");

    // Imprimir los tokens en consola
    removeCommentsAndPrintTokens("test.py", regexMap);

    return 0;
}
