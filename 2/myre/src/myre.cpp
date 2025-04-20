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

bool search_first(const std::string& expr, DFA& dfa, Match& match){
	unsigned start_state = DFA::start_state;
	if (dfa.accepting_states.contains(start_state)){
		match.begin = expr.begin();
		match.end = expr.begin();
		return true;
	}
	for (int start_ind=0; start_ind<expr.size(); ++start_ind){
		int curr_ind = start_ind;
		unsigned curr_state = start_state;
		while (curr_ind < expr.size()){
			bool no_tranz = true;
			for (auto& tranz : dfa.transitions[curr_state]){
				if (tranz.symbol == expr[curr_ind]){
					curr_state = tranz.to;
					no_tranz = false;
					if (dfa.accepting_states.contains(curr_state)){
						match.begin = expr.begin()+start_ind;
						match.end = expr.begin()+curr_ind;
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

bool search_next(const std::string& expr, DFA& dfa, Match& match){
	
}


bool search(const std::string& expr, const std::string& regex){
	DFA dfa = compile(regex);
	Match match;
	return search_first(expr, dfa, match);
}

bool search(const std::string& expr, DFA& dfa){
	Match match;
	return search_first(expr, dfa, match);
}

bool fullmatch(const std::string& expr, const std::string& regex){
	DFA dfa = compile(regex);
	return fullmatch(expr, dfa);
}

bool search_first(const std::string& expr, const std::string& regex, Match& match){
	DFA dfa = compile(regex);
	return search_first(expr, dfa, match);
}

bool search_next(const std::string& expr, const std::string& regex, Match& match){
	DFA dfa = compile(regex);
	return search_next(expr, dfa, match);
}
} // namespace myre
