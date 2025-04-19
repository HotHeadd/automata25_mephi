#pragma once

#include "parser.hpp"
#include <set>
#include <map>

namespace myre
{

struct Transition{
	char symbol;
	unsigned to;
	Transition(char sym, unsigned ind) : symbol(sym), to(ind) {}
	Transition() {}
};

struct DFA {
	static constexpr unsigned start_state = 0;
	std::unordered_map<unsigned, std::vector<Transition>> transitions;
	std::set<unsigned> accepting_states;
};

class DFABuilder {
public:
	DFA buildDFA(std::shared_ptr<SyntaxNode> tree);
};

} // namespace myre


