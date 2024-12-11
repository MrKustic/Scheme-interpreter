#include "lambda.h"
#include <memory>
#include "../garbage_collector.h"

Node CustomFunction::operator()(const std::vector<Node>& args) const {
    auto new_scope = std::make_shared<Scope>(parent_);
    std::swap(Interpreter::scope, new_scope);
    if (args.size() != vars_.size()) {
        throw RuntimeError("bad count of args");
    }
    for (size_t pos = 0; pos < args.size(); ++pos) {
        Interpreter::scope->Define(vars_[pos], args[pos]);
    }
    Node value{};
    for (const Node& op : operations_) {
        value = op->Eval();
    }
    std::swap(Interpreter::scope, new_scope);
    return value;
}

Node Lambda(const std::vector<Node>& args) {
    ExpectedCountArgs(args, 2, LLONG_MAX);
    std::vector<Node> vars;
    Node bad_arg = nullptr;
    GetArgs<false>(args[0], vars, bad_arg);
    if (bad_arg != nullptr) {
        throw SyntaxError("unexpected bad arg");
    }
    std::vector<std::string> var_names(vars.size());
    for (size_t i = 0; i < vars.size(); i++) {
        if (!Is<Symbol>(vars[i])) {
            throw SyntaxError("variable name expected");
        }
        var_names[i] = As<Symbol>(vars[i])->GetName();
    }
    std::vector<Node> operations;
    for (size_t i = 1; i < args.size(); i++) {
        operations.push_back(args[i]);
    }
    const Node ret = Heap::Instance().MakePointer<Function>(CustomFunction(var_names, operations));
    for (const auto op : operations) {
        ret->AddDependency(op);
    }
    return ret;
}