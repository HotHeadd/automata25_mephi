#include <iostream>
#include "RegexResolver.hpp"
#include "CorrectGenerator.hpp"
#include "IncorrectGenerator.hpp"

using namespace resolvers;

int main(){
    IncorrectGenerator gen(1, 5);
    CorrectGenerator gen2(1, 5);
    RegexResolver resolver = RegexResolver();
    for (auto i=0; i<5; ++i){
        std::string wrong = gen.gen_string();
        if (resolver.is_suitable(wrong)){
            std::cout << "NE POLUCHILOS\n";
        }
        std::string right = gen2.gen_string();
        if (!resolver.is_suitable(right)){
            std::cout << "NE POLUCHILOS\n";
        }
    }
    resolver.process_file("z_files/in.txt", "z_files/out.txt");
    return 0;
}