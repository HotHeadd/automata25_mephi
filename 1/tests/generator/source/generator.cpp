#include "generator.hpp"
#include <limits>

std::string Generator::generate_string(){
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
    static std::uniform_int_distribution<unsigned> coinflip(0, 1);
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
    static std::uniform_int_distribution<unsigned> dist_literal(0, std::numeric_limits<unsigned>::max());
    return std::to_string(dist_literal(rng));
}