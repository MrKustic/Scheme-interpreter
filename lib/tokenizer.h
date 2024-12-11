#pragma once

#include <variant>
#include <istream>
#include "error.h"

struct SymbolToken {
    std::string name;

    SymbolToken() = default;
    SymbolToken(const SymbolToken& other) = default;
    SymbolToken(SymbolToken&& other) = default;
    SymbolToken& operator=(const SymbolToken& other) = default;
    SymbolToken& operator=(SymbolToken&& other) = default;

    explicit SymbolToken(const std::string_view str) : name(str) {
    }

    bool operator==(const SymbolToken& other) const;
};

struct QuoteToken {
    bool operator==(const QuoteToken&) const;
};

struct DotToken {
    bool operator==(const DotToken&) const;
};

enum class BracketToken { OPEN, CLOSE };

struct ConstantToken {
    int value;

    ConstantToken() : value(0) {
    }
    ConstantToken(const ConstantToken& other) = default;
    ConstantToken(ConstantToken&& other) = default;
    ConstantToken& operator=(const ConstantToken& other) = default;
    ConstantToken& operator=(ConstantToken&& other) = default;

    explicit ConstantToken(const int val) : value(val) {
    }

    bool operator==(const ConstantToken& other) const;
};

using Token = std::variant<ConstantToken, BracketToken, SymbolToken, QuoteToken, DotToken>;

class Tokenizer {
public:
    explicit Tokenizer(std::istream* in);

    bool IsEnd() const;

    void Next();

    Token GetToken();

private:
    bool is_end_;
    Token least_;
    std::istream* in_;
};