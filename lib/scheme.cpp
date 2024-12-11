#include "scheme.h"
#include "operations_impl//variables.h"

#include "garbage_collector.h"
#include <sstream>
#include "tokenizer.h"
#include "parser.h"

Interpreter::Interpreter() {
    for (auto [name, value] : inner_functions) {
        global_scope[name] = Heap::Instance().MakePointer<Function>(value);
    }
    scope = std::make_shared<Scope>();
    std::vector<std::shared_ptr<Scope>> scopes{};
}

std::string Interpreter::Run(const std::string& str) {
    std::stringstream ss{str};
    Tokenizer tokenizer{&ss};
    const Node ast = Read(&tokenizer);
    if (ast == nullptr) {
        throw RuntimeError("Empty input");
    }
    try {
        ast->Eval();
        std::string ans = ast->SerializeToString();
        Heap::Instance().Mark();
        Heap::Instance().Sweep();
        return ans;
    } catch (const std::exception&) {
        Heap::Instance().Mark();
        Heap::Instance().Sweep();
        throw;
    }
}

Interpreter::~Interpreter() {
    Heap::Instance().Clear();
    scope.reset();
}
