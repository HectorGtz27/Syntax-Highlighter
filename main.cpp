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
    regexMap["comment"] = std::regex("\"\"\"[\\s\\S]*?\"\"\"");

    // Comentarios de una línea
    regexMap["comment"] = std::regex("#.*");

    // Identificadores
    regexMap["identifier"] = std::regex("\\b[a-zA-Z_][a-zA-Z0-9_]*\\b");

    // Imprimir los tokens en consola
    removeCommentsAndPrintTokens("test.py", regexMap);

    return 0;
}
