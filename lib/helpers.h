#pragma once
#include "object.h"
#include "error.h"
#include <vector>
#include <functional>
#include "garbage_collector.h"

using FuncType = std::function<Node(const std::vector<Node>&)>;

extern std::unordered_map<std::string, FuncType> inner_functions;
extern std::unordered_map<std::string, Node> global_scope;

extern bool change_global_scope;

class Scope {
public:
    friend class Heap;
    explicit Scope(const std::shared_ptr<Scope>& parent);
    Scope();
    Node& GetVariable(const std::string& name);

    Function* GetFunction(const std::string& name) {
        const Node& var = GetVariable(name);
        try {
            return As<Function>(var);
        } catch (const std::exception&) {
            throw RuntimeError("bad type of symbol");
        }
    }

    template <typename T>
    void Define(const std::string& name, T value) {
        if (!parent_->parent_) {
            change_global_scope = true;
        }
        variables_[name] = value;
    }

    template <typename T>
    void Set(const std::string& name, T value) {
        if (!parent_) {
            change_global_scope = true;
        }
        Node& value_ptr = GetVariable(name);
        if (value_ptr != nullptr) {
            Heap::Instance().ChangeObject(value_ptr, value);
        }
        value_ptr = value;
    }

    bool IsGlobalScope() const {
        return !parent_->parent_;
    }

private:
    std::unordered_map<std::string, Node> variables_;
    std::shared_ptr<Scope> parent_;
};

template <bool EVAL>
static void GetArgs(const Node& node, std::vector<Node>& args, Node& bad_arg) {
    if (node == nullptr) {
        return;
    }
    if (!Is<Cell>(node)) {
        args.push_back(EVAL ? node->Eval() : node);
        return;
    }
    const auto cell_node = As<Cell>(node);
    if (!cell_node->GetFirst()) {
        args.push_back(nullptr);
        GetArgs<EVAL>(cell_node->GetSecond(), args, bad_arg);
        return;
    }
    args.push_back(EVAL ? cell_node->GetFirst()->Eval() : cell_node->GetFirst());
    if (cell_node->GetSecond() && !Is<Cell>(cell_node->GetSecond())) {
        bad_arg = EVAL ? cell_node->GetSecond()->Eval() : cell_node->GetSecond();
    } else {
        GetArgs<EVAL>(As<Cell>(cell_node->GetSecond()), args, bad_arg);
    }
}

void ExpectedCountArgs(const std::vector<Node>& args, size_t min_count, size_t max_count);

void ExpectedCountArgs(const std::vector<Node>& args, size_t count);
