#pragma once
#include "../object.h"
#include "../helpers.h"
#include "../scheme.h"

class CustomFunction {
public:
    Node operator()(const std::vector<Node>& args) const;
    CustomFunction(const std::vector<std::string>& vars, const std::vector<Node>& operations)
        : vars_(vars), parent_(Interpreter::scope), operations_(operations) {
    }

private:
    std::vector<std::string> vars_;
    std::shared_ptr<Scope> parent_;
    std::vector<Node> operations_;
};

Node Lambda(const std::vector<Node>& args);