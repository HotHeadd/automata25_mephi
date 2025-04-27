#include "context.hpp"
#include "syntax_node.hpp"

namespace myre
{

ContextIndex Context::emplace_node(NodeType type, ContextIndex left, ContextIndex right){
	nodes.emplace_back(
		type,
		*this,
		left,
		right
	);
	return nodes.size()-1;
};

ContextIndex Context::emplace_node(NodeType type, char value_i){
	nodes.emplace_back(
		type,
		*this,
		value_i
	);
	return nodes.size()-1;
};

SyntaxNode& Context::get_node(ContextIndex index){
	return nodes[index];
}

void Context::deduce_sets(SyntaxNode* node){
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
		SyntaxNode& left = get_node(node->left);
		node->is_nullable = true;
		node->first_pos = left.first_pos;
		node->last_pos = left.last_pos;
		handle_kleene_followpos(node);
	}
	else if (node->type == NodeType::OR){
		SyntaxNode& left = get_node(node->left);
		SyntaxNode& right = get_node(node->right);
		node->is_nullable = left.is_nullable or right.is_nullable;
		node->first_pos = left.first_pos; // объединение множеств
		node->first_pos.insert(right.first_pos.begin(), right.first_pos.end());
		node->last_pos = left.last_pos;
		node->last_pos.insert(right.last_pos.begin(), right.last_pos.end());
	}
	else if (node->type == NodeType::CONCAT){
		SyntaxNode& left = get_node(node->left);
		SyntaxNode& right = get_node(node->right);
		node->is_nullable = left.is_nullable and right.is_nullable;
		node->first_pos = left.first_pos;
		node->last_pos = right.last_pos;
		if (left.is_nullable){
			node->first_pos.insert(right.first_pos.begin(), right.first_pos.end());
		}
		if (right.is_nullable){
			node->last_pos.insert(left.last_pos.begin(), left.last_pos.end());
		}
		handle_concat_followpos(node);
	}
}

void Context::handle_kleene_followpos(SyntaxNode* node){
	for (unsigned pos : node->last_pos){
		followpos[pos].insert(node->first_pos.begin(), node->first_pos.end());
	}
}
void Context::handle_concat_followpos(SyntaxNode* node){
	SyntaxNode& left = get_node(node->left);
	SyntaxNode& right = get_node(node->right);
	for (unsigned pos : left.last_pos){
		followpos[pos].insert(right.first_pos.begin(), right.first_pos.end());
	}
}

} // namespace myre
