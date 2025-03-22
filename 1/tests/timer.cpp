#include <iostream>
#include <chrono>
#include "Generator.hpp"
#include "RegexResolver.hpp"
#include "SMCResolver.hpp"
#include "FlexResolver.hpp"
#include <fstream>
#include <cmath>

std::vector<std::string> generate_strings(unsigned amount, Generator& gen){
    static std::uniform_int_distribution<unsigned> coinflip(0, 1);
    unsigned inc_amount = amount/3;
    unsigned corr_amount = amount-inc_amount;
    std::vector<std::string> res;
    while (amount > 0){
        --amount;
        if (amount%2==0){
            res.push_back(gen.gen_inc_string());
        }
        else{
            res.push_back(gen.gen_corr_string());
        }
    }
    return res;
    
}

int main(int argc, char* argv[]){ // TODO: выбор распознавателя через аргумент
    IResolver* resolver;
    std::string filename;
    int power_start=8, power_end=21;
    Generator gen(1000, 3000);
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
                resolver = new FlexResolver();
                filename = "Flex";
                break;
            default:
                std::cout << "Wrong mode!\n";
                return 1;
        }
    }
    else{
        std::cout << "No resolver chosen\n";
        return 2;
    }
    if (argc > 2){
        std::string mode = argv[2];
        switch (std::stoi(mode)){
            case 1:
                gen.switch_bounds(300, 500);
                power_end = 17;
                filename += "_big";
                break;
            case 2:
                filename += "_smoll";
                break;
            default:
                std::cout << "Wrong mode!\n";
                return 1;
        }
    }
    std::cout << filename << std::endl;
    std::unordered_map<std::string, std::string> map;
    double sum=0;
    std::ofstream out("z_files/stats_" + filename + ".txt");
    for (int i=8; i<power_end; ++i){
        unsigned amount = std::pow(2, i), repeats=10;
        std::cout << amount << std::endl;
        std::vector<std::string> strs = generate_strings(amount, gen);
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