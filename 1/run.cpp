#include <iostream>
#include "RegexResolver.hpp"

using namespace resolvers;

int main(){
    RegexResolver resolver = RegexResolver();
    std::cout << "hello, world!\n";
    std::cout << resolver.is_suitable("abcdf") << std::endl;
    return 0;
}