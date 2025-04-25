#include "myre.hpp"
#include <optional>

namespace myre
{
bool fullmatch(const std::string& expr, DFA& dfa){
	unsigned curr_state = dfa.start_state;
	for (auto ch: expr){
		bool no_tranz = true;
		for (auto& tranz : dfa.transitions[curr_state]){
			if (tranz.symbol == ch){
				curr_state = tranz.to;
				no_tranz = false;
				break;
			}
		}
		if (no_tranz){
			return dfa.accepting_states.contains(dfa.null_state);
		}
	}
	if (dfa.accepting_states.contains(curr_state)){
		return true;
	}
	return false;
}



bool fullmatch(const std::string& expr, const std::string& regex, bool optimize){
	DFA dfa = compile(regex, optimize);
	return fullmatch(expr, dfa);
	}
}