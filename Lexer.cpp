#include "Lexer.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

// Función para imprimir los tokens en consola, asegurando que los comentarios se detecten y se omitan al buscar identificadores
void removeCommentsAndPrintTokens(const std::string& filePath, const std::map<std::string, std::regex>& regexMap) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return;
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    file.close();

    std::regex multilineCommentRegex("\"\"\"[\\s\\S]*?\"\"\"");
    std::regex singlelineCommentRegex("#.*");
    std::regex identifierRegex("\\b[a-zA-Z_][a-zA-Z0-9_]*\\b");

    // Identificar y eliminar comentarios de varias líneas
    std::sregex_iterator begin(content.begin(), content.end(), multilineCommentRegex);
    std::sregex_iterator end;
    for (std::sregex_iterator i = begin; i != end; ++i) {
        std::smatch match = *i;
        std::cout << "Found comment: " << match.str() << std::endl;
        content.replace(match.position(), match.length(), std::string(match.length(), ' '));
    }

    // Identificar y eliminar comentarios de una línea
    begin = std::sregex_iterator(content.begin(), content.end(), singlelineCommentRegex);
    for (std::sregex_iterator i = begin; i != end; ++i) {
        std::smatch match = *i;
        std::cout << "Found comment: " << match.str() << std::endl;
        content.replace(match.position(), match.length(), std::string(match.length(), ' '));
    }

    // Identificar identificadores
    begin = std::sregex_iterator(content.begin(), content.end(), identifierRegex);
    for (std::sregex_iterator i = begin; i != end; ++i) {
        std::smatch match = *i;
        std::cout << "Found identifier: " << match.str() << std::endl;
    }
}
