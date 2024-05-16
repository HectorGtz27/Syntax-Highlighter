#include "Lexer.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

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

    std::regex multilineCommentRegex = regexMap.at("comment_multiline");
    std::regex singlelineCommentRegex = regexMap.at("comment_singleline");
    std::regex keywordRegex = regexMap.at("keyword");
    std::regex literalRegex = regexMap.at("literal");
    std::regex operatorRegex = regexMap.at("operator");
    std::regex identifierRegex = regexMap.at("identifier");

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

    // Identificar palabras clave
    begin = std::sregex_iterator(content.begin(), content.end(), keywordRegex);
    for (std::sregex_iterator i = begin; i != end; ++i) {
        std::smatch match = *i;
        std::cout << "Found keyword: " << match.str() << std::endl;
        content.replace(match.position(), match.length(), std::string(match.length(), ' '));
    }

    // Identificar literales
    begin = std::sregex_iterator(content.begin(), content.end(), literalRegex);
    for (std::sregex_iterator i = begin; i != end; ++i) {
        std::smatch match = *i;
        std::cout << "Found literal: " << match.str() << std::endl;
        content.replace(match.position(), match.length(), std::string(match.length(), ' '));
    }

    // Identificar operadores
    begin = std::sregex_iterator(content.begin(), content.end(), operatorRegex);
    for (std::sregex_iterator i = begin; i != end; ++i) {
        std::smatch match = *i;
        std::cout << "Found operator: " << match.str() << std::endl;
        content.replace(match.position(), match.length(), std::string(match.length(), ' '));
    }

    // Identificar identificadores
    begin = std::sregex_iterator(content.begin(), content.end(), identifierRegex);
    for (std::sregex_iterator i = begin; i != end; ++i) {
        std::smatch match = *i;
        std::cout << "Found identifier: " << match.str() << std::endl;
    }
}
