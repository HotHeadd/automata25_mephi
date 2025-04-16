#include <gtest/gtest.h>
#include "parser.hpp"

using namespace myre;

TEST(tokenize, parentheses){
	RegexParser parser;
	ASSERT_THROW(parser.tokenize("(aga))"), ParenthesesError);
	ASSERT_THROW(parser.tokenize("((abc|d)"), ParenthesesError);

	ASSERT_NO_THROW(parser.tokenize("(aga|(cd))"));
}

TEST(tokenize, range_errors){
	RegexParser parser;
	ASSERT_THROW(parser.tokenize("a{3,2}"), RangeError);
	ASSERT_THROW(parser.tokenize("a{,3}"), SyntaxError);
	ASSERT_THROW(parser.tokenize("a{bc,3}"), SyntaxError);
	ASSERT_THROW(parser.tokenize("a{2,3"), ParenthesesError);

	ASSERT_NO_THROW(parser.tokenize("a2,3}"));
	ASSERT_NO_THROW(parser.tokenize("a{2,3}"));
	ASSERT_NO_THROW(parser.tokenize("a{3,}"));
	ASSERT_NO_THROW(parser.tokenize("a{2}"));
}

TEST(tokenize, shielding){
	RegexParser parser;
	ASSERT_THROW(parser.tokenize("ab#"), SyntaxError);

	std::list<std::shared_ptr<Token>> expected = {
		std::make_shared<Token>(TokenType::CHAR, 'a'),
		std::make_shared<Token>(TokenType::CONCAT),
		std::make_shared<Token>(TokenType::CHAR, '('),
		std::make_shared<Token>(TokenType::CONCAT),
		std::make_shared<Token>(TokenType::CHAR, ')')
	};

	std::string expr = "a#(#)";
	auto curr = expected.begin();
	for (auto& elem : parser.tokenize(expr)){
		ASSERT_EQ((*curr)->type, elem->type);
		if ((*curr)->type == TokenType::CHAR){
			ASSERT_EQ((*curr)->value, elem->value);
		}
		++curr;
	}
}

TEST(tokenize, range_no_brackets){
	RegexParser parser;
	std::string expr = "a{2,3}";

	std::list<std::shared_ptr<Token>> expected = {
		std::make_shared<Token>(TokenType::CHAR, 'a'),
		std::make_shared<Token>(TokenType::CONCAT),
		std::make_shared<Token>(TokenType::CHAR, 'a'),
		std::make_shared<Token>(TokenType::CONCAT),
		std::make_shared<Token>(TokenType::LPAR),
		std::make_shared<Token>(TokenType::CHAR, 'a'),
		std::make_shared<Token>(TokenType::OR),
		std::make_shared<Token>(TokenType::RPAR),
	};

	auto curr = expected.begin();
	for (auto& elem : parser.tokenize(expr)){
		ASSERT_EQ((*curr)->type, elem->type);
		if ((*curr)->type == TokenType::CHAR){
			ASSERT_EQ((*curr)->value, elem->value);
		}
		++curr;
	}
	ASSERT_EQ(curr, expected.end());
}

TEST(tokenize, range_single_value){
	RegexParser parser;
	std::string expr = "ab{2}";

	std::list<std::shared_ptr<Token>> expected = {
		std::make_shared<Token>(TokenType::CHAR, 'a'),
		std::make_shared<Token>(TokenType::CONCAT),
		std::make_shared<Token>(TokenType::CHAR, 'b'),
		std::make_shared<Token>(TokenType::CONCAT),
		std::make_shared<Token>(TokenType::CHAR, 'b'),
	};

	auto curr = expected.begin();
	for (auto& elem : parser.tokenize(expr)){
		ASSERT_EQ((*curr)->type, elem->type);
		if ((*curr)->type == TokenType::CHAR){
			ASSERT_EQ((*curr)->value, elem->value);
		}
		++curr;
	}
	ASSERT_EQ(curr, expected.end());
}

TEST(tokenize, range_with_brackets){
	RegexParser parser;
	std::string expr = "(ab){2,3}";

	std::list<std::shared_ptr<Token>> expected = {
		std::make_shared<Token>(TokenType::LPAR),
		std::make_shared<Token>(TokenType::CHAR, 'a'),
		std::make_shared<Token>(TokenType::CONCAT),
		std::make_shared<Token>(TokenType::CHAR, 'b'),
		std::make_shared<Token>(TokenType::RPAR),
		std::make_shared<Token>(TokenType::CONCAT),
		std::make_shared<Token>(TokenType::LPAR),
		std::make_shared<Token>(TokenType::CHAR, 'a'),
		std::make_shared<Token>(TokenType::CONCAT),
		std::make_shared<Token>(TokenType::CHAR, 'b'),
		std::make_shared<Token>(TokenType::RPAR),
		std::make_shared<Token>(TokenType::CONCAT),
		std::make_shared<Token>(TokenType::LPAR),
		std::make_shared<Token>(TokenType::LPAR),
		std::make_shared<Token>(TokenType::CHAR, 'a'),
		std::make_shared<Token>(TokenType::CONCAT),
		std::make_shared<Token>(TokenType::CHAR, 'b'),
		std::make_shared<Token>(TokenType::RPAR),
		std::make_shared<Token>(TokenType::OR),
		std::make_shared<Token>(TokenType::RPAR),
	};

	auto curr = expected.begin();
	for (auto& elem : parser.tokenize(expr)){
		ASSERT_EQ((*curr)->type, elem->type);
		if ((*curr)->type == TokenType::CHAR){
			ASSERT_EQ((*curr)->value, elem->value);
		}
		++curr;
	}
	ASSERT_EQ(curr, expected.end());
}