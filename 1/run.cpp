#include <iostream>
#include <fstream>
#include "RegexResolver.hpp"
#include "SMCResolver.hpp"
#include "Generator.hpp"
#include "FlexResolver.hpp"
#include <thread>

void process_file(IResolver& resolver, std::vector<std::string> lines, const std::string& out_filename){
    std::string line;
    std::ofstream out(out_filename);
    if (!out.is_open()){
        throw std::runtime_error("Wrong output filename format");
    }
    unsigned count=0, correct=0;
    std::unordered_map<std::string, std::string> tokens;
    for (auto line: lines){
        ++count;
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

std::vector<std::string> generate_strings(const std::string& filename){
    std::random_device device;
    std::mt19937 rng(device());
    static std::uniform_int_distribution<unsigned> coinflip(0, 1);
    Generator gen(1, 5);
    std::string line;
    std::fstream file(filename, std::ios::in);
    if (!file.is_open()){
        throw std::runtime_error("Wrong input filename format");
    }
    std::vector<std::string> lines;
    std::getline(file, line);
    unsigned line_amount = std::stoi(line);
    for (int i=0; i<line_amount; ++i){
        std::getline(file, line);
        lines.push_back(line);
    }
    file.clear();  // Сбрасываем флаги потока (EOF)
    unsigned right=100, wrong=170;
    while (right>0){
        if (coinflip(rng) and wrong > 0){
            --wrong;
            line = gen.gen_inc_string();
        }
        else{
            --right;
            line = gen.gen_corr_string();
        }
        lines.push_back(line);
    }
    file.close();
    file.open(filename, std::ios::out);
    file << line_amount << "\n";
    for (auto str: lines){
        file << str << "\n";
    }
    return lines;
}

int main(int argc, char* argv[]){
    IResolver* resolver;
    if (argc > 1){
        std::string mode = argv[1];
        switch (std::stoi(mode)){
            case 1:
                resolver = new RegexResolver();
                break;
            case 2:
                resolver = new SMCResolver();
                break;
            case 3:
                resolver = new FlexResolver();
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
    std::string input_file = "z_files/in.txt";
    std::string output_file = "z_files/out.txt";
    std::vector<std::string> lines = generate_strings(input_file);
    process_file(*resolver, lines, output_file);
    std::unordered_map<std::string, std::string> tokens;
    Generator gen(1, 5);
    bool allright = true;
    for (int i=0; i<10000; i++){
        std::string str = gen.gen_corr_string();
        if (!resolver->is_suitable(str, tokens)){
            std::cout << "CORRECT\n";
            std::cout << str << std::endl;
            allright = false;
        }
    }
    for (int i=0; i<17000; i++){
        std::string str = gen.gen_inc_string();
        if (resolver->is_suitable(str, tokens)){
            std::cout << "INCORRECT\n";
            std::cout << str << "\n";
            allright = false;
        }
    }
    if (allright){
        std::cout << "ALL GOOD\n";
    }
    else{
        std::cout << "NOT ALL GOOD\n";
    }
    Generator gen_big(3000, 6000);
    for (int i=0; i<100; i++){
        std::string str = gen_big.gen_corr_string();
        if (!resolver->is_suitable(str, tokens)){
            std::cout << "CORRECT\n";
            std::cout << str << std::endl;
            allright = false;
        }
    }
    for (int i=0; i<100; i++){
        std::string str = gen_big.gen_inc_string();
        if (resolver->is_suitable(str, tokens)){
            std::cout << "INCORRECT\n";
            std::cout << str << "\n";
            allright = false;
        }
    }
    delete resolver;
    return 0;
}