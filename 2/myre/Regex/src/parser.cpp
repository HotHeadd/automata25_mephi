#include "parser.hpp"
#include <cctype>

namespace myre
{

std::shared_ptr<SyntaxNode> RegexParser::parse(const std::string& regex){
	regex_ = regex;
	SetHandler::reset();
	pos = 0;
	std::shared_ptr<SyntaxNode> node = parse_expression();
	if (pos != regex.size()) throw SyntaxError(regex);
	return std::make_shared<SyntaxNode>(
				NodeType::CONCAT, 
				node, 
				std::make_shared<SyntaxNode>(NodeType::EOS)
	);
}

std::shared_ptr<SyntaxNode> RegexParser::parse_expression(){
	std::shared_ptr<SyntaxNode> node = parse_term();
	std::shared_ptr<SyntaxNode> right;
	if (!node) {
		node = std::make_shared<SyntaxNode>(NodeType::EPSYLON);
	}
	while (consume_if_match('|')){
		right = parse_term();
		if (!right) {
			right = std::make_shared<SyntaxNode>(NodeType::EPSYLON);
		}
		node = std::make_shared<SyntaxNode>(NodeType::OR, node, right);
	}
	return node;
}

std::shared_ptr<SyntaxNode> RegexParser::parse_term(){
	std::shared_ptr<SyntaxNode> node = nullptr, right;
	while (!tokens.empty() and tokens.front()->type == TokenType::CONCAT){
		char next_char = peek();
		if (next_char == '\0' or next_char == ')' or next_char == '|'){
			break;
		}
		right = parse_atom();
		node = std::make_shared<SyntaxNode>(NodeType::CONCAT, node, right);
	}
	return node;
}

std::shared_ptr<SyntaxNode> RegexParser::parse_atom(){
	std::shared_ptr<SyntaxNode> node;
	std::shared_ptr<Token> token = consume();
	if (token->type == TokenType::LPAR){
		node = parse_expression();
		if (consume()->type != TokenType::RPAR){
			throw SyntaxError(regex_);
		}
	}
	else if (token->type != TokenType::CHAR and token->type != TokenType::EPSYLON){
		throw SyntaxError(regex_);
	}
	else{
		node = std::make_shared<SyntaxNode>(token);
	}
	if (!tokens.empty() and tokens.front()->type == TokenType::KLEENE){
		std::shared_ptr<SyntaxNode> prev_node = node;
		node = std::make_shared<SyntaxNode>(consume(), prev_node);
	}
	if (!tokens.empty() and tokens.front()->type == TokenType::KLEENE){
		throw SyntaxError(regex_);
	}
	return node;
}

std::list<std::shared_ptr<Token>> RegexParser::	transform_range(unsigned lower, unsigned upper, 
															   std::list<std::shared_ptr<Token>>& tokens,
															   TokenType& prev_type){
	std::list<std::shared_ptr<Token>> new_tokens;
	std::list<std::shared_ptr<Token>> old_tokens;
	if (tokens.empty()){
		throw SyntaxError(regex_);
	}
	old_tokens.push_front(tokens.back());
	tokens.pop_back();
	if (old_tokens.back()->type == TokenType::RPAR){
		int braces = 1;
		while (old_tokens.front()->type != TokenType::LPAR or braces > 0){
			if (tokens.back()->type == TokenType::LPAR){
				--braces;
			}
			if (tokens.back()->type == TokenType::RPAR){
				++braces;
			}
			old_tokens.push_front(tokens.back());
			tokens.pop_back();
		}

	}
	bool first_token = true;
	for (int i=0; i<lower; ++i){
		if (!first_token){
			new_tokens.push_back(std::make_shared<Token>(TokenType::CONCAT));
		}
		new_tokens.insert(new_tokens.end(), old_tokens.begin(), old_tokens.end());
		first_token = false;
	}
	if (upper == INF){
		new_tokens.push_back(std::make_shared<Token>(TokenType::CONCAT));
		new_tokens.push_back(std::make_shared<Token>(TokenType::LPAR));
		new_tokens.insert(new_tokens.end(), old_tokens.begin(), old_tokens.end());
		new_tokens.push_back(std::make_shared<Token>(TokenType::RPAR));
		new_tokens.push_back(std::make_shared<Token>(TokenType::KLEENE));
	}
	else{
		for (int i=lower; i<upper; ++i){
			if (!first_token){
				new_tokens.push_back(std::make_shared<Token>(TokenType::CONCAT));
			}
			new_tokens.push_back(std::make_shared<Token>(TokenType::LPAR));
			new_tokens.insert(new_tokens.end(), old_tokens.begin(), old_tokens.end());
			new_tokens.push_back(std::make_shared<Token>(TokenType::OR));
			new_tokens.push_back(std::make_shared<Token>(TokenType::RPAR));
			first_token = false;
		}
	}
	prev_type = TokenType::NONE;
	new_tokens.push_front(std::make_shared<Token>(TokenType::LPAR));
	new_tokens.push_back(std::make_shared<Token>(TokenType::RPAR));
	return new_tokens;
}

std::pair<unsigned, unsigned> RegexParser::parse_range(const std::string& regex, int& i, std::list<std::shared_ptr<Token>>& tokens){
	std::string lower, upper;
	bool is_upper = false;
	unsigned i_lower=0, i_upper=0;
	while (i < regex.size()){
		char sym = regex[i];
		if (sym == '}'){
			break;
		}
		else if (sym == ',' and is_upper == false){
			is_upper = true;
		}
		else if (std::isdigit(sym)){
			if (is_upper){
				upper += sym;
			}
			else{
				lower += sym;
			}
		}
		else if (sym != '{'){
			throw SyntaxError(regex);
		}
		++i;
	}
	if (i>=regex.size()){
		throw ParenthesesError(regex);
	}
	if (lower.empty()){
		throw SyntaxError(regex);
	}
	if (upper.empty()){
		if (is_upper == true){
			i_upper = INF;
		}
		else{
			upper = lower;
		}
	}
	try{
		i_lower = std::stoull(lower);
		if (!upper.empty()){
			i_upper = std::stoull(upper);
		}
	}
	catch (std::invalid_argument& e){
		throw SyntaxError(regex);
	}
	if (i_lower > i_upper){
		throw RangeError(i_lower, i_upper);
	}
	return {i_lower, i_upper};
}

char RegexParser::peek() const {
	return pos < regex_.size() ? regex_[pos] : '\0';
}

char RegexParser::next() {
	return pos < regex_.size() ? regex_[pos++] : '\0';
}

bool RegexParser::consume_if_match(char c) {
	if (peek() == c) {
		++pos;
		return true;
	}
	return false;
}
} // namespace myre
