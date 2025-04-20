#pragma once

#include <unordered_map>
#include <stdexcept>
#include <set>

namespace myre
{

class SyntaxNode;

class Context {
public:
	std::unordered_map<unsigned, std::set<unsigned>> followpos;
	std::unordered_map<char, std::set<unsigned>> symbols;
	unsigned get_final_ind(){
		if (!symbols.contains('\0')){
			throw std::runtime_error("No EOS symbol in set handler");
		}
		return *(symbols['\0'].begin());
	}

	void reset(){
		number_count = 0;
		symbols.clear();
		followpos.clear();
	}

	void deduce_sets(SyntaxNode* node);
	void handle_kleene_followpos(SyntaxNode* node);
	void handle_concat_followpos(SyntaxNode* node);
private:
	unsigned number_count;
};

} // namespace myre


