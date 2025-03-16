#include <iostream>
#include <chrono>
#include "CorrectGenerator.hpp"
#include "IncorrectGenerator.hpp"
#include "RegexResolver.hpp"
#include <fstream>
#include <cmath>

using namespace resolvers;

std::vector<std::string> generate_strings(unsigned amount){
    static IncorrectGenerator inc_gen(10, 20);
    static CorrectGenerator corr_gen(10, 20);
    static std::uniform_int_distribution<unsigned> coinflip(0, 1);
    unsigned inc_amount = amount/3;
    unsigned corr_amount = amount-inc_amount;
    std::vector<std::string> res;
    while (amount > 0){
        --amount;
        if (amount%2==0){
            res.push_back(inc_gen.gen_string());
        }
        else{
            res.push_back(corr_gen.gen_string());
        }
    }
    return res;
    
}

int main(int argc, char* argv[]){ // TODO: выбор распознавателя через аргумент
    IResolver* resolver;
    std::string filename;
    if (argc > 0){
        std::string mode = argv[1];
        switch (std::stoi(mode)){
            case 1:
                resolver = new RegexResolver();
                filename = "regex";
                break;
            case 2:
                break;
            case 3:
                break;
            default:
                std::cout << "Wrong mode!\n";
                return 0;
        }
    }
    std::unordered_map<std::string, std::string> map;
    double sum=0;
    std::ofstream out("z_files/stats" + filename + ".txt");
    for (int i=1; i<10; ++i){
        unsigned amount = std::pow(4, i), repeats=10;
        std::cout << amount << std::endl;
        std::vector<std::string> strs = generate_strings(amount);
        for (int j=0; j<repeats; ++j){
            auto start = std::chrono::high_resolution_clock::now();
            for (auto& str : strs){
                resolver->is_suitable(str, map);
            }
            auto end = std::chrono::high_resolution_clock::now();
            sum += std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
        }
        out << amount << " " << sum/repeats << "\n";
    }
    delete resolver;
    return 0;
}