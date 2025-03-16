#include "CorrectGenerator.hpp"
#include <limits>
#include <iostream>

std::string CorrectGenerator::gen_string(){
    unsigned amount;
    std::string str_amount = gen_amount(amount);
    return gen_name() + str_amount + gen_arr(amount, str_amount);
}


std::string CorrectGenerator::gen_arr(unsigned amount, const std::string& str_amount){
    std::string result = "={";
    for (int i=0; i<amount; ++i){
        if (i!=0){
            result += ",";
        }
        result += gen_literal();
    }
    result += "}";
    return result;
}

std::string CorrectGenerator::gen_name(){
    static std::uniform_int_distribution<unsigned> dist_amount(0, 15);
    static std::uniform_int_distribution<char> dist_letter(0, 25);
    std::string name;
    unsigned amount = dist_amount(rng);
    name += 'a'+dist_letter(rng);
    for (int i=0; i<amount; ++i){
        name += 'a'+dist_letter(rng);
    }
    return name;
}

std::string CorrectGenerator::gen_amount(unsigned& amount){
    static std::uniform_int_distribution<unsigned> dist_amount(amount_min, std::min(amount_max, 999999999u)), dist_empty(0, 3);
    amount = dist_amount(rng);
    if (!dist_empty(rng)){
        if (dist_empty(rng)%2 == 0){
            return "[]";
        }
        return "[0]";
    }
    else{
        return "[" + std::to_string(amount) + "]";
    }

}

std::string CorrectGenerator::gen_literal(){
    static std::uniform_int_distribution<int> dist_literal(-999999999, 999999999);
    return std::to_string(dist_literal(rng));
}