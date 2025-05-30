#include "dfa.hpp"
#include <queue>
#include <algorithm>
#include <map>

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

DFA DFABuilder::buildDFA(ContextIndex root_ind, Context& context){
	std::queue<std::set<unsigned>> sets_q;
	std::map<std::set<unsigned>, int> set_to_ind;
	std::unordered_map<unsigned, std::set<unsigned>>& followpos = context.followpos;
	DFA automaton;
	SyntaxNode& root = context.get_node(root_ind);
	sets_q.push(root.first_pos);
	set_to_ind.emplace(root.first_pos, 0);
	automaton.transitions.push_back({});
	unsigned state_counter = 0;
	while (sets_q.empty() == false){
		std::set<unsigned> current_state = std::move(sets_q.front());
		sets_q.pop();
		if (current_state.contains(context.get_final_ind())){
			automaton.accepting_states.insert(set_to_ind[current_state]);
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
				automaton.transitions[set_to_ind[current_state]].insert({sym_pair.first, index});
			}
		}
	}
	return automaton;
}

DFA DFABuilder::minimize_dfa(const DFA& dfa) {
    using State = unsigned;
	std::vector<std::vector<State>> groups;
	std::vector<char> alphabet;
	std::map<std::vector<unsigned>, std::vector<State>> sign_to_state;
	std::map<State, unsigned> state_to_group;


	std::vector<State> A, NA;
	for (int i = 0; i<dfa.transitions.size(); ++i){
		if (dfa.accepting_states.contains(i)){
			A.push_back(i);
		}
		else{
			NA.push_back(i);
		}
	}

	if (!A.empty()){
		groups.push_back(std::move(A));
	}
	if (!NA.empty()){
		groups.push_back(std::move(NA));
	}
	for (const auto& tlist : dfa.transitions){
		for (const auto& [symbol, to] : tlist){
			alphabet.push_back(symbol);
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
					auto& tr_map = dfa.transitions[state];
					if (tr_map.contains(alphabet[i])){
						signature[i+1] = state_to_group[tr_map.at(alphabet[i])];
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
			for (auto& [symbol, to] : dfa.transitions[state]){
				auto& tr_map = new_dfa.transitions[i];
				new_dfa.transitions[i][symbol] = state_to_group[to];
			}
			if (dfa.accepting_states.contains(state)){
				new_dfa.accepting_states.insert(i);
			}
		}
	}
	if (dfa.accepting_states.contains(dfa.null_state)){
		new_dfa.accepting_states.insert(new_dfa.null_state);
	}
	return new_dfa;
}




} // namespace myre


