#include "helpers.h"
#include "operations_impl/list_operations.h"
#include "operations_impl/integer_operations.h"
#include "operations_impl/boolean_operations.h"
#include "operations_impl/lambda.h"

std::unordered_map<std::string, FuncType> inner_functions = {
    {"quote", Quote},
    {"pair?", TypePredicate<1ll, -1ll, false>},
    {"null?", TypePredicate<0ll, 0ll, true>},
    {"list?", TypePredicate<-1ll, -1ll, true>},
    {"cons", CreatePair},
    {"car", GetPairValue<0>},
    {"cdr", GetPairValue<1>},
    {"list", CreateGoodList},
    {"list-ref", GetByIndex},
    {"list-tail", ListTail},

    {"=", MonotonicOperation<Equal>},
    {"<=", MonotonicOperation<LessEqual>},
    {">=", MonotonicOperation<GreaterEqual>},
    {"<", MonotonicOperation<Less>},
    {">", MonotonicOperation<Greater>},
    {"+", Arithmetic<Plus>},
    {"-", Arithmetic<Minus>},
    {"*", Arithmetic<Multiply>},
    {"/", Arithmetic<Division>},
    {"min", Arithmetic<Min>},
    {"max", Arithmetic<Max>},
    {"abs", Abs},

    {"number?", IsNumber},
    {"boolean?", IsBoolean},
    {"not", Not},
    {"and", AndOr<true>},
    {"or", AndOr<false>},

    {"define", Define},
    {"set!", Set},
    {"symbol?", IsSymbol},

    {"if", If},

    {"set-car!", SetPairValue<0>},
    {"set-cdr!", SetPairValue<1>},

    {"lambda", Lambda}};

std::unordered_map<std::string, Node> global_scope;

Scope::Scope(const std::shared_ptr<Scope>& parent) : parent_(parent) {
}

Scope::Scope() {
    parent_ = std::make_shared<Scope>(nullptr);
    parent_->variables_ = global_scope;
}

Node& Scope::GetVariable(const std::string& name) {
    if (variables_.contains(name)) {
        return variables_[name];
    }
    if (!parent_) {
        throw NameError("no such symbol");
    }
    return parent_->GetVariable(name);
}

void ExpectedCountArgs(const std::vector<Node>& args, size_t min_count, size_t max_count) {
    if (args.size() < min_count || args.size() > max_count) {
        throw SyntaxError("bad count of args");
    }
}

void ExpectedCountArgs(const std::vector<Node>& args, size_t count) {
    if (args.size() != count) {
        throw SyntaxError("bad count of args");
    }
}