#pragma once

#include <memory>
#include <functional>
#include <string>

class Object {
public:
    friend class Heap;
    friend class CustomFunction;
    friend void AddDependencies(Object* node, const std::vector<Object*>& targets);
    friend Object* Lambda(const std::vector<Object*>& args);
    virtual ~Object() = default;
    virtual Object* Eval() = 0;
    virtual std::string SerializeToString() = 0;

protected:
    void AddDependency(Object* to) {
        if (to) {
            edges_.push_back(to);
        }
    }

    void RemoveDependency(Object* to) {
        if (!to) {
            return;
        }
        if (const auto iter = std::find(edges_.begin(), edges_.end(), to); iter != edges_.end()) {
            edges_.erase(iter);
        }
    }

private:
    std::vector<Object*> edges_;
};

using Node = Object*;

class Number final : public Object {
public:
    friend class Heap;
    int GetValue() const;
    Number(const Number& other) = delete;
    Number(Number&& other) = default;
    Number& operator=(const Number& other) = delete;
    Number& operator=(Number&& other) = default;
    Object* Eval() override;
    std::string SerializeToString() override;

private:
    Number() = default;
    explicit Number(int value);
    int value_;
};

class Boolean final : public Object {
public:
    friend class Heap;
    bool GetValue() const;
    Boolean(const Boolean& other) = delete;
    Boolean(Boolean&& other) = default;
    Boolean& operator=(const Boolean& other) = delete;
    Boolean& operator=(Boolean&& other) = default;
    Object* Eval() override;
    std::string SerializeToString() override;

private:
    Boolean() = default;
    explicit Boolean(bool value);
    bool value_;
};

class Symbol final : public Object {
public:
    friend class Heap;
    Symbol(const Symbol& other) = delete;
    Symbol(Symbol&& other) = default;
    Symbol& operator=(const Symbol& other) = delete;
    Symbol& operator=(Symbol&& other) = default;
    const std::string& GetName() const;
    const Node& GetValue() const;
    void SetValue(Node);
    bool& GetIsEvaluated();
    Node Eval() override;
    std::string SerializeToString() override;
    void Clear();

private:
    Symbol() = default;
    explicit Symbol(const std::string& name);
    std::string name_;
    Node value_;
    bool is_evaluated_;
};

class Cell final : public Object {
public:
    friend class Heap;
    const Node& GetFirst() const;
    const Node& GetSecond() const;
    const Node& GetValue() const;
    void SetFirst(Node);
    void SetSecond(Node);
    void SetValue(Node);
    bool& GetIsEvaluated();
    Cell(const Cell& other) = delete;
    Cell(Cell&& other) = default;
    Cell& operator=(const Cell& other) = delete;
    Cell& operator=(Cell&& other) = default;
    Object* Eval() override;
    std::string SerializeToString() override;
    void Clear();

private:
    Cell();
    Node first_;
    Node second_;
    Node value_;
    bool is_evaluated_;
};

class Function final : public Object {
public:
    friend class Heap;
    Function(const Function& other) = delete;
    Function(Function&& other) = default;
    Function& operator=(const Function& other) = delete;
    Function& operator=(Function&& other) = default;
    std::function<Node(const std::vector<Node>&)> GetFunc() const {
        return func_;
    }
    Object* Eval() override;
    std::string SerializeToString() override;

private:
    Function() = default;
    Function(const std::function<Node(const std::vector<Node>&)>& func) : func_(func) {
    }
    std::function<Node(const std::vector<Node>&)> func_;
};

///////////////////////////////////////////////////////////////////////////////

template <class T>
T* As(Node obj) {
    return dynamic_cast<T*>(obj);
}

template <class T>
bool Is(Node obj) {
    return dynamic_cast<T*>(obj) != nullptr;
}