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
	State curr_state = dfa.ind_to_state[0];
	for (auto ch: expr){
		bool no_tranz = true;
		for (auto& tranz : curr_state.transitions){
			if (tranz.symbol == ch){
				curr_state = dfa.ind_to_state[tranz.to];
				no_tranz = false;
				break;
			}
		}
		if (no_tranz){
			return false;
		}
	}
	if (curr_state.is_accepting){
		return true;
	}
	return false;
}

bool search(const std::string& expr, DFA& dfa){
	State start_state = dfa.ind_to_state[0];
	if (start_state.is_accepting){
		return true;
	}
	for (int ind=0; ind<expr.size(); ++ind){
		int curr_ind = ind;
		State curr_state = dfa.ind_to_state[0];
		while (curr_ind < expr.size()){
			bool no_tranz = true;
			for (auto& tranz : curr_state.transitions){
				if (tranz.symbol == expr[curr_ind]){
					curr_state = dfa.ind_to_state[tranz.to];
					no_tranz = false;
					if (curr_state.is_accepting){
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
