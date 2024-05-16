#include "SExpr.h"
#include "RegexConverter.h"
#include "Lexer.h"
#include <map>
#include <regex>
#include <iostream>

int main() {
    // Definir expresiones regulares directamente
    std::map<std::string, std::regex> regexMap;

    try {
        // Comentarios de varias líneas
        regexMap["comment_multiline"] = std::regex(R"((\/\*[\s\S]*?\*\/)|(\#\|[\s\S]*?\|\#)|(\"\"\"[\s\S]*?\"\"\"))");

        // Comentarios de una línea (considerando ; como comentario solo si va seguido de algo)
        regexMap["comment_singleline"] = std::regex(R"(\/\/.*|#.*|;.+)");

        // Identificadores (permitiendo guiones en Racket)
        regexMap["identifier"] = std::regex(R"(\b[a-zA-Z_][a-zA-Z0-9_-]*\b)");

        // Literales (incluyendo True, False, None, #t, #f, null)
        regexMap["literal"] = std::regex(R"((\".*?\")|('.*?')|\b\d+\b|True|False|None|true|false|NULL|#t|#f|null)");

        // Operadores (ajustar para evitar conflictos con identificadores que contienen guiones)
        regexMap["operator"] = std::regex(R"([+\-*/=<>!&|^%~]+)");

        // Palabras clave
        regexMap["keyword"] = std::regex(R"(\b(if|else|while|for|def|class|import|return|void|define|list|display|print|printf|int)\b)");

        // Imprimir los tokens en consola
        removeCommentsAndPrintTokens("ejemplos.txt", regexMap);

        // Guardar los tokens en un vector
        std::vector<Token> tokens = removeCommentsAndStoreTokens("ejemplos.txt", regexMap);

        // Generar el archivo HTML con los tokens
        generateHTMLWithTokens(tokens, "output.html");
    }
    catch (const std::regex_error& e) {
        std::cerr << "Regex error: " << e.what() << std::endl;
    }

    return 0;
}
