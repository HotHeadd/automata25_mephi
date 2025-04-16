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
};

struct State {
	std::vector<Transition> transitions;
	bool is_accepting = false;
	std::set<unsigned> set_pos;
	State(std::set<unsigned> set) : set_pos(set) {
		is_accepting = set_pos.contains(SetHandler::get_final_ind());
	}
};

struct DFA {
	std::vector<State> ind_to_state;
};

class DFABuilder {
public:
	DFA buildDFA(std::shared_ptr<SyntaxNode> tree);
};

} // namespace myre


