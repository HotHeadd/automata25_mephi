#include <gtest/gtest.h>
#include "myre.hpp"

using namespace myre;

// "a{2,3}{1,2}"
// "(((((((a))))))){2,3}"
// a{2,3}{1,2}{3,4}
// a{2,3}{0,2}{3,4}
// a{2,3}{4,4}{0,4}
// a{2,3}{3,4}{0,4}
// a{2,3}{4,4}
// a{2,3}{4,4}{0,2}
// a{1,1}
// a{1,1}{1,1}{1,1}{1,1}
// a{7,} 
// a{7,}* TODO: UNDERSTAND DFA
// a{7,8}*

TEST(mutiple_ranges, t1){
	
}