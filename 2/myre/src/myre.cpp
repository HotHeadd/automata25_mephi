#include "myre.hpp"
#include <optional>

namespace myre
{

DFA compile(const std::string& regex, bool optimize){
	RegexParser parser;
	DFABuilder builder;
	Context context;
	std::shared_ptr<SyntaxNode> tree = parser.parse(regex, context);
	DFA dfa = builder.buildDFA(tree, context);
	// if (optimize){
	// 	dfa = builder.minimize_dfa(dfa, context);
	// }
	return dfa;
}

bool fullmatch(const std::string& expr, DFA& dfa){
	unsigned curr_state = dfa.start_state;
	for (auto ch: expr){
		bool no_tranz = true;
		for (auto& tranz : dfa.transitions[curr_state]){
			if (tranz.symbol == ch){
				curr_state = tranz.to;
				no_tranz;
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
	unsigned start_state = dfa.start_state;
	if (dfa.accepting_states.contains(start_state)){
		match.begin = expr.begin();
		match.end = expr.begin()+1;
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
					no_tranz;
					if (dfa.accepting_states.contains(curr_state)){
						match.begin = expr.begin()+start_ind;
						match.end = expr.begin()+curr_ind+1;
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

auto make_lazy_search(const std::string& expr, DFA& dfa){
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
                bool matched;
                for (const auto& tranz : dfa.transitions[curr_state]) {
                    if (tranz.symbol == expr[curr_ind]) {
                        curr_state = tranz.to;
                        ++curr_ind;
                        matched = true;

                        if (dfa.accepting_states.contains(curr_state)) {
                            match.begin = expr.begin() + pos;
                            match.end = expr.begin() + curr_ind;
                            pos += 1;
                            return match;
                        }
                        break;
                    }
                }
                if (!matched) break;
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

bool fullmatch(const std::string& expr, const std::string& regex, bool optimize){
	DFA dfa = compile(regex, optimize);
	return fullmatch(expr, dfa);
}

bool search_first(const std::string& expr, const std::string& regex, Match& match, bool optimize){
	DFA dfa = compile(regex, optimize);
	return search_first(expr, dfa, match);
}

auto make_lazy_search(const std::string& expr, const std::string& regex, bool optimize){
	DFA dfa = compile(regex, optimize);
	return make_lazy_search(expr, dfa);
}
} // namespace myre
