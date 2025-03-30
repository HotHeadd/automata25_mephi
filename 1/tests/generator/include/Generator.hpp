#pragma once

#include <string>
#include <random>

class Generator{
public:
    Generator(unsigned min, unsigned max) : amount_min(min), amount_max(max), 
    rng(device()), coinflip(0, 1) {}

    void switch_bounds(unsigned lower, unsigned upper);
    
    std::string gen_corr_string();
    std::string gen_inc_string();
private:
    std::string gen_corr_name();
    std::string gen_corr_amount(unsigned& amount);
    std::string gen_corr_literal();
    std::string gen_corr_arr(unsigned amount, const std::string& str_amount);

    std::string gen_inc_name();
    std::string gen_inc_amount(unsigned& amount);
    std::string gen_inc_literal();
    std::string gen_inc_arr(unsigned amount, const std::string& str_amount);

    unsigned amount_min, amount_max, defect_type;
    std::random_device device;
    std::mt19937 rng;
    std::uniform_int_distribution<unsigned> coinflip;

    char gen_digit();
    char gen_symbol();
    char gen_letter();
    char gen_forbidden();
};
