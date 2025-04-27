#include "myre.hpp"

namespace myre
{
bool fullmatch(const std::string& expr, DFA& dfa){
	unsigned curr_state = dfa.start_state;
	for (auto ch: expr){
		if (dfa.transitions[curr_state].contains(ch)){
			curr_state = dfa.transitions[curr_state][ch];
		}
		else{
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