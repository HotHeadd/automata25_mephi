#pragma once

#include "parser.hpp"
#include <set>
#include <map>
#include "context.hpp"

namespace myre
{

struct Transition{
	char symbol;
	unsigned to;
	Transition(char sym, unsigned ind) : symbol(sym), to(ind) {}
	Transition() {}
};

struct DFA {
	unsigned start_state = 0;
	static constexpr unsigned null_state = -1;
	std::vector<std::vector<Transition>> transitions;
	std::set<unsigned> accepting_states;
};

class DFABuilder {
public:
	DFA buildDFA(ContextIndex tree_ind, Context& context);
	DFA minimize_dfa(const DFA& dfa);
	DFA build_complement(const DFA& dfa);
	DFA build_intersection(const DFA& first, const DFA& second);
private:
	std::set<unsigned> set_intersection(std::set<unsigned>& a, std::set<unsigned>& b);
};

} // namespace myre


