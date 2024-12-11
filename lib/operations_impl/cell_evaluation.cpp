#include "../helpers.h"
#include "../scheme.h"
#include <ranges>

inline std::vector<std::string> change_scope = {"set!", "define"};

Function* GetOperator(const Node& cell) {
    if (Is<Symbol>(cell)) {
        const std::string& symbol = As<Symbol>(cell)->GetName();
        if (std::ranges::find(change_scope, symbol) != change_scope.end()) {
            if (Interpreter::scope->IsGlobalScope()) {
                change_global_scope = true;
            }
        }
        return Interpreter::scope->GetFunction(symbol);
    }
    if (!cell) {
        throw RuntimeError("bad operator");
    }
    Node ret = cell->Eval();
    if (!Is<Function>(ret)) {
        throw RuntimeError("operator expected");
    }
    return As<Function>(ret);
}

inline std::vector<std::string> not_evaluate = {"quote", "and", "or",    "define",
                                                "set!",  "if",  "lambda"};

void AddDependencies(Node node, const std::vector<Node>& targets) {
    for (auto target : targets) {
        node->AddDependency(target);
    }
}

Node Cell::Eval() {
    Function* func_node = GetOperator(this->GetFirst());
    std::vector<Node> args;
    Node bad_arg = nullptr;
    if (!Is<Symbol>(GetFirst()) ||
        std::ranges::find(not_evaluate, As<Symbol>(GetFirst())->GetName()) == not_evaluate.end()) {
        GetArgs<true>(GetSecond(), args, bad_arg);
    } else {
        GetArgs<false>(GetSecond(), args, bad_arg);
    }
    if (bad_arg != nullptr) {
        throw RuntimeError("Unexpected parameter");
    }
    if (Is<Symbol>(GetFirst()) &&
        std::ranges::find(change_scope, As<Symbol>(GetFirst())->GetName()) != change_scope.end()) {
        AddDependencies(func_node, args);
    }
    auto func = func_node->GetFunc();
    auto val = func(args);
    SetValue(val);
    GetIsEvaluated() = true;
    return GetValue();
}
