#include "dfa.hpp"
#include <set>
#include <queue>
#include <algorithm>
#include <map>

namespace myre
{

DFA DFABuilder::build_complement(const DFA& dfa){
	DFA complement;
	complement.transitions = dfa.transitions;
	for (int i = 0; i<dfa.transitions.size(); ++i){
		if (dfa.accepting_states.contains(i) == false){
			complement.accepting_states.insert(i);
		}
	}
	if (dfa.accepting_states.contains(dfa.null_state) == false){
		complement.accepting_states.insert(dfa.null_state);
	}
	complement.start_state = dfa.start_state;
	return complement;
}

DFA DFABuilder::build_intersection(const DFA& dfa1, const DFA& dfa2){
	DFA intersection;
	using NewState = std::pair<unsigned, unsigned>;

	std::vector<char> alphabet;
	for (const auto& tlist : dfa1.transitions){
		for (const auto& [symbol, to] : tlist){
			alphabet.push_back(symbol);
		}
	}
	for (const auto& tlist : dfa2.transitions){
		for (const auto& [symbol, to] : tlist){
			alphabet.push_back(symbol);
		}
	}
	std::map<NewState, unsigned> new_state_to_ind;
	std::queue<NewState> queue;

	auto get_or_create_state = [&](unsigned s1, unsigned s2) -> unsigned {
		auto p = std::make_pair(s1, s2);
		if (new_state_to_ind.contains(p)) return new_state_to_ind[p];
		unsigned index = intersection.transitions.size();
		new_state_to_ind[p] = index;
		intersection.transitions.emplace_back();
		if (dfa1.accepting_states.contains(s1) && dfa2.accepting_states.contains(s2))
			intersection.accepting_states.insert(index);
		queue.push(p);
		return index;
	};

	intersection.start_state = get_or_create_state(dfa1.start_state, dfa2.start_state);
	while (!queue.empty()){
		NewState current = queue.front(); queue.pop();
		unsigned s1 = current.first, s2 = current.second, current_index = new_state_to_ind[current];
		for (char c : alphabet){
			if (dfa1.transitions[s1].contains(c) and dfa2.transitions[s2].contains(c)){
				unsigned next_ind = get_or_create_state(dfa1.transitions[s1].at(c), dfa2.transitions[s2].at(c));
				intersection.transitions[current_index].insert({c, next_ind});
			}
		}
	}
	if (dfa1.accepting_states.contains(dfa1.null_state) and dfa2.accepting_states.contains(dfa2.null_state)){
		intersection.accepting_states.insert(intersection.null_state);
	}
	return intersection;
}

} // namespace myre
