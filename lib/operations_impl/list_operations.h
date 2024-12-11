#pragma once
#include "../object.h"
#include "error.h"
#include "../helpers.h"
#include "../garbage_collector.h"
#include <vector>

template <long long MIN_CNT, long long MAX_CNT, bool NO_BAD_ARG>
static Node TypePredicate(const std::vector<Node>& args) {
    ExpectedCountArgs(args, 1);
    std::vector<Node> new_args;
    Node new_bad_arg = nullptr;
    if (args[0] && !Is<Cell>(args[0])) {
        throw RuntimeError("Bad arg");
    }
    GetArgs<true>(args[0], new_args, new_bad_arg);
    bool bad_exist = new_bad_arg != nullptr;
    long long cnt = new_args.size() + bad_exist;
    bool res = (MIN_CNT == -1 || cnt >= MIN_CNT) && (MAX_CNT == -1 || cnt <= MAX_CNT) &&
               (!NO_BAD_ARG || !bad_exist);
    return Heap::Instance().MakePointer<Boolean>(res);
}

Node Quote(const std::vector<Node>& args);

Node CreateList(const std::vector<Node>& args, const Node& bad_arg);

Node CreateGoodList(const std::vector<Node>& args);

Node CreatePair(const std::vector<Node>& args);

Node ListTail(const std::vector<Node>& args);

Node GetByIndex(const std::vector<Node>& args);

template <int POS>
Node GetPairValue(const std::vector<Node>& args) {
    ExpectedCountArgs(args, 1);
    if (!Is<Cell>(args[0])) {
        throw RuntimeError("pair expected");
    }
    const auto node = As<Cell>(args[0]);
    if (node->GetFirst() == nullptr) {
        throw RuntimeError("pair expected");
    }
    return POS ? node->GetSecond() : node->GetFirst();
}

template <int POS>
Node SetPairValue(const std::vector<Node>& args) {
    ExpectedCountArgs(args, 2);
    const Node new_node = args[0];
    if (!Is<Cell>(new_node)) {
        throw RuntimeError("pair expected");
    }
    const auto node = As<Cell>(new_node);
    if (node->GetFirst() == nullptr) {
        throw RuntimeError("pair expected");
    }
    if (POS) {
        node->SetSecond(args[1]);
    } else {
        node->SetFirst(args[1]);
    }
    return nullptr;
}