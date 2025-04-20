#include "dfa.hpp"
#include <queue>
#include <iostream>

namespace myre
{

std::set<unsigned> form_intersection(std::set<unsigned>& a, std::set<unsigned>& b){
	const std::set<unsigned>& smaller = (a.size() < b.size()) ? a : b;
    const std::set<unsigned>& larger  = (a.size() < b.size()) ? b : a;
	std::set<unsigned> result;
	for (auto elem : smaller){
		if (larger.contains(elem)){
			result.insert(elem);
		}
	}
	return result;
}

DFA DFABuilder::buildDFA(std::shared_ptr<SyntaxNode> root){
	std::queue<std::set<unsigned>> sets_q;
	std::map<std::set<unsigned>, int> set_to_ind;
	std::unordered_map<unsigned, std::set<unsigned>>& followpos = SetHandler::followpos;
	DFA automaton;
	sets_q.push(root->first_pos);
	set_to_ind.emplace(root->first_pos, 0);
	unsigned state_counter = 0;
	while (sets_q.empty() == false){
		std::set<unsigned> current_state = std::move(sets_q.front());
		sets_q.pop();
		if (current_state.contains(SetHandler::get_final_ind())){
			automaton.accepting_states.insert(state_counter);
		}
		for (auto sym_pair : SetHandler::symbols){
			std::set<unsigned> new_pos_set;
			std::set<unsigned> intersection = form_intersection(current_state, sym_pair.second);
			for (auto pos : intersection){
				new_pos_set.insert(followpos[pos].begin(), followpos[pos].end());
			}
			if (!new_pos_set.empty() and set_to_ind.contains(new_pos_set) == false){
				++state_counter;
				set_to_ind.emplace(new_pos_set, state_counter);
				sets_q.push(new_pos_set);
				automaton.transitions.push_back({});
			}
			if (set_to_ind.contains(new_pos_set)){
				unsigned index = set_to_ind[new_pos_set];
				automaton.transitions[set_to_ind[current_state]].push_back({sym_pair.first, index});
			}
		}
	}
	return automaton;
}

} // namespace myre


