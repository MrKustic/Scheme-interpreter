#include "integer_operations.h"
#include "../helpers.h"

bool Equal::operator()(const Number* a, const Number* b) const {
    return a->GetValue() == b->GetValue();
}

bool LessEqual::operator()(const Number* a, const Number* b) const {
    return a->GetValue() <= b->GetValue();
}

bool GreaterEqual::operator()(const Number* a, const Number* b) const {
    return a->GetValue() >= b->GetValue();
}

bool Less::operator()(const Number* a, const Number* b) const {
    return a->GetValue() < b->GetValue();
}

bool Greater::operator()(const Number* a, const Number* b) const {
    return a->GetValue() > b->GetValue();
}

Number* Plus::operator()(const Number* a, const Number* b) const {
    return Heap::Instance().MakePointer<Number>(a->GetValue() + b->GetValue());
}

Number* Minus::operator()(const Number* a, const Number* b) const {
    return Heap::Instance().MakePointer<Number>(a->GetValue() - b->GetValue());
}

Number* Multiply::operator()(const Number* a, const Number* b) const {
    return Heap::Instance().MakePointer<Number>(a->GetValue() * b->GetValue());
}

Number* Division::operator()(const Number* a, const Number* b) const {
    if (b->GetValue() == 0) {
        throw RuntimeError("Division by zero");
    }
    return Heap::Instance().MakePointer<Number>(a->GetValue() / b->GetValue());
}

Number* Min::operator()(const Number* a, const Number* b) const {
    return Heap::Instance().MakePointer<Number>(std::min(a->GetValue(), b->GetValue()));
}

Number* Max::operator()(const Number* a, const Number* b) const {
    return Heap::Instance().MakePointer<Number>(std::max(a->GetValue(), b->GetValue()));
}

Node Abs(const std::vector<Node>& args) {
    if (args.size() != 1) {
        throw RuntimeError("bad count of args");
    }
    if (!Is<Number>(args[0])) {
        throw RuntimeError("unexpected type of argument");
    }
    return Heap::Instance().MakePointer<Number>(std::abs(As<Number>(args[0])->GetValue()));
}

Node IsNumber(const std::vector<Node>& args) {
    return Heap::Instance().MakePointer<Boolean>(args.size() == 1 && Is<Number>(args[0]));
}