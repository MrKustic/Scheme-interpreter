#pragma once
#include <vector>
#include "object.h"
#include <unordered_set>

class Heap {
public:
    Heap(const Heap&) = delete;
    Heap(Heap&&) = delete;
    Heap& operator=(const Heap&) = delete;
    Heap& operator=(Heap&&) = delete;

    template <typename T, typename... Args>
    friend T* MakePointer(Args&&... args)
        requires(std::is_base_of_v<Object, T>);

    static Heap& Instance();

    template <typename T, typename... Args>
    T* MakePointer(Args&&... args)
        requires(std::is_base_of_v<Object, T>)
    {
        auto ptr = new T(std::forward<Args>(args)...);
        heap_.push_back(std::unique_ptr<T>(ptr));
        if constexpr (std::is_same_v<T, Cell>) {
            for (std::array<Node, sizeof...(args)> list = {args...}; const Node child : list) {
                ptr->AddDependency(child);
            }
        }
        return ptr;
    }

    void ChangeObject(Node from, Node to) const;

    void Mark();

    void Sweep();

    void Clear();

private:
    Heap() = default;
    void Mark(Node root);

    std::vector<std::unique_ptr<Object>> heap_;
    std::unordered_set<Node> marked_;
};
