#include "tokenizer.h"
#include <regex>

bool SymbolToken::operator==(const SymbolToken& other) const {
    return name == other.name;
}

bool QuoteToken::operator==(const QuoteToken&) const {
    return true;
}

bool DotToken::operator==(const DotToken&) const {
    return true;
}

bool ConstantToken::operator==(const ConstantToken& other) const {
    return value == other.value;
}

Tokenizer::Tokenizer(std::istream* in) {
    is_end_ = false;
    in_ = in;
    Next();
}

static const std::regex kFirstSymbol{"[a-zA-Z<=>*/#+-]"};
static const std::regex kNextSymbols{"[a-zA-Z<=>*/#0-9?!-]"};

void Tokenizer::Next() {
    if (is_end_) {
        return;
    }
    char ch;
    if (!(*in_ >> ch)) {
        is_end_ = true;
        return;
    }
    switch (ch) {
        case '\'':
            least_ = QuoteToken();
            return;
        case '.':
            least_ = DotToken();
            return;
        case '(':
            least_ = BracketToken::OPEN;
            return;
        case ')':
            least_ = BracketToken::CLOSE;
            return;
        default:
            break;
    }
    std::string str{ch};
    if (ch == '+' || ch == '-' || (ch >= '0' && ch <= '9')) {
        ch = in_->peek();
        while (ch >= '0' && ch <= '9') {
            str.push_back(ch);
            in_->get();
            ch = in_->peek();
        }
        if (str.size() == 1 && (str[0] == '+' || str[0] == '-')) {
            least_ = SymbolToken(str);
        } else {
            least_ = ConstantToken(std::atoi(str.c_str()));
        }
        return;
    }
    if (!std::regex_match(std::string{ch}, kFirstSymbol)) {
        throw SyntaxError("Bad name of the symbol");
    }
    ch = in_->peek();
    while (std::regex_match(std::string{ch}, kNextSymbols)) {
        str.push_back(ch);
        in_->get();
        ch = in_->peek();
    }
    least_ = SymbolToken(str);
}

bool Tokenizer::IsEnd() const {
    return is_end_;
}

Token Tokenizer::GetToken() {
    return least_;
}
