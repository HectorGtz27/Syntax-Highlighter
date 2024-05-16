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
        // Procesar cada línea individualmente
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
            // Añadir manejo especial para #t, #f y null como literales en lugar de comentarios
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
            // Verificar si el operador es un guion y está dentro de un identificador
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

void generateHTML(const std::string& inputFilePath, const std::string& outputFilePath, const std::map<std::string, std::regex>& regexMap) {
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
        std::string originalLine = line; // Mantener una copia de la línea original
        // Procesar cada línea individualmente
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
            // Añadir manejo especial para #t, #f y null como literales en lugar de comentarios
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
            // Verificar si el operador es un guion y está dentro de un identificador
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

        // Generar HTML con los tokens en orden
        std::string htmlLine;
        size_t currentPos = 0;
        for (const auto& token : tokens) {
            size_t tokenPos = originalLine.find(token.second, currentPos);
            if (tokenPos != std::string::npos) {
                htmlLine += originalLine.substr(currentPos, tokenPos - currentPos);
                htmlLine += "<span class=\"" + token.first + "\">" + token.second + "</span>";
                currentPos = tokenPos + token.second.length();
            }
        }
        htmlLine += originalLine.substr(currentPos);
        outputFile << htmlLine << "\n";
    }

    outputFile << "</pre></body></html>";
    inputFile.close();
    outputFile.close();
}
