#include "nodes.hpp"

namespace myre
{
unsigned SetHandler::number_count = 0;
std::unordered_map<char, std::set<unsigned>> SetHandler::symbols = {};
std::unordered_map<unsigned, std::set<unsigned>> SetHandler::followpos = {};

SyntaxNode::SyntaxNode(std::shared_ptr<Token> token, std::shared_ptr<SyntaxNode> left_kid, std::shared_ptr<SyntaxNode> right_kid) 
						: left(left_kid), right(right_kid){
	if (token->type == TokenType::KLEENE){
		type = NodeType::KLEENE;
	}
	else if (token->type == TokenType::CHAR){
		type = NodeType::CHAR;
		value = token->value;
	}
	else if (token->type == TokenType::EPSYLON){
		type = NodeType::EPSYLON;
	}
	else if (token->type == TokenType::OR){
		type = NodeType::OR;
	}
	else if (token->type == TokenType::CONCAT){
		type = NodeType::CONCAT;
	}
	else{
		throw std::runtime_error("Unable to transform token");
	}
	SetHandler::deduce_sets(this);
}

void SetHandler::deduce_sets(SyntaxNode* node){
	if (node->type == NodeType::EPSYLON){
		node->is_nullable = true;
	}
	else if (node->type == NodeType::CHAR or node->type == NodeType::EOS){
		node->number = number_count++;
		if (node->type == NodeType::EOS){
			symbols['\0'].insert(node->number);
		}
		else{
			symbols[node->value].insert(node->number);
		}
		node->is_nullable = false;
		node->first_pos.insert(node->number);
		node->last_pos.insert(node->number);
	}
	else if (node->type == NodeType::KLEENE){
		node->is_nullable = true;
		node->first_pos = node->left->first_pos;
		node->last_pos = node->left->last_pos;
		SetHandler::handle_kleene_followpos(node);
	}
	else if (node->type == NodeType::OR){
		node->is_nullable = node->left->is_nullable or node->right->is_nullable;
		node->first_pos = node->left->first_pos; // объединение множеств
		node->first_pos.insert(node->right->first_pos.begin(), node->right->first_pos.end());
		node->last_pos = node->left->last_pos;
		node->last_pos.insert(node->right->last_pos.begin(), node->right->last_pos.end());
	}
	else if (node->type == NodeType::CONCAT){
		node->is_nullable = node->left->is_nullable and node->right->is_nullable;
		node->first_pos = node->left->first_pos;
		node->last_pos = node->right->last_pos;
		if (node->left->is_nullable){
			node->first_pos.insert(node->right->first_pos.begin(), node->right->first_pos.end());
		}
		if (node->right->is_nullable){
			node->last_pos.insert(node->left->last_pos.begin(), node->left->last_pos.end());
		}
		SetHandler::handle_concat_followpos(node);
	}
}

void SetHandler::handle_kleene_followpos(SyntaxNode* node){
	for (unsigned pos : node->last_pos){
		followpos[pos].insert(node->first_pos.begin(), node->first_pos.end());
	}
}
void SetHandler::handle_concat_followpos(SyntaxNode* node){
	for (unsigned pos : node->left->last_pos){
		followpos[pos].insert(node->right->first_pos.begin(), node->right->first_pos.end());
	}
}

} // namespace myre
