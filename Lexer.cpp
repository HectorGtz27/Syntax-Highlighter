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
        bool error_found = false; // Track if an error is found in the line

        std::cout << "Processing line: " << line << std::endl;

        // Validation: Check if the line starts with a number
        if (!line.empty() && std::isdigit(line[0])) {
            std::cerr << "Syntax error: Line starts with a number: " << line << std::endl;
            continue; // Skip processing this line
        }

        // Validation: Check if the line starts with an operator
        std::smatch match;
        if (std::regex_search(line, match, regexMap.at("operator")) && match.position() == 0) {
            std::cerr << "Syntax error: Line starts with an operator: " << line << std::endl;
            continue; // Skip processing this line
        }

        // Process multiline comments
        std::sregex_iterator begin_multiline(line.begin(), line.end(), regexMap.at("comment_multiline"));
        std::sregex_iterator end_multiline;
        for (std::sregex_iterator i = begin_multiline; i != end_multiline; ++i) {
            std::smatch match = *i;
            lineTokens.emplace_back("comment_multiline", match.str());
            line.replace(match.position(), match.length(), std::string(match.length(), ' '));
        }

        // Process single-line comments
        std::sregex_iterator begin_singleline(line.begin(), line.end(), regexMap.at("comment_singleline"));
        std::sregex_iterator end_singleline;
        for (std::sregex_iterator i = begin_singleline; i != end_singleline; ++i) {
            std::smatch match = *i;
            lineTokens.emplace_back("comment_singleline", match.str());
            line.replace(match.position(), match.length(), std::string(match.length(), ' '));
        }

        // Process identifiers and validate them
        std::cout << "Checking for identifiers..." << std::endl;
        std::sregex_iterator begin_identifier(line.begin(), line.end(), regexMap.at("identifier"));
        std::sregex_iterator end_identifier;
        for (std::sregex_iterator i = begin_identifier; i != end_identifier; ++i) {
            std::smatch match = *i;
            std::string identifier = match.str();
            std::cout << "Found identifier: " << identifier << std::endl;
            if (!std::isalpha(identifier[0])) {
                std::cerr << "Lexical error (Variable name has a wrong composition): " << identifier << " in line: " << line << std::endl;
                error_found = true;
                break; // Skip processing this line
            }
            lineTokens.emplace_back("identifier", match.str());
            line.replace(match.position(), match.length(), std::string(match.length(), ' '));
        }

        if (error_found) {
            std::cout << "Error found, skipping line" << std::endl;
            continue; // Skip the rest of the processing for this line
        }

        // Process literals
        std::sregex_iterator begin_literal(line.begin(), line.end(), regexMap.at("literal"));
        std::sregex_iterator end_literal;
        for (std::sregex_iterator i = begin_literal; i != end_literal; ++i) {
            std::smatch match = *i;
            lineTokens.emplace_back("literal", match.str());
            line.replace(match.position(), match.length(), std::string(match.length(), ' '));
        }

        // Process keywords
        std::sregex_iterator begin_keyword(line.begin(), line.end(), regexMap.at("keyword"));
        std::sregex_iterator end_keyword;
        for (std::sregex_iterator i = begin_keyword; i != end_keyword; ++i) {
            std::smatch match = *i;
            lineTokens.emplace_back("keyword", match.str());
            line.replace(match.position(), match.length(), std::string(match.length(), ' '));
        }

        // Process operators
        std::sregex_iterator begin_operator(line.begin(), line.end(), regexMap.at("operator"));
        std::sregex_iterator end_operator;
        for (std::sregex_iterator i = begin_operator; i != end_operator; ++i) {
            std::smatch match = *i;
            lineTokens.emplace_back("operator", match.str());
            line.replace(match.position(), match.length(), std::string(match.length(), ' '));
        }

        // Process special characters
        std::sregex_iterator begin_special(line.begin(), line.end(), regexMap.at("special_character"));
        std::sregex_iterator end_special;
        for (std::sregex_iterator i = begin_special; i != end_special; ++i) {
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
