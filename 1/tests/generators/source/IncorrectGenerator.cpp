#include "IncorrectGenerator.hpp"
#include <limits>
#include <iostream>

std::string IncorrectGenerator::gen_string(){
    static std::uniform_int_distribution<unsigned> dist_defect(0, 2);
    defect_type = dist_defect(rng);
    unsigned amount;
    std::string str_amount = gen_amount(amount);
    return gen_name() + str_amount + gen_arr(amount, str_amount);
}

std::string IncorrectGenerator::gen_name(){
    static std::uniform_int_distribution<unsigned> dist_defect(0, 2), dist_char(0, 255), dist_additional(2 ,8);
    if (defect_type != 0){
        return corr_gen.gen_name();
    }
    std::string name;
    switch (dist_defect(rng)){
        case 0:
            name += gen_digit();
            break;
        case 1:
            for (int i=0; i<16; ++i){
                name += gen_symbol();
            }
            break;
        case 2:
            name += gen_forbidden();
            break;
    }
    unsigned add_amount = dist_additional(rng);
    for (int i=0; i<add_amount; ++i){
        name += gen_symbol();
    }
    return name;
}

std::string IncorrectGenerator::gen_amount(unsigned& amount){
    if (defect_type != 1){
        return corr_gen.gen_amount(amount);
    }
    static std::uniform_int_distribution<unsigned> dist_amount(amount_min, amount_max), dist_defect(0, 2);
    amount = dist_amount(rng);
    std::string res = std::to_string(amount);
    switch (dist_defect(rng)){
        case 0:
            res.insert(res.end(), gen_letter());
            break;
        case 1:
            for (int i=0; i<9; ++i){
                res += gen_digit();
            }
            break;
        case 2:
            switch (dist_defect(rng)){
                case 0:
                    return res + "]";
                case 1:
                    return "[" + res;
                case 2:
                    return res;
            }
            break;
    }
    return "[" + res + "]";
}

std::string IncorrectGenerator::gen_arr(unsigned amount, const std::string& str_amount){
    static std::uniform_int_distribution<unsigned> dist_defect(0, 3), dist_index(0, amount-1);
    if (defect_type != 2){
        return corr_gen.gen_arr(amount, str_amount);
    }
    unsigned arr_defect = dist_defect(rng);
    if (str_amount == "[]" or str_amount == "[0]"){
        switch (dist_defect(rng)){
            case 0:
                return "";
            case 1:
                return "=}";
            case 2:
                return "={}";
            case 3:
                arr_defect = 1;
        }
    }
    std::string result="={";
    switch (arr_defect){
        case 0:
            result = corr_gen.gen_arr(amount+1, str_amount);
            break;
        case 1:
        case 2:
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
        case 3:
            result = corr_gen.gen_arr(amount+1, str_amount);
            switch (dist_defect(rng)){
                case 0:
                    result[0] = gen_symbol();
                    break;
                case 1:
                    result[1] = gen_symbol();
                    break;
                case 2:
                    result.back() = gen_symbol();
                case 3:
                    result.push_back(gen_symbol());
                    break;
            }
    }
    return result;
}

std::string IncorrectGenerator::gen_literal(){
    static std::uniform_int_distribution<unsigned> dist_defect(0, 1);
    std::string res=corr_gen.gen_literal();
    switch (dist_defect(rng)){
        case 0:
            res.push_back(gen_forbidden());
            break;
        case 1:
            res.insert(res.begin(), gen_letter());
            break;
    }
    return res;
}

char IncorrectGenerator::gen_digit(){
    static std::uniform_int_distribution<unsigned> dist_digit(0, 9);
    return '0' + dist_digit(rng);
}

char IncorrectGenerator::gen_forbidden(){
    static std::string forbidden = "_+=;:'][`~!@#$%^&*()";
    static std::uniform_int_distribution<unsigned> dist_forbidden(0, forbidden.size()-1);
    return forbidden[dist_forbidden(rng)];
}

char IncorrectGenerator::gen_letter(){
    static std::uniform_int_distribution<unsigned> dist_digit(0, 9);
    return '0' + dist_digit(rng);
}

char IncorrectGenerator::gen_symbol(){
    static std::uniform_int_distribution<unsigned> dist_letter(0, 25), coinflip(0, 1);
    if (coinflip(rng)){
        return gen_digit();
    }
    return gen_letter();
}
