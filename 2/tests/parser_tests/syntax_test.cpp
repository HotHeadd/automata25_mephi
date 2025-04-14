#include <gtest/gtest.h>
#include "parser.hpp"

using namespace myre;

TEST(tokenize, parentheses){
	RegexParser parser;
	ASSERT_THROW(parser.tokenize("(aga))"), ParenthesesError);
}

TEST(tokenize, range){
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
