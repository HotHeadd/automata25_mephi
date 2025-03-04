#include "IncorrectGenerator.hpp"
#include <limits>
#include <iostream>

std::string IncorrectGenerator::gen_string(){
    static std::uniform_int_distribution<unsigned> dist_defect(0, 2);
    defect_type = 2; //dist_defect(rng);
    unsigned amount;
    std::string str_amount = gen_amount(amount);
    return gen_name() + str_amount + gen_arr(amount);
}

std::string IncorrectGenerator::gen_name(){
    static std::uniform_int_distribution<unsigned> dist_defect(0, 2), dist_letter(0, 25), dist_digit(0, 9), dist_char(0, 255);
    if (defect_type != 0){
        return corr_gen.gen_name();
    }
    std::string name;
    switch (dist_defect(rng)){
        case 0:
            name += '0' + dist_digit(rng);
            break;
        case 1:
            for (int i=0; i<16; ++i){
                name += 'a' + dist_letter(rng);
            }
            break;
        case 2:
            for (int i=0; i<8; ++i){
                name += dist_char(rng);
            }
            break;
    }
    for (int i=0; i<6; ++i){
        name += 'a' + dist_letter(rng);
    }
    return name;
}

std::string IncorrectGenerator::gen_amount(unsigned& amount){
    if (defect_type != 1){
        std::string res = corr_gen.gen_amount(amount);
        is_empty_amount = res == "[]" or res == "[0]";
        return res;
    }
    static std::uniform_int_distribution<unsigned> dist_amount(amount_min, amount_max), dist_defect(0, 1), dist_char(0, 25), dist_digit(0, 9);
    amount = dist_amount(rng);
    std::string res = std::to_string(amount);
    switch (dist_defect(rng)){
        case 0:
            res.insert(res.end(), 'a' + dist_char(rng));
            break;
        case 1:
            for (int i=0; i<9; ++i){
                res += '0' + dist_digit(rng);
            }
            break;
    }
    is_empty_amount=false;
    return "[" + res + "]";
}

std::string IncorrectGenerator::gen_arr(unsigned amount){
    if (defect_type != 2){
        return corr_gen.gen_arr(amount);
    }
    if (is_empty_amount){
        return "";
    }
    static std::uniform_int_distribution<unsigned> dist_defect(0, 1), dist_index(0, amount-1);
    std::string result="={";
    switch (dist_defect(rng)){
        case 0:
            result = corr_gen.gen_arr(amount+1);
            break;
        case 1:
            unsigned bad_ind=dist_index(rng);
            for (int i=0; i<amount; ++i){
                if (i!=0){
                    result += ",";
                }
                if (i == bad_ind){
                    result += gen_literal();
                }
                result += corr_gen.gen_literal();
            }
            result += "}";
            break;
    }
    return result;
}

std::string IncorrectGenerator::gen_literal(){
    static std::uniform_int_distribution<unsigned> dist_defect(0, 1);
    std::string res=corr_gen.gen_literal();
    switch (dist_defect(rng)){
        case 0:
            res.push_back('-');
            break;
        case 1:
            res.insert(res.begin(), 'a');
            break;
    }
    return res;
}