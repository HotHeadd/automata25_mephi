#include "myre.hpp"

namespace myre
{

bool search_first(const std::string& expr, DFA& dfa, Match& match){
	unsigned start_state = dfa.start_state;
	if (dfa.accepting_states.contains(start_state)){
		match.begin = expr.begin();
		match.end = expr.begin();
		return true;
	}
	for (int start_ind=0; start_ind<expr.size(); ++start_ind){
		int curr_ind = start_ind;
		unsigned curr_state = start_state;
		while (curr_ind < expr.size()){
			if (dfa.transitions[curr_state].contains(expr[curr_ind])){
				curr_state = dfa.transitions[curr_state][expr[curr_ind]];
				if (dfa.accepting_states.contains(curr_state)){
					match.begin = expr.begin()+start_ind;
					match.end = expr.begin()+curr_ind+1;
					return true;
				}
			}
			else{
				if (dfa.accepting_states.contains(dfa.null_state)){
					match.begin = expr.begin()+start_ind;
					match.end = expr.begin()+curr_ind+1;
					return true;
				}
				break;
			}
			++curr_ind;
		}
	}
	return false;
}

std::function<std::optional<Match>()> make_lazy_search(const std::string& expr, DFA& dfa){
	unsigned pos = 0;
	Match match;
	
	return [&expr, &dfa, pos, match]() mutable -> std::optional<Match> { // mutable чтобы можно было менять pos, match
		unsigned start_state = dfa.start_state;

		while (pos <= expr.size()) {
			unsigned curr_state = start_state;
			size_t curr_ind = pos;

			if (dfa.accepting_states.contains(curr_state)) {
				match.begin = expr.begin() + pos;
				match.end = expr.begin() + pos;
				++pos;
				return match;
			}

			while (curr_ind < expr.size()) {
				bool matched = false;
				if (dfa.transitions[curr_state].contains(expr[curr_ind])) {
					curr_state = dfa.transitions[curr_state][expr[curr_ind]];
					++curr_ind;
					matched = true;

					if (dfa.accepting_states.contains(curr_state)) {
						match.begin = expr.begin() + pos;
						match.end = expr.begin() + curr_ind;
						++pos;
						return match;
					}
				}
				else {
					if (dfa.accepting_states.contains(dfa.null_state)){
						match.begin = expr.begin()+pos;
						match.end = expr.begin()+curr_ind+1;
						++pos;
						return match;
					}
					break;
				};
			}
			++pos;
		}

		return std::nullopt;
	};
}


bool search(const std::string& expr, const std::string& regex, bool optimize){
	DFA dfa = compile(regex, optimize);
	Match match;
	return search_first(expr, dfa, match);
}

bool search(const std::string& expr, DFA& dfa){
	Match match;
	return search_first(expr, dfa, match);
}

std::function<std::optional<Match>()> make_lazy_search(const std::string& expr, const std::string& regex, bool optimize){
	DFA dfa = compile(regex, optimize);
	return make_lazy_search(expr, dfa);
}

bool search_first(const std::string& expr, const std::string& regex, Match& match, bool optimize){
	DFA dfa = compile(regex, optimize);
	return search_first(expr, dfa, match);
}

} // namespace myre


