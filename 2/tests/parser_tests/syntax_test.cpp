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
		std::make_shared<Token>(TokenType::CHAR, '('),
		std::make_shared<Token>(TokenType::CHAR, ')')
	};

	ASSERT_EQ(parser.tokenize("a#(#)"), expected);
}
