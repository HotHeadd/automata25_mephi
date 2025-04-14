#pragma once

#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <vector>

namespace myre
{
enum class TokenType{
	LPAR,
	RPAR,
	KLEENE,
	OR,
	CONCAT,
	CHAR,
	EPSYLON,
	NONE
};

enum class NodeType{
	KLEENE,
	OR,
	CONCAT,
	CHAR,
	EOS,
	EPSYLON
};

struct Token;
struct SyntaxNode;

class SetHandler {
public:
	static std::vector<char> symbols;
	static unsigned number_count;
	static std::unordered_map<unsigned, std::unordered_set<unsigned>> followpos;
	
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
	std::unordered_set<unsigned> first_pos;
	std::unordered_set<unsigned> last_pos;

	SyntaxNode(NodeType type_i, std::shared_ptr<SyntaxNode> left_kid = nullptr, std::shared_ptr<SyntaxNode> right_kid = nullptr)
	: type(type_i), left(left_kid), right(right_kid) {
		SetHandler::deduce_sets(this);
	}
	SyntaxNode(NodeType type_i, char value_i)
		: type(type_i), value(value_i) {
			SetHandler::deduce_sets(this);
	}
	SyntaxNode(std::shared_ptr<Token> token, std::shared_ptr<SyntaxNode> left_kid = nullptr, std::shared_ptr<SyntaxNode> right_kid = nullptr);

	
};

struct Token{
	TokenType type;
	char value='\0';
	unsigned start, finish;

	Token(TokenType type_i, char value_i='\0')
		: type(type_i), value(value_i) {}
	Token(TokenType type_i, unsigned value_left, unsigned value_right) 
		: type(type_i), start(value_left), finish(value_right) {}

	std::shared_ptr<SyntaxNode> to_node();
};

} // namespace myre
