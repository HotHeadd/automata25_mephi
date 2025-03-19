#include "CorrectGenerator.hpp"
#include <limits>

std::string CorrectGenerator::gen_string(){
    unsigned amount;
    std::string str_amount = gen_amount(amount);
    return gen_name() + str_amount + gen_arr(amount, str_amount);
}

std::string CorrectGenerator::gen_arr(unsigned amount, const std::string& str_amount){
    static std::uniform_int_distribution<unsigned> dist_empty(0, 9);
    if (!(str_amount == "[0]" or str_amount == "[]") and dist_empty(rng) == 0){
        return "={}";
    }
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
    name += gen_letter();
    for (int i=0; i<amount; ++i){
        name += gen_symbol();
    }
    return name;
}

std::string CorrectGenerator::gen_amount(unsigned& amount){
    static std::uniform_int_distribution<unsigned> dist_amount(amount_min, std::min(amount_max, 999999999u)), dist_empty(0, 6);
    amount = dist_amount(rng);
    if (dist_empty(rng) == 0){
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

char CorrectGenerator::gen_digit(){
    static std::uniform_int_distribution<unsigned> dist_digit(0, 9);
    return '0' + dist_digit(rng);
}

char CorrectGenerator::gen_letter(){
    static std::uniform_int_distribution<unsigned> dist_letter(0, 25), coinflip(0, 1);
    if (coinflip(rng)){
        return 'A' + dist_letter(rng);
    }
    return 'a' + dist_letter(rng);
}

char CorrectGenerator::gen_symbol(){
    static std::uniform_int_distribution<unsigned> dist_letter(0, 25), coinflip(0, 1);
    if (coinflip(rng)){
        return gen_digit();
    }
    return gen_letter();
}