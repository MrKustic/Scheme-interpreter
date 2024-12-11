#pragma once
#include <vector>
#include "../object.h"

Node LambdaSugar(const std::vector<Node>& args);

Node Define(const std::vector<Node>& args);

Node Set(const std::vector<Node>& args);

Node IsSymbol(const std::vector<Node>& args);

Node If(const std::vector<Node>& args);

Node GetPtrToVal(const Node&);
