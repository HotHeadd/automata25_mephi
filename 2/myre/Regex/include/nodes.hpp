#pragma once

#include <memory>
#include <unordered_map>
#include <set>
#include <vector>

namespace myre
{

enum class NodeType{
	KLEENE,
	OR,
	CONCAT,
	CHAR,
	EOS,
	EPSYLON
};

struct SyntaxNode;

class SetHandler {
public:
	static std::unordered_map<char, std::set<unsigned>> symbols;
	static unsigned number_count;
	static std::unordered_map<unsigned, std::set<unsigned>> followpos;
	
	static unsigned get_final_ind(){
		if (!symbols.contains('\0')){
			throw std::runtime_error("No EOS symbol in set handler");
		}
		return *(symbols['\0'].begin());
	}

	static void reset(){
		number_count = 0;
		symbols.clear();
		followpos.clear();
	}

	static void deduce_sets(SyntaxNode* node);
	static void handle_kleene_followpos(SyntaxNode* node);
	static void handle_concat_followpos(SyntaxNode* node);
};

struct SyntaxNode {
	NodeType type;
	char value='\0';
	std::shared_ptr<SyntaxNode> left=nullptr, right=nullptr;

	unsigned number;
	bool is_nullable;
	std::set<unsigned> first_pos;
	std::set<unsigned> last_pos;

	SyntaxNode(NodeType type_i, std::shared_ptr<SyntaxNode> left_kid = nullptr, std::shared_ptr<SyntaxNode> right_kid = nullptr)
	: type(type_i), left(left_kid), right(right_kid) {
		SetHandler::deduce_sets(this);
	}
	SyntaxNode(NodeType type_i, char value_i)
		: type(type_i), value(value_i) {
			SetHandler::deduce_sets(this);
	}
	
};

} // namespace myre
