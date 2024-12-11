#include "variables.h"
#include "boolean_operations.h"
#include "../helpers.h"
#include "../scheme.h"

#include "../garbage_collector.h"

Node LambdaSugar(const std::vector<Node>& args) {
    const auto signature = As<Cell>(args[0]);
    if (!Is<Symbol>(signature->GetFirst())) {
        throw RuntimeError("symbol expected");
    }
    std::vector<Node> lambda = args;
    lambda[0] = signature->GetSecond();
    const Node body = Lambda(lambda);
    const std::string name = As<Symbol>(signature->GetFirst())->GetName();
    Interpreter::scope->Define(name, body);
    return nullptr;
}

Node Define(const std::vector<Node>& args) {
    ExpectedCountArgs(args, 1, LLONG_MAX);
    if (Is<Cell>(args[0])) {
        return LambdaSugar(args);
    }
    ExpectedCountArgs(args, 2);
    if (!Is<Symbol>(args[0])) {
        throw RuntimeError("bad symbol's name");
    }
    const std::string name = As<Symbol>(args[0])->GetName();
    Interpreter::scope->Define(name, args[1]->Eval());
    return nullptr;
}

Node Set(const std::vector<Node>& args) {
    ExpectedCountArgs(args, 2);
    if (!Is<Symbol>(args[0])) {
        throw RuntimeError("bad symbol's name");
    }
    const std::string name = As<Symbol>(args[0])->GetName();
    Interpreter::scope->Set(name, args[1]->Eval());
    return Heap::Instance().MakePointer<Symbol>();
}

Node IsSymbol(const std::vector<Node>& args) {
    ExpectedCountArgs(args, 1);
    bool res = Is<Symbol>(args[0]);
    return Heap::Instance().MakePointer<Boolean>(res);
}

Node If(const std::vector<Node>& args) {
    ExpectedCountArgs(args, 2, 3);
    if (BooleanValue(args[0]->Eval())) {
        return args[1]->Eval();
    }
    if (args.size() == 3) {
        return args[2]->Eval();
    }
    return nullptr;
}

Node GetPtrToVal(const Node& node) {
    if (!Is<Symbol>(node)) {
        throw RuntimeError("can not evaluate");
    }
    return node->Eval();
}
