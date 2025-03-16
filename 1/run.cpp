#include <iostream>
#include <fstream>
#include "RegexResolver.hpp"
#include "CorrectGenerator.hpp"
#include "IncorrectGenerator.hpp"

using namespace resolvers;

void process_file(IResolver& resolver, const std::string& in_filename, const std::string& out_filename){
    std::string line;
    std::ifstream in(in_filename);
    if (!in.is_open()){
        throw std::runtime_error("Wrong input filename format");
    }
    std::ofstream out(out_filename);
    if (!out.is_open()){
        throw std::runtime_error("Wrong output filename format");
    }
    unsigned count=0, correct=0;
    while(std::getline(in, line)){
        ++count;
        std::unordered_map<std::string, std::string> tokens;
        if (resolver.is_suitable(line, tokens)){
            std::string& name = tokens["name"];
            std::string& amount = tokens["amount"];
            ++correct;
            if (!amount.size()){
                amount = std::to_string(std::count(line.begin(), line.end(),  ',')+1);
            }
            out << name << " - " << amount << "\n";
        }
    }
    out << "Correct: " << correct << "/" << count << "\n";
}

int main(){
    RegexResolver resolver = RegexResolver();
    CorrectGenerator gen(1, 5);
    std::unordered_map<std::string, std::string> map;
    for (int i=0; i<100; ++i){
        std::string res = gen.gen_string();
        std::cout << res << std::endl;
    }
    return 0;
}