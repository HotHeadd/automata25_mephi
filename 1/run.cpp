#include <iostream>
#include <fstream>
#include "RegexResolver.hpp"
#include "CorrectGenerator.hpp"
#include "IncorrectGenerator.hpp"

using namespace resolvers;

void process_file(IResolver& resolver, std::vector<std::string> lines, const std::string& out_filename){
    std::string line;
    std::ofstream out(out_filename);
    if (!out.is_open()){
        throw std::runtime_error("Wrong output filename format");
    }
    unsigned count=0, correct=0;
    for (auto line: lines){
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

std::vector<std::string> generate_strings(const std::string& filename, unsigned& begin_line){
    std::random_device device;
    std::mt19937 rng;
    static std::uniform_int_distribution<unsigned> coinflip(0, 1);
    CorrectGenerator corr_gen(1, 5);
    IncorrectGenerator inc_gen(1, 5);
    std::string line;
    std::fstream file(filename, std::ios::in);
    if (!file.is_open()){
        throw std::runtime_error("Wrong input filename format");
    }
    std::vector<std::string> lines;
    std::getline(file, line);
    unsigned line_amount = std::stoi(line);
    for (int i=0; i<line_amount; ++i){
        --line_amount;
        std::getline(file, line);
        lines.push_back(line);
    }
    file.clear();  // Сбрасываем флаги потока (EOF)
    unsigned generated = 100;
    while (generated > 0){
        if (coinflip(rng)){
            line = inc_gen.gen_string();
        }
        else{
            line = corr_gen.gen_string();
        }
        --generated;
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
    std::string input_file = "z_files/in.txt";
    std::string output_file = "z_files/out.txt";
    unsigned begin_line;
    std::vector<std::string> lines = generate_strings(input_file, begin_line);
    process_file(*resolver, lines, output_file);
    delete resolver;
    return 0;
}