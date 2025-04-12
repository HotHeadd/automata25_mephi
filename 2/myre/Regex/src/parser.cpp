#include "parser.hpp"
#include <cctype>

namespace myre
{

std::shared_ptr<SyntaxNode> RegexParser::parse(const std::string& regex){
	std::list<std::shared_ptr<Token>> pol_notation = tokenize(regex);
	// алгоритм разбиения в дерево
	return nullptr;
}

std::shared_ptr<Token> RegexParser::parse_range(const std::string& regex, int& i){
	std::string lower, upper;
	bool is_upper = false;
	unsigned i_lower=0, i_upper=0;
	while (i < regex.size()){
		++i;
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
		else {
			throw SyntaxError(regex);
		}
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
		i_lower = std::stoi(lower);
		if (!upper.empty()){
			i_upper = std::stoi(upper);
		}
	}
	catch (std::invalid_argument& e){
		throw SyntaxError(regex);
	}
	if (i_lower > i_upper){
		throw RangeError(i_lower, i_upper);
	}
	return std::make_shared<Token>(TokenType::RANGE, i_lower, i_upper);
}


std::list<std::shared_ptr<Token>> RegexParser::tokenize(const std::string& regex){
	std::list<std::shared_ptr<Token>> nodes;
	TokenType prev_type = TokenType::NONE;
	std::shared_ptr<Token> token;
	int i = 0, par_count=0;
	while (i < regex.size()){
		char sym = regex[i];
		if (sym == '('){
			++par_count;
			token = std::make_shared<Token>(TokenType::LPAR);
		}
		else if (sym == ')'){
			--par_count;
			if (par_count < 0){
				throw ParenthesesError(regex);
			}
			token = std::make_shared<Token>(TokenType::RPAR);
		}
		else if (sym == '|'){
			token = std::make_shared<Token>(TokenType::OR);
		}
		else if (sym == '*'){
			token = std::make_shared<Token>(TokenType::RANGE, 0, INF);
		}
		else if (sym == '+'){
			token = std::make_shared<Token>(TokenType::RANGE, 1, INF);
		}
		else if (sym == '?'){
			token = std::make_shared<Token>(TokenType::RANGE, 0, 1);
		}
		else if (sym == '{'){
			token = parse_range(regex, i);
		}
		else if (sym == '#'){
			++i;
			if (i < regex.size()){
				throw SyntaxError(regex);
			}
			token = std::make_shared<Token>(TokenType::CHAR, sym);

		}
		else {
			token = std::make_shared<Token>(TokenType::CHAR, sym);
		}
		if (prev_type == TokenType::CHAR or prev_type == TokenType::RPAR or prev_type == TokenType::RANGE){
			if (token->type == TokenType::CHAR or token->type == TokenType::LPAR){
				nodes.push_back(std::make_shared<Token>(TokenType::CONCAT));
			}
		}
		nodes.push_back(token);
		prev_type = token->type;
		++i;
	}
	if (par_count != 0){
		throw ParenthesesError(regex);
	}
	return nodes;
}


std::shared_ptr<SyntaxNode> Token::to_node(){
	if (type == TokenType::RANGE){
		return std::make_shared<SyntaxNode>(NodeType::RANGE, start, finish);
	}
	if (type == TokenType::CHAR){
		return std::make_shared<SyntaxNode>(NodeType::CHAR, value);
	}
	if (type == TokenType::OR){
		return std::make_shared<SyntaxNode>(NodeType::OR);
	}
	if (type == TokenType::CONCAT){
		return std::make_shared<SyntaxNode>(NodeType::CONCAT);
	}
	throw std::runtime_error("Unable to transform token");
}

} // namespace myre
