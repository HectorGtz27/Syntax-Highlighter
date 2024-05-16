#ifndef SEXPR_H
#define SEXPR_H

#include <string>
#include <vector>

struct SExpr {
    std::string type;
    std::string value;
    std::vector<SExpr> children;

    SExpr(const std::string& t) : type(t) {}
    SExpr(const std::string& t, const std::string& v) : type(t), value(v) {}
    SExpr(const std::string& t, const std::vector<SExpr>& c) : type(t), children(c) {}
    SExpr(const std::string& t, const std::string& v, const std::vector<SExpr>& c) : type(t), value(v), children(c) {}
};

#endif // SEXPR_H
