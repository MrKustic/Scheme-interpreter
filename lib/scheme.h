#pragma once

#include "helpers.h"
#include "object.h"
#include <memory>
#include <string>

class Interpreter {
public:
    static std::string Run(const std::string&);
    Interpreter();
    ~Interpreter();
    friend class Symbol;
    friend class Cell;
    friend Node Define(const std::vector<Node>& args);
    friend Node Set(const std::vector<Node>& args);
    friend class CustomFunction;
    friend Function* GetOperator(const Node& cell);
    friend Node LambdaSugar(const std::vector<Node>& args);
    friend class Heap;

private:
    inline static std::shared_ptr<Scope> scope = nullptr;
};
