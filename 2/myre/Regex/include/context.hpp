#pragma once

#include <unordered_map>
#include <set>
#include <vector>
#include <stdexcept>
#include "syntax_node.hpp"

namespace myre
{


class Context {
public:
	std::unordered_map<unsigned, std::set<unsigned>> followpos;
	std::unordered_map<char, std::set<unsigned>> symbols;
	std::vector<SyntaxNode> nodes;

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
		nodes.clear();
	}

	void deduce_sets(SyntaxNode* node);
	void handle_kleene_followpos(SyntaxNode* node);
	void handle_concat_followpos(SyntaxNode* node);
	
	ContextIndex emplace_node(NodeType type, ContextIndex left = -1, ContextIndex right=-1);
	ContextIndex emplace_node(NodeType type, char value_i);
	SyntaxNode& get_node(ContextIndex index);

private:
	unsigned number_count;
};

} // namespace myre


