#include "generator.hpp"
#include <limits>
#include <iostream>

std::string Generator::gen_correct_string(){
    unsigned amount;
    std::string result = gen_name() + "[" + gen_amount(amount) + "]={";
    for (int i=0; i<amount; ++i){
        if (i!=0){
            result += ",";
        }
        result += gen_literal();
    }
    result += "}";
    return result;
}

std::string Generator::gen_incorrect_string(){
    static std::uniform_int_distribution<unsigned> dist_difference(0, 5);
    unsigned amount;
    std::string name=gen_name(), str_amount=gen_amount(amount);
    switch (dist_difference(rng)){
        default:
            std::cout << "Nothing ever happens\n";
        case 0: // wrong name
            break;
        case 1: // wrong amount
            break;
        case 2: // wrong literal
            break;
        case 3: // wrong literal amount
            break;
        case 4: // empty list
            break;
        case 5: // no list no amount
            break;
    }
    return "";
}

std::string Generator::gen_name(){
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

std::string Generator::gen_amount(unsigned& amount){
    static std::uniform_int_distribution<unsigned> dist_amount(amount_min, amount_max);
    amount = dist_amount(rng);
    if (coinflip(rng)){
        if (coinflip(rng)){
            return "";
        }
        return "0";
    }
    else{
        return std::to_string(amount);
    }

}

std::string Generator::gen_literal(){
    static std::uniform_int_distribution<int> dist_literal(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    return std::to_string(dist_literal(rng));
}