#include <iostream>
#include <chrono>
#include "CorrectGenerator.hpp"
#include "IncorrectGenerator.hpp"
#include "RegexResolver.hpp"
#include "SMCResolver.hpp"
#include <fstream>
#include <cmath>

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
    if (argc > 1){
        std::string mode = argv[1];
        switch (std::stoi(mode)){
            case 1:
                resolver = new RegexResolver();
                filename = "regex";
                break;
            case 2:
                resolver = new SMCResolver();
                filename = "SMC";
                break;
            case 3:
                break;
            default:
                std::cout << "Wrong mode!\n";
                return 0;
        }
    }
    else{
        std::cout << "No resolver chosen\n";
        return 1;
    }
    std::unordered_map<std::string, std::string> map;
    double sum=0;
    std::ofstream out("z_files/stats_" + filename + ".txt");
    for (int i=8; i<21; ++i){
        unsigned amount = std::pow(2, i), repeats=10;
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