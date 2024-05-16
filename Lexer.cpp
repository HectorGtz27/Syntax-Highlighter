#include "Lexer.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

// Función para imprimir los tokens en consola
void printTokens(const std::string& filePath, const std::map<std::string, std::regex>& regexMap) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return;
    }

    std::string line;
    int lineNumber = 1;
    while (std::getline(file, line)) {
        for (const auto& token : regexMap) {
            const auto& tokenType = token.first;
            const auto& regex = token.second;
            std::sregex_iterator begin(line.begin(), line.end(), regex);
            std::sregex_iterator end;
            for (std::sregex_iterator i = begin; i != end; ++i) {
                std::smatch match = *i;
                std::cout << "Found " << tokenType << " at line " << lineNumber << ": " << match.str() << std::endl;
            }
        }
        lineNumber++;
    }

    file.close();
}
