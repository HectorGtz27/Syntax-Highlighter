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
    regexMap["comment_multiline"] = std::regex(R"((\/\*[\s\S]*?\*\/)|(\#\|[\s\S]*?\|\#)|(\"\"\"[\s\S]*?\"\"\"))");

    // Comentarios de una línea
    regexMap["comment_singleline"] = std::regex(R"((\/\/.*)|(\;.*)|(\#.*))");

    // Identificadores
    regexMap["identifier"] = std::regex(R"(\b[a-zA-Z_][a-zA-Z0-9_]*\b)");

    // Literales
    regexMap["literal"] = std::regex(R"((\".*?\")|('.*?')|(\b\d+\b))");

    // Operadores
    regexMap["operator"] = std::regex(R"([+\-*/=<>!&|^%~]+)");

    // Palabras clave
    regexMap["keyword"] = std::regex(R"(\b(if|else|while|for|def|class|import|return|True|False|None|void|define|null|NULL|true|false|#t|#f)\b)");

    // Imprimir los tokens en consola
    removeCommentsAndPrintTokens("ejemplos.txt", regexMap);

    return 0;
}
