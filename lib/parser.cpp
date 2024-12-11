#include <error.h>
#include "parser.h"
#include "helpers.h"
#include "garbage_collector.h"

static bool IsRightParenthesis(const Token& token, const BracketToken& val) {
    if (const BracketToken* par = std::get_if<BracketToken>(&token)) {
        if (*par == val) {
            return true;
        }
    }
    return false;
}

Node ReadList(Tokenizer* tokenizer) {
    if (IsRightParenthesis(tokenizer->GetToken(), BracketToken::CLOSE)) {
        tokenizer->Next();
        return nullptr;
    }
    const auto parent = Heap::Instance().MakePointer<Cell>();
    Cell* curr_vertex = parent;
    while (!IsRightParenthesis(tokenizer->GetToken(), BracketToken::CLOSE) && !tokenizer->IsEnd()) {
        curr_vertex->SetFirst(Read(tokenizer));
        Token token{tokenizer->GetToken()};
        if (std::get_if<DotToken>(&token)) {
            tokenizer->Next();
            curr_vertex->SetSecond(Read(tokenizer));
            break;
        }
        if (IsRightParenthesis(token, BracketToken::CLOSE)) {
            break;
        }
        const auto right = Heap::Instance().MakePointer<Cell>();
        curr_vertex->SetSecond(right);
        curr_vertex = right;
    }
    if (tokenizer->IsEnd() || !IsRightParenthesis(tokenizer->GetToken(), BracketToken::CLOSE)) {
        throw SyntaxError("Unexpected EOF, expected ')'");
    }
    tokenizer->Next();
    return parent;
}

bool change_global_scope = false;

Node Read(Tokenizer* tokenizer) {
    change_global_scope = false;
    if (tokenizer->IsEnd()) {
        throw SyntaxError("Unexpected EOF");
    }
    const Token token{tokenizer->GetToken()};
    tokenizer->Next();
    if (IsRightParenthesis(token, BracketToken::OPEN)) {
        return ReadList(tokenizer);
    }
    if (IsRightParenthesis(token, BracketToken::CLOSE)) {
        throw SyntaxError("Unexpected ')'");
    }
    if (std::get_if<QuoteToken>(&token)) {
        auto ret = Heap::Instance().MakePointer<Cell>();
        ret->SetFirst(Heap::Instance().MakePointer<Symbol>("quote"));
        auto right = Heap::Instance().MakePointer<Cell>();
        ret->SetSecond(right);
        right->SetFirst(Read(tokenizer));
        return ret;
    }
    if (const SymbolToken* sym = std::get_if<SymbolToken>(&token)) {
        if (sym->name == "#t") {
            return Heap::Instance().MakePointer<Boolean>(true);
        }
        if (sym->name == "#f") {
            return Heap::Instance().MakePointer<Boolean>(false);
        }
        return Heap::Instance().MakePointer<Symbol>(sym->name);
    }
    if (const ConstantToken* sym = std::get_if<ConstantToken>(&token)) {
        return Heap::Instance().MakePointer<Number>(sym->value);
    }
    throw SyntaxError("Unexpected symbol");
}
