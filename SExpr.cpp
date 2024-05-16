#ifndef REGEXCONVERTER_H
#define REGEXCONVERTER_H

#include "SExpr.h"
#include <string>

std::string translateSExprToRegex(const SExpr& sexpr);

#endif // REGEXCONVERTER_H