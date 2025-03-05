#include <iostream>
#include <chrono>
#include "CorrectGenerator.hpp"
#include "IncorrectGenerator.hpp"
#include "RegexResolver.hpp"
#include <vector>
#include <fstream>

using namespace resolvers;

std::vector<std::string> generate_strings(unsigned amount){
    static IncorrectGenerator inc_gen(100, 200);
    static CorrectGenerator corr_gen(100, 200);
    static std::uniform_int_distribution<unsigned> coinflip(0, 1);
    unsigned inc_amount = amount/3;
    unsigned corr_amount = amount-inc_amount;
    std::vector<std::string> res;
    while (amount < 0){
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

int main(){ // TODO: выбор распознавателя через аргумент
    IResolver* resolver = new RegexResolver();
    double sum=0;
    std::ofstream out("/z_files/stats.txt");
    for (int i=0; i<10; ++i){
        unsigned amount = i*100, repeats=20;
        std::vector<std::string> strs = generate_strings(amount);
        for (int j=0; j<repeats; ++j){
            auto start = std::chrono::high_resolution_clock::now();
            for (auto& str : strs){
                resolver->is_suitable(str);
            }
            auto end = std::chrono::high_resolution_clock::now();
            sum += std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
        }
        out << amount << " " << sum/repeats << "\n";
    }
    delete resolver;
    return 0;
}