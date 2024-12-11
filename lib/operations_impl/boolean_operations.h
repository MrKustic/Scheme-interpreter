#pragma once
#include "../object.h"

#include "../garbage_collector.h"
#include <vector>

bool BooleanValue(const Node&);

Node IsBoolean(const std::vector<Node>& args);

Node Not(const std::vector<Node>& args);

template <bool BASE_VALUE>
Node AndOr(const std::vector<Node>& args) {
    if (args.empty()) {
        return Heap::Instance().MakePointer<Boolean>(BASE_VALUE);
    }
    for (size_t i = 0; i < args.size(); i++) {
        if (BooleanValue(args[i]->Eval()) != BASE_VALUE) {
            return args[i];
        }
    }
    return args.back();
}
