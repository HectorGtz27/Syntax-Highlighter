#ifndef REGEXCONVERTER_H
#define REGEXCONVERTER_H

#include "SExpr.h"
#include <string>

// Declaraciones de funciones
std::string translateSExprToRegex(const SExpr& sexpr);
std::string escapeSpecialCharacters(const std::string& str); // A�adir esta l�nea

#endif // REGEXCONVERTER_H
