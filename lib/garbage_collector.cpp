#include "garbage_collector.h"
#include "helpers.h"

#include "scheme.h"

Heap& Heap::Instance() {
    static auto heap = Heap();
    return heap;
}

void Heap::Mark(const Node root) {
    marked_.insert(root);
    for (Node to : root->edges_) {
        if (!marked_.contains(to)) {
            Mark(to);
        }
    }
}

void Heap::Mark() {
    for (auto [key, value] : Interpreter::scope->variables_) {
        Mark(value);
    }
    for (auto [key, value] : Interpreter::scope->parent_->variables_) {
        Mark(value);
    }
}

void Heap::ChangeObject(const Node from, const Node to) const {
    for (size_t pos = 0; pos < heap_.size(); ++pos) {
        auto elem = heap_[pos].get();
        if (auto iter = std::ranges::find(elem->edges_, from); iter != elem->edges_.end()) {
            *iter = to;
        }
    }
    to->edges_ = from->edges_;
}

void Heap::Sweep() {
    std::vector<std::unique_ptr<Object>> new_heap;
    for (size_t pos = 0; pos < heap_.size(); ++pos) {
        if (!marked_.contains(heap_[pos].get())) {
            heap_[pos].reset(nullptr);
        } else {
            new_heap.push_back(std::move(heap_[pos]));
        }
    }
    marked_.clear();
    heap_ = std::move(new_heap);
}

void Heap::Clear() {
    heap_.clear();
}
