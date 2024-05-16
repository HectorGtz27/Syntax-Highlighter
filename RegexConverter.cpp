#include "RegexConverter.h"
#include <regex>
#include <iostream>

// Escapa los caracteres especiales en la cadena de entrada
std::string escapeSpecialCharacters(const std::string& str) {
    std::string escaped;
    for (char c : str) {
        if (std::string("[]{}()^$.|?*+\\").find(c) != std::string::npos) {
            escaped += '\\';
        }
        escaped += c;
    }
    return escaped;
}

std::string translateSExprToRegex(const SExpr& sexpr) {
    try {
        if (sexpr.type == "lit") {
            return escapeSpecialCharacters(sexpr.value);
        }
        else if (sexpr.type == "concat") {
            std::string result;
            for (const auto& child : sexpr.children) {
                result += translateSExprToRegex(child);
            }
            return result;
        }
        else if (sexpr.type == "or") {
            std::string result;
            for (const auto& child : sexpr.children) {
                if (!result.empty()) result += "|";
                result += translateSExprToRegex(child);
            }
            return "(" + result + ")";
        }
        else if (sexpr.type == "star") {
            return "(" + translateSExprToRegex(sexpr.children[0]) + ")*";
        }
        else if (sexpr.type == "plus") {
            return "(" + translateSExprToRegex(sexpr.children[0]) + ")+";
        }
        else if (sexpr.type == "opt") {
            return "(" + translateSExprToRegex(sexpr.children[0]) + ")?";
        }
        else if (sexpr.type == "char-class") {
            return "[" + escapeSpecialCharacters(sexpr.value) + "]";
        }
        else if (sexpr.type == "group") {
            return "(" + translateSExprToRegex(sexpr.children[0]) + ")";
        }
    }
    catch (const std::regex_error& e) {
        std::cerr << "Regex error: " << e.what() << std::endl;
        return "";
    }
    return "";
}
