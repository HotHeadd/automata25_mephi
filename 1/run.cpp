#include <iostream>
#include "RegexResolver.hpp"

using namespace resolvers;

int main(){
    RegexResolver resolver = RegexResolver();
    resolver.process_file("z_filez/in.txt", "z_files/out.txt");
    return 0;
}