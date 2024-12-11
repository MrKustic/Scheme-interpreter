#pragma once
#include "../object.h"
#include "../error.h"
#include "variables.h"
#include "../garbage_collector.h"

struct Equal {
    bool operator()(const Number* a, const Number* b) const;
};

struct LessEqual {
    bool operator()(const Number* a, const Number* b) const;
};

struct GreaterEqual {
    bool operator()(const Number* a, const Number* b) const;
};

struct Less {
    bool operator()(const Number* a, const Number* b) const;
};

struct Greater {
    bool operator()(const Number* a, const Number* b) const;
};

template <typename Functor>
Node MonotonicOperation(const std::vector<Node>& args) {
    for (size_t i = 0; i + 1 < args.size(); i++) {
        if (!Is<Number>(args[i]) || !Is<Number>(args[i + 1])) {
            throw RuntimeError("bad parameter");
        }
        if (!Functor{}(As<Number>(args[i]), As<Number>(args[i + 1]))) {
            return Heap::Instance().MakePointer<Boolean>(false);
        }
    }
    return Heap::Instance().MakePointer<Boolean>(true);
}

struct BaseValue {
    int base_;
};

struct Plus : public BaseValue {
    Plus() : BaseValue() {
    }
    Number* operator()(const Number* a, const Number* b) const;
};

struct Minus {
    Minus() = default;
    Number* operator()(const Number* a, const Number* b) const;
};

struct Multiply : public BaseValue {
    Multiply() : BaseValue(1) {
    }
    Number* operator()(const Number* a, const Number* b) const;
};

struct Division {
    Division() = default;
    Number* operator()(const Number* a, const Number* b) const;
};

struct Min {
    Min() = default;
    Number* operator()(const Number* a, const Number* b) const;
};

struct Max {
    Max() = default;
    Number* operator()(const Number* a, const Number* b) const;
};

template <typename Functor>
Node Arithmetic(const std::vector<Node>& args) {
    if (args.empty()) {
        if constexpr (!std::is_base_of_v<BaseValue, Functor>) {
            throw RuntimeError("too few arguments");
        } else {
            return Heap::Instance().MakePointer<Number>(Functor{}.base_);
        }
    }
    if (!Is<Number>(args[0])) {
        throw RuntimeError("Bad arg");
    }
    Number* val = As<Number>(args[0]);
    for (size_t i = 1; i < args.size(); i++) {
        if (!Is<Number>(args[i])) {
            throw RuntimeError("Bad arg");
        }
        val = Functor{}(val, As<Number>(args[i]));
    }
    return val;
}

Node Abs(const std::vector<Node>& args);

Node IsNumber(const std::vector<Node>& args);