#include "dfa.hpp"

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
	std::unordered_map<unsigned, std::set<unsigned>>& followpos = SetHandler::followpos;
	DFA automaton;
	automaton.ind_to_state.emplace_back(root->first_pos);
	automaton.set_to_ind.emplace(root->first_pos, 0);
	int i=0;
	while (i < automaton.ind_to_state.size()){
		for (auto sym_pair : SetHandler::symbols){
			std::set<unsigned> new_pos_set;
			std::set<unsigned> intersection = form_intersection(automaton.ind_to_state[i].set_pos, sym_pair.second);
			for (auto pos : intersection){
				new_pos_set.insert(followpos[pos].begin(), followpos[pos].end());
			}
			if (!new_pos_set.empty() and automaton.set_to_ind.contains(new_pos_set) == false){
				automaton.set_to_ind.emplace(new_pos_set, automaton.ind_to_state.size());
				automaton.ind_to_state.emplace_back(new_pos_set);
			}
			if (automaton.set_to_ind.contains(new_pos_set)){
				int index = automaton.set_to_ind[new_pos_set];
				automaton.ind_to_state[i].transitions.emplace_back(sym_pair.first, index);
			}
		}
		++i;
	}
	return automaton;
}

} // namespace myre


