#include "Generator.hpp"
#include <limits>
#include <assert.h>

std::string Generator::gen_corr_string(){
    unsigned amount;
    std::string str_amount = gen_corr_amount(amount);
    return gen_corr_name() + str_amount + gen_corr_arr(amount, str_amount);
}

std::string Generator::gen_corr_arr(unsigned amount, const std::string& str_amount){
    static std::uniform_int_distribution<unsigned> dist_empty(0, 9);
    if (!(str_amount == "[0]" or str_amount == "[]") and dist_empty(rng) == 0){
        return "={}";
    }
    std::string result = "={";
    for (int i=0; i<amount; ++i){
        if (i!=0){
            result += ",";
        }
        result += gen_corr_literal();
    }
    result += "}";
    return result;
}

std::string Generator::gen_corr_name(){
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

std::string Generator::gen_corr_amount(unsigned& amount){
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

std::string Generator::gen_corr_literal(){
    static std::uniform_int_distribution<int> dist_literal(-999999999, 999999999);
    return std::to_string(dist_literal(rng));
}

std::string Generator::gen_inc_string(){
    static std::uniform_int_distribution<unsigned> dist_defect(0, 2);
    defect_type = dist_defect(rng);
    unsigned amount;
    std::string str_amount = gen_inc_amount(amount);
    return gen_inc_name() + str_amount + gen_inc_arr(amount, str_amount);
}

std::string Generator::gen_inc_name(){
    static std::uniform_int_distribution<unsigned> dist_defect(0, 2), dist_char(0, 255), dist_additional(2 ,8);
    if (defect_type != 0){
        return gen_corr_name();
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

std::string Generator::gen_inc_amount(unsigned& amount){
    if (defect_type != 1){
        return gen_corr_amount(amount);
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

std::string Generator::gen_inc_arr(unsigned amount, const std::string& str_amount){
    std::uniform_int_distribution<unsigned> dist_defect(0, 3), dist_index(0, amount-1);
    if (defect_type != 2){
        return gen_corr_arr(amount, str_amount);
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
    unsigned bad_ind=dist_index(rng);
    std::string result="={";
    switch (arr_defect){
        case 0:
            result = gen_corr_arr(amount+1, str_amount);
            while (result == "={}"){
                result = gen_corr_arr(amount+1, str_amount);
            }
            break;
        case 1:
        case 2:
            if (bad_ind == 0 or amount == 1){
                arr_defect = 2; // если индекс 0, то меняем дефект на неправильный литерал
            }
            for (int i=0; i<amount; ++i){
                if (i!=0){
                    if (arr_defect != 1 or i != bad_ind){ // пропускает запятую если дефект 1 и нужный индекс
                        result += ",";
                    }
                    else{
                        result += " ";
                    }
                }
                if (arr_defect == 2 and i == bad_ind){
                    result += gen_inc_literal();
                }
                result += gen_corr_literal();
            }
            result += "}";
            break;
        case 3:
            result = gen_corr_arr(amount, str_amount);
            switch (dist_defect(rng)){
                case 0:
                    result[0] = gen_symbol();
                    break;
                case 1:
                    result[1] = gen_symbol();
                    break;
                case 2:
                    result.back() = gen_symbol();
                    break;
                case 3:
                    result.push_back(gen_symbol());
                    break;
            }
            break;
    }
    return result;
}

std::string Generator::gen_inc_literal(){
    static std::uniform_int_distribution<unsigned> dist_defect(0, 30);
    std::string res=gen_corr_literal();
    assert(res.size() != 0);
    unsigned roll=dist_defect(rng);
    if (roll == 0){
        res.push_back(gen_forbidden());
    }
    else if (roll < 10){
        res.push_back(gen_forbidden());
    }
    else if (roll < 20){
        res.insert(res.end(), gen_letter());
    }
    else{
        res[res.size()/2] = gen_letter();
    }
    return res;
}

char Generator::gen_digit(){
    static std::uniform_int_distribution<unsigned> dist_digit(0, 9);
    return '0' + dist_digit(rng);
}

char Generator::gen_forbidden(){
    static std::string forbidden = "_+=;:'][`~!@#$%^&*()";
    static std::uniform_int_distribution<unsigned> dist_forbidden(0, forbidden.size()-1);
    return forbidden[dist_forbidden(rng)];
}

char Generator::gen_letter(){
    static std::uniform_int_distribution<unsigned> dist_letter(0, 25);
    if (coinflip(rng)){
        return 'A' + dist_letter(rng);
    }
    return 'a' + dist_letter(rng);
}

char Generator::gen_symbol(){
    static std::uniform_int_distribution<unsigned> dist_letter(0, 25), coinflip(0, 1);
    if (coinflip(rng)){
        return gen_digit();
    }
    return gen_letter();
}

void Generator::switch_bounds(unsigned lower, unsigned upper){
    if (lower > upper){
        std::swap(lower, upper);
    }
    amount_min = lower;
    amount_max = upper;
}