#include <iostream>
#include "RegexResolver.hpp"
#include "generator.hpp"

using namespace resolvers;

int main(){
    Generator gen(1, 5);
    for (auto i=0; i<5; ++i){
        std::cout << gen.gen_correct_string() << std::endl;
    }
    RegexResolver resolver = RegexResolver();
    resolver.process_file("z_files/in.txt", "z_files/out.txt");
    gen.gen_incorrect_string();
    return 0;
}