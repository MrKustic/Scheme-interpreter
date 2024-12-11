#include "object.h"
#include "operations_impl/variables.h"
#include "scheme.h"
#include <vector>

Number::Number(const int value) : value_(value) {
}

int Number::GetValue() const {
    return value_;
}

Node Number::Eval() {
    return this;
}

std::string Number::SerializeToString() {
    return std::to_string(GetValue());
}

Boolean::Boolean(const bool value) : value_(value) {
}

bool Boolean::GetValue() const {
    return value_;
}

Node Boolean::Eval() {
    return this;
}

std::string Boolean::SerializeToString() {
    return value_ ? "#t" : "#f";
}

Symbol::Symbol(const std::string& name) : name_(name), value_(nullptr), is_evaluated_(false) {
}

const std::string& Symbol::GetName() const {
    return name_;
}

const Node& Symbol::GetValue() const {
    return value_;
}

void Symbol::SetValue(const Node value) {
    value_ = value;
}

bool& Symbol::GetIsEvaluated() {
    return is_evaluated_;
}

Node Symbol::Eval() {
    const auto var = Interpreter::scope->GetVariable(GetName());
    SetValue(var);
    GetIsEvaluated() = true;
    return GetValue();
}

std::string Symbol::SerializeToString() {
    if (GetIsEvaluated()) {
        return GetValue()->SerializeToString();
    }
    return GetName();
}

Cell::Cell() : first_(nullptr), second_(nullptr), value_(nullptr), is_evaluated_(false) {
}

const Node& Cell::GetFirst() const {
    return first_;
}

const Node& Cell::GetSecond() const {
    return second_;
}

void Cell::SetFirst(const Node value) {
    if (first_) {
        RemoveDependency(first_);
    }
    first_ = value;
    AddDependency(value);
}

void Cell::SetSecond(const Node value) {
    if (second_) {
        RemoveDependency(second_);
    }
    second_ = value;
    AddDependency(value);
}

bool& Cell::GetIsEvaluated() {
    return is_evaluated_;
}

const Node& Cell::GetValue() const {
    return value_;
}

void Cell::SetValue(const Node value) {
    value_ = value;
}

static void GetArgsForSerialize(const Cell* node, std::vector<Node>& args, Node& bad_arg) {
    if (node->GetFirst() == nullptr) {
        args.push_back(nullptr);
    } else if (Is<Cell>(node->GetFirst())) {
        args.push_back(As<Cell>(node->GetFirst())->GetValue());
    } else {
        args.push_back(node->GetFirst());
    }
    if (node->GetSecond() == nullptr) {
        return;
    }
    if (Is<Cell>(node->GetSecond())) {
        GetArgsForSerialize(As<Cell>(node->GetSecond()), args, bad_arg);
    } else {
        bad_arg = node->GetSecond();
    }
}

std::string Cell::SerializeToString() {
    if (GetIsEvaluated()) {
        return GetValue() ? GetValue()->SerializeToString() : "()";
    }
    std::vector<Node> args;
    Node bad_arg = nullptr;
    GetArgsForSerialize(As<Cell>(this), args, bad_arg);
    std::string res = "(";
    for (size_t i = 0; i + 1 < args.size(); i++) {
        if (args[i] == nullptr) {
            res += "()";
        } else {
            res += args[i]->SerializeToString() + ' ';
        }
    }
    if (args.back() == nullptr) {
        res += "()";
    } else {
        res += args.back()->SerializeToString();
    }
    if (bad_arg != nullptr) {
        res += " . ";
        res += bad_arg->SerializeToString();
    }
    res += ')';
    return res;
}

void Symbol::Clear() {
    SetValue(nullptr);
    GetIsEvaluated() = false;
}

void Cell::Clear() {
    GetIsEvaluated() = false;
    SetValue(nullptr);
    if (Is<Cell>(GetFirst())) {
        As<Cell>(GetFirst())->Clear();
    } else if (Is<Symbol>(GetFirst())) {
        As<Symbol>(GetFirst())->Clear();
    }
    if (Is<Cell>(GetSecond())) {
        As<Cell>(GetSecond())->Clear();
    } else if (Is<Symbol>(GetSecond())) {
        As<Symbol>(GetSecond())->Clear();
    }
}

Node Function::Eval() {
    throw RuntimeError("lambda eval");
}
std::string Function::SerializeToString() {
    throw RuntimeError("lambda to string");
}