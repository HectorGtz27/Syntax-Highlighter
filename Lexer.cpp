#include "Lexer.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

// Función para tokenizar y mostrar tokens en la consola
void removeCommentsAndPrintTokens(const std::string& filePath, const std::map<std::string, std::regex>& regexMap) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::pair<std::string, std::string>> tokens;

        // Procesar comentarios de varias líneas
        std::sregex_iterator begin(line.begin(), line.end(), regexMap.at("comment_multiline"));
        std::sregex_iterator end;
        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            tokens.emplace_back("comment_multiline", match.str());
            line.replace(match.position(), match.length(), std::string(match.length(), ' '));
        }

        // Procesar comentarios de una línea
        begin = std::sregex_iterator(line.begin(), line.end(), regexMap.at("comment_singleline"));
        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            std::string comment = match.str();
            if (comment.find("#t") != std::string::npos || comment.find("#f") != std::string::npos || comment.find("null") != std::string::npos) {
                std::string::size_type pos = 0;
                while ((pos = comment.find("#t", pos)) != std::string::npos) {
                    tokens.emplace_back("literal", "#t");
                    pos += 2;
                }
                pos = 0;
                while ((pos = comment.find("#f", pos)) != std::string::npos) {
                    tokens.emplace_back("literal", "#f");
                    pos += 2;
                }
                pos = 0;
                while ((pos = comment.find("null", pos)) != std::string::npos) {
                    tokens.emplace_back("literal", "null");
                    pos += 4;
                }
            }
            else {
                tokens.emplace_back("comment_singleline", match.str());
            }
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
            std::string token = match.str();
            if (token == "-" && std::regex_search(match.prefix().str() + "-" + match.suffix().str(), regexMap.at("identifier"))) {
                continue;
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

        for (const auto& token : tokens) {
            std::cout << "Found " << token.first << ": " << token.second << std::endl;
        }
    }
    file.close();
}

// Función para tokenizar y almacenar tokens en un vector
std::vector<Token> removeCommentsAndStoreTokens(const std::string& filePath, const std::map<std::string, std::regex>& regexMap) {
    std::ifstream file(filePath);
    std::vector<Token> tokens;

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return tokens;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::pair<std::string, std::string>> lineTokens;

        // Procesar comentarios de varias líneas
        std::sregex_iterator begin(line.begin(), line.end(), regexMap.at("comment_multiline"));
        std::sregex_iterator end;
        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            lineTokens.emplace_back("comment_multiline", match.str());
            line.replace(match.position(), match.length(), std::string(match.length(), ' '));
        }

        // Procesar comentarios de una línea
        begin = std::sregex_iterator(line.begin(), line.end(), regexMap.at("comment_singleline"));
        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            std::string comment = match.str();
            if (comment.find("#t") != std::string::npos || comment.find("#f") != std::string::npos || comment.find("null") != std::string::npos) {
                std::string::size_type pos = 0;
                while ((pos = comment.find("#t", pos)) != std::string::npos) {
                    lineTokens.emplace_back("literal", "#t");
                    pos += 2;
                }
                pos = 0;
                while ((pos = comment.find("#f", pos)) != std::string::npos) {
                    lineTokens.emplace_back("literal", "#f");
                    pos += 2;
                }
                pos = 0;
                while ((pos = comment.find("null", pos)) != std::string::npos) {
                    lineTokens.emplace_back("literal", "null");
                    pos += 4;
                }
            }
            else {
                lineTokens.emplace_back("comment_singleline", match.str());
            }
            line.replace(match.position(), match.length(), std::string(match.length(), ' '));
        }

        // Procesar literales
        begin = std::sregex_iterator(line.begin(), line.end(), regexMap.at("literal"));
        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            lineTokens.emplace_back("literal", match.str());
            line.replace(match.position(), match.length(), std::string(match.length(), ' '));
        }

        // Procesar palabras clave
        begin = std::sregex_iterator(line.begin(), line.end(), regexMap.at("keyword"));
        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            lineTokens.emplace_back("keyword", match.str());
            line.replace(match.position(), match.length(), std::string(match.length(), ' '));
        }

        // Procesar operadores
        begin = std::sregex_iterator(line.begin(), line.end(), regexMap.at("operator"));
        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            std::string token = match.str();
            if (token == "-" && std::regex_search(match.prefix().str() + "-" + match.suffix().str(), regexMap.at("identifier"))) {
                continue;
            }
            lineTokens.emplace_back("operator", match.str());
            line.replace(match.position(), match.length(), std::string(match.length(), ' '));
        }

        // Procesar identificadores
        begin = std::sregex_iterator(line.begin(), line.end(), regexMap.at("identifier"));
        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            lineTokens.emplace_back("identifier", match.str());
            line.replace(match.position(), match.length(), std::string(match.length(), ' '));
        }

        for (const auto& token : lineTokens) {
            tokens.emplace_back(token.first, token.second);
        }
    }

    file.close();
    return tokens;
}

// Función para generar HTML con los tokens y el contenido del archivo de entrada
void generateHTMLWithTokens(const std::vector<Token>& tokens, const std::string& inputFilePath, const std::string& outputFilePath) {
    std::ifstream inputFile(inputFilePath);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << inputFilePath << std::endl;
        return;
    }

    std::ofstream outputFile(outputFilePath);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening file: " << outputFilePath << std::endl;
        return;
    }

    outputFile << "<!DOCTYPE html><html><head><style>"
        << ".comment_multiline { color: green; }"
        << ".comment_singleline { color: green; }"
        << ".literal { color: orange; }"
        << ".keyword { color: blue; font-weight: bold; }"
        << ".operator { color: red; }"
        << ".identifier { color: aqua; }"
        << "</style></head><body><pre>";

    std::string line;
    while (std::getline(inputFile, line)) {
        outputFile << line << "\n";
    }

    outputFile << "\nTokens:\n";

    for (const auto& token : tokens) {
        outputFile << "Found <span class=\"" << token.type << "\">" << token.type << "</span>: <span class=\"" << token.type << "\">" << token.value << "</span><br>";
    }

    outputFile << "</pre></body></html>";

    inputFile.close();
    outputFile.close();
}
