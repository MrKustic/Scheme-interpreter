#include "list_operations.h"
#include "error.h"
#include "../object.h"

Node Quote(const std::vector<Node>& args) {
    ExpectedCountArgs(args, 1);
    return args[0];
}

Node CreateList(const std::vector<Node>& args, const Node& bad_arg) {
    if (args.empty()) {
        if (bad_arg) {
            throw RuntimeError("bad count of args");
        }
        return nullptr;
    }
    auto ret = Heap::Instance().MakePointer<Cell>();
    auto curr = ret;
    for (size_t i = 0; i < args.size(); i++) {
        curr->SetFirst(args[i]);
        if (i == args.size() - 1) {
            curr->SetSecond(bad_arg);
        } else {
            curr->SetSecond(Heap::Instance().MakePointer<Cell>());
        }
        curr = As<Cell>(curr->GetSecond());
    }
    return ret;
}

Node CreateGoodList(const std::vector<Node>& args) {
    return CreateList(args, nullptr);
}

Node CreatePair(const std::vector<Node>& args) {
    ExpectedCountArgs(args, 2);
    const Node bad_arg = args.back();
    std::vector<Node> new_args = args;
    new_args.pop_back();
    return CreateList(new_args, bad_arg);
}

Node ListTail(const std::vector<Node>& args) {
    ExpectedCountArgs(args, 2);
    if (!Is<Number>(args[1])) {
        throw RuntimeError("Bad arg");
    }
    size_t start_pos = As<Number>(args[1])->GetValue();
    std::vector<Node> new_args;
    Node new_bad_arg = nullptr;
    if (!Is<Cell>(args[0])) {
        throw RuntimeError("Bad arg");
    }
    GetArgs<true>(args[0], new_args, new_bad_arg);
    if (start_pos > new_args.size()) {
        throw RuntimeError("Bad index");
    }
    std::vector<Node> ret_args;
    if (start_pos == new_args.size()) {
        return new_bad_arg;
    }
    for (size_t pos = start_pos; pos < new_args.size(); ++pos) {
        ret_args.push_back(new_args[pos]);
    }
    const auto ret_bad_arg = new_bad_arg;
    return CreateList(ret_args, ret_bad_arg);
}

Node GetByIndex(const std::vector<Node>& args) {
    ExpectedCountArgs(args, 2);
    if (!Is<Number>(args[1])) {
        throw RuntimeError("Bad arg");
    }
    const size_t pos = As<Number>(args[1])->GetValue();
    std::vector<Node> new_args;
    Node new_bad_arg = nullptr;
    if (!Is<Cell>(args[0])) {
        throw RuntimeError("Bad arg");
    }
    GetArgs<true>(args[0], new_args, new_bad_arg);
    if (pos >= new_args.size()) {
        throw RuntimeError("Bad index");
    }
    return new_args[pos];
}
