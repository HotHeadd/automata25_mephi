#pragma once

#include "parser.hpp"

namespace myre
{

struct DFAState {

};

class Regex {
public:
	Regex(std::string expr) : expr_(expr) {}
	std::shared_ptr<DFAState> compile(); // parse + toDFA
private:
	std::string expr_;
};

class DFABuilder {
public:
	std::shared_ptr<DFAState> buildDFA(std::shared_ptr<SyntaxNode> tree);
};

} // namespace myre
