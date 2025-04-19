#include "myre.hpp"
#include <iostream>

namespace myre
{

DFA compile(const std::string& regex){
	RegexParser parser;
	DFABuilder builder;
	
	std::shared_ptr<SyntaxNode> tree = parser.parse(regex);
	return builder.buildDFA(tree);
}

bool search(const std::string& expr, const std::string& regex){
	DFA dfa = compile(regex);
	return search(expr, dfa);
}

bool fullmatch(const std::string& expr, const std::string& regex){
	DFA dfa = compile(regex);
	return fullmatch(expr, dfa);
}

bool fullmatch(const std::string& expr, DFA& dfa){
	unsigned curr_state = DFA::start_state;
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
			return false;
		}
	}
	if (dfa.accepting_states.contains(curr_state)){
		return true;
	}
	return false;
}

bool search(const std::string& expr, DFA& dfa){
	unsigned start_state = DFA::start_state;
	if (dfa.accepting_states.contains(start_state)){
		return true;
	}
	for (int ind=0; ind<expr.size(); ++ind){
		int curr_ind = ind;
		unsigned curr_state = start_state;
		while (curr_ind < expr.size()){
			bool no_tranz = true;
			for (auto& tranz : dfa.transitions[curr_state]){
				if (tranz.symbol == expr[curr_ind]){
					curr_state = tranz.to;
					no_tranz = false;
					if (dfa.accepting_states.contains(curr_state)){
						return true;
					}
					break;
				}
			}
			if (no_tranz){
				break;
			}
			++curr_ind;
		}
	}
	return false;
}
	
} // namespace myre
