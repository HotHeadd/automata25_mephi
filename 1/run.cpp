#include <iostream>
#include "RegexResolver.hpp"
#include "CorrectGenerator.hpp"
#include "IncorrectGenerator.hpp"

using namespace resolvers;

int main(){
    IncorrectGenerator gen(1, 5);
    for (auto i=0; i<5; ++i){
        std::cout << gen.gen_string() << std::endl;
    }
    RegexResolver resolver = RegexResolver();
    resolver.process_file("z_files/in.txt", "z_files/out.txt");
    return 0;
}