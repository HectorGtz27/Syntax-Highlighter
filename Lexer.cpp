#include "Lexer.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <vector>

// Function to remove comments, print tokens, and store tokens
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

        // Process multiline comments
        std::sregex_iterator begin(line.begin(), line.end(), regexMap.at("comment_multiline"));
        std::sregex_iterator end;
        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            lineTokens.emplace_back("comment_multiline", match.str());
            line.replace(match.position(), match.length(), std::string(match.length(), ' '));
        }

        // Process single-line comments
        begin = std::sregex_iterator(line.begin(), line.end(), regexMap.at("comment_singleline"));
        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            std::string comment = match.str();
            if (comment.find("#t") != std::string::npos || comment.find("#f") != std::string::npos || comment.find("NULL") != std::string::npos) {
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
                while ((pos = comment.find("NULL", pos)) != std::string::npos) {
                    lineTokens.emplace_back("literal", "NULL");
                    pos += 4;
                }
            }
            else {
                lineTokens.emplace_back("comment_singleline", match.str());
            }
            line.replace(match.position(), match.length(), std::string(match.length(), ' '));
        }

        // Process literals
        begin = std::sregex_iterator(line.begin(), line.end(), regexMap.at("literal"));
        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            lineTokens.emplace_back("literal", match.str());
            line.replace(match.position(), match.length(), std::string(match.length(), ' '));
        }

        // Process keywords
        begin = std::sregex_iterator(line.begin(), line.end(), regexMap.at("keyword"));
        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            lineTokens.emplace_back("keyword", match.str());
            line.replace(match.position(), match.length(), std::string(match.length(), ' '));
        }

        // Process operators
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

        // Process identifiers
        begin = std::sregex_iterator(line.begin(), line.end(), regexMap.at("identifier"));
        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            lineTokens.emplace_back("identifier", match.str());
            line.replace(match.position(), match.length(), std::string(match.length(), ' '));
        }

        // Process special characters
        begin = std::sregex_iterator(line.begin(), line.end(), regexMap.at("special_character"));
        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            lineTokens.emplace_back("special_character", match.str());
            line.replace(match.position(), match.length(), std::string(match.length(), ' '));
        }

        for (const auto& token : lineTokens) {
            tokens.emplace_back(token.first, token.second);
            std::cout << "Found " << token.first << ": " << token.second << std::endl;
        }
    }

    file.close();
    return tokens;
}

// Function to generate HTML with the input file content and highlight tokens
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
        << ".special_character { color: purple; }"
        << "</style></head><body><pre>";

    std::string line;
    while (std::getline(inputFile, line)) {
        size_t pos = 0;
        while (pos < line.size()) {
            bool tokenFound = false;
            for (const auto& token : tokens) {
                if (line.compare(pos, token.value.size(), token.value) == 0) {
                    outputFile << "<span class=\"" << token.type << "\">" << token.value << "</span>";
                    pos += token.value.size();
                    tokenFound = true;
                    break;
                }
            }
            if (!tokenFound) {
                outputFile << line[pos++];
            }
        }
        outputFile << "\n";
    }

    outputFile << "</pre></body></html>";
    inputFile.close();
    outputFile.close();
}
