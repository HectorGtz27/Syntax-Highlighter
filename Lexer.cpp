#include "Lexer.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <vector>

void removeCommentsAndPrintTokens(const std::string& filePath, const std::map<std::string, std::regex>& regexMap) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Procesar cada l�nea individualmente
        std::vector<std::pair<std::string, std::string>> tokens;

        // Procesar comentarios de varias l�neas
        std::sregex_iterator begin(line.begin(), line.end(), regexMap.at("comment_multiline"));
        std::sregex_iterator end;
        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            tokens.emplace_back("comment_multiline", match.str());
            line.replace(match.position(), match.length(), std::string(match.length(), ' '));
        }

        // Procesar comentarios de una l�nea
        begin = std::sregex_iterator(line.begin(), line.end(), regexMap.at("comment_singleline"));
        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            tokens.emplace_back("comment_singleline", match.str());
            line.replace(match.position(), match.length(), std::string(match.length(), ' '));
        }

        // Procesar literales
        begin = std::sregex_iterator(line.begin(), line.end(), regexMap.at("literal"));
        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            tokens.emplace_back("literal", match.str());
            line.replace(match.position(), match.length(), std::string(match.length(), ' '));
        }

        // Procesar palabras clave
        begin = std::sregex_iterator(line.begin(), line.end(), regexMap.at("keyword"));
        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            tokens.emplace_back("keyword", match.str());
            line.replace(match.position(), match.length(), std::string(match.length(), ' '));
        }

        // Procesar operadores
        begin = std::sregex_iterator(line.begin(), line.end(), regexMap.at("operator"));
        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            // Verificar si el operador es un guion y est� dentro de un identificador
            std::string token = match.str();
            if (token == "-" && std::regex_search(match.prefix().str() + "-" + match.suffix().str(), regexMap.at("identifier"))) {
                continue; // Saltar si es parte de un identificador
            }
            tokens.emplace_back("operator", match.str());
            line.replace(match.position(), match.length(), std::string(match.length(), ' '));
        }

        // Procesar identificadores
        begin = std::sregex_iterator(line.begin(), line.end(), regexMap.at("identifier"));
        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            tokens.emplace_back("identifier", match.str());
            line.replace(match.position(), match.length(), std::string(match.length(), ' '));
        }

        // Imprimir tokens en orden
        for (const auto& token : tokens) {
            std::cout << "Found " << token.first << ": " << token.second << std::endl;
        }
    }
    file.close();
}
