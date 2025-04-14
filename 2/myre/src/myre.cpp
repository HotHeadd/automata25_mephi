#include "myre.hpp"

namespace myre
{

bool search(const std::string& expr, Regex& regex){
	DFA dfa = regex.compile();
	for (int ind=0; ind<expr.size(); ++ind){
		int curr_ind = ind;
		State curr_state = dfa.ind_to_state[0];
		while (curr_ind < expr.size()){
			bool no_tranz = false;
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
