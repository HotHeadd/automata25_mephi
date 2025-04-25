#include "dfa.hpp"
#include <queue>
#include <algorithm>

namespace myre
{

std::set<unsigned> DFABuilder::set_intersection(std::set<unsigned>& a, std::set<unsigned>& b){
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

DFA DFABuilder::buildDFA(std::shared_ptr<SyntaxNode> root, Context& context){
	std::queue<std::set<unsigned>> sets_q;
	std::map<std::set<unsigned>, int> set_to_ind;
	std::unordered_map<unsigned, std::set<unsigned>>& followpos = context.followpos;
	DFA automaton;
	sets_q.push(root->first_pos);
	set_to_ind.emplace(root->first_pos, 0);
	automaton.transitions.push_back({});
	unsigned state_counter = 0;
	while (sets_q.empty() == false){
		std::set<unsigned> current_state = std::move(sets_q.front());
		sets_q.pop();
		if (current_state.contains(context.get_final_ind())){
			automaton.accepting_states.insert(state_counter);
		}
		for (auto sym_pair : context.symbols){
			std::set<unsigned> new_pos_set;
			std::set<unsigned> intersection = set_intersection(current_state, sym_pair.second);
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

DFA DFABuilder::minimize_dfa(const DFA& dfa, Context& context) {
    using State = unsigned;
	std::vector<std::vector<State>> groups(2);
	std::vector<char> alphabet;
	std::map<std::vector<unsigned>, std::vector<State>> sign_to_state;
	std::map<State, unsigned> state_to_group;

	for (int i = 0; i<dfa.transitions.size(); ++i){
		if (dfa.accepting_states.contains(i)){
			groups[1].push_back(i);
		}
		else{
			groups[0].push_back(i);
		}
	}
	for (auto& [c, _] : context.symbols){
		if (c != '\0'){
			alphabet.push_back(c);
		}
	}
	bool changed = false;
	do{
		changed = false;
		state_to_group.clear();
		sign_to_state.clear();
		std::vector<unsigned> signature(alphabet.size()+1);
		for (unsigned i=0; i<groups.size(); ++i){
			for (auto& state : groups[i]){
				state_to_group[state] = i;
			}
		}
		for (auto& group : groups){
			for (auto& state : group){
				for (int i=0; i<alphabet.size(); ++i){
					auto tr_iter = std::find_if(dfa.transitions[state].begin(), dfa.transitions[state].end(), [&](const Transition& tr){
						return tr.symbol == alphabet[i];
					});
					if (tr_iter != dfa.transitions[state].end()){
						signature[i+1] = state_to_group[tr_iter->to];
					}
					else{
						signature[i+1] = -1;
					}
				}
				signature[0] = state_to_group[state];
				sign_to_state[signature].push_back(state);
			}
		}
		if (sign_to_state.size() != groups.size()){
			changed = true;
			groups.clear();
			int i = 0;
			for (auto& [sign, new_gr] : sign_to_state){
				groups.push_back(new_gr);
				++i;
			}
		}
		
	} while (changed);
	
	DFA new_dfa;
	new_dfa.start_state = state_to_group[dfa.start_state];
	new_dfa.transitions.resize(groups.size());
	for (unsigned i=0; i<groups.size(); ++i){
		for (auto& state : groups[i]){
			for (auto& tranz : dfa.transitions[state]){
				auto iter = std::find_if(new_dfa.transitions[i].begin(), new_dfa.transitions[i].end(), [&](const Transition& tr){
					return tr.symbol == tranz.symbol;
				});
				if (iter == new_dfa.transitions[i].end()){
					new_dfa.transitions[i].push_back({tranz.symbol, state_to_group[tranz.to]});
				}
			}
			if (dfa.accepting_states.contains(state)){
				new_dfa.accepting_states.insert(i);
			}
		}
	}
	return new_dfa;
}




} // namespace myre


