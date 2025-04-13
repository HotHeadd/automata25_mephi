#pragma once

#include "parser.hpp"

namespace myre
{

struct DFAState {

};


class DFABuilder {
public:
	std::shared_ptr<DFAState> buildDFA(std::shared_ptr<SyntaxNode> tree);
};

} // namespace myre


