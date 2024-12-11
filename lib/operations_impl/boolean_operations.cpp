#include "boolean_operations.h"

#include "error.h"

#include "../helpers.h"

Node IsBoolean(const std::vector<Node>& args) {
    ExpectedCountArgs(args, 1);
    bool ret_val = Is<Boolean>(args[0]);
    return Heap::Instance().MakePointer<Boolean>(ret_val);
}

bool BooleanValue(const Node& node) {
    return !Is<Boolean>(node) || As<Boolean>(node)->GetValue();
}

Node Not(const std::vector<Node>& args) {
    if (args.size() != 1) {
        throw RuntimeError("bad count of args");
    }
    return Heap::Instance().MakePointer<Boolean>(!BooleanValue(args[0]));
}
