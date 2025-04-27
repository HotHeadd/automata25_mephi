#include "myre.hpp"

namespace myre
{
	
DFA compile(const std::string& regex, bool optimize){
	RegexParser parser;
	DFABuilder builder;
	Context context;
	ContextIndex tree = parser.parse(regex, context);
	DFA dfa = builder.buildDFA(tree, context);
	if (optimize){
		dfa = builder.minimize_dfa(dfa);
	}
	return dfa;
}

std::string regex_union(std::string& f, std::string& s){
	return "(" + f + "|" + s + ")";
}
std::string regex_kleene(std::string& f){
	return "(" + f + ")*";
}

std::string decompile(const DFA& dfa){
	static const std::set<char> special_symbols = {'(', ')', '{', '}', ',', '+', '?', '*', '|', ':', '#'};

	size_t N = dfa.transitions.size();
	std::vector<std::vector<std::string>> R(N, std::vector<std::string>(N, ""));
	for (unsigned state=0; state<N; ++state){
		if (dfa.transitions[state].empty()){
			continue;
		}
		for (const auto& [ch, to] : dfa.transitions[state]){
			std::string symbol(1, ch);
			if (special_symbols.contains(ch)){
				symbol = '#' + symbol;
			}
			if (R[state][to].empty()){
				R[state][to] = symbol;
			}
			else{
				R[state][to] = regex_union(R[state][to], symbol);
			}
		}
	}
	
	for (unsigned k = 0; k < N; ++k)
    	for (unsigned i = 0; i < N; ++i)
			for (unsigned j = 0; j < N; ++j) {
				if (R[i][k].empty() || R[k][j].empty()){
					continue;
				}
				std::string mid;
				if (R[k][k].empty() == false){
					mid = regex_kleene(R[k][k]);
				}
				std::string path = R[i][k] + mid + R[k][j];
				if (R[i][j].empty())
					R[i][j] = path;
				else
					R[i][j] = regex_union(R[i][j], path);
			}

	std::vector<std::string> parts;
	for (unsigned acc : dfa.accepting_states) {
		if (!R[dfa.start_state][acc].empty()){
			parts.push_back(R[dfa.start_state][acc]);
		}
	}
	if (parts.empty()){
		return "";
	}
	std::string regex = parts[0];
	for (int i = 1; i < parts.size(); ++i){
		regex = regex_union(regex, parts[i]);
	}
	if (dfa.accepting_states.contains(dfa.start_state)){
		regex = "(" + regex + ")?";
	}
	return regex;
}

}