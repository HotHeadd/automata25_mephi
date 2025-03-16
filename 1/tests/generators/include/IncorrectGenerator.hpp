#pragma once

#include "IGenerator.hpp"
#include "CorrectGenerator.hpp"
#include <string>
#include <random>

class IncorrectGenerator : public IGenerator{
public:
    IncorrectGenerator(unsigned min, unsigned max) : amount_min(min), amount_max(max), 
                                            rng(device()), coinflip(0, 1), corr_gen(min, max) {}
    std::string gen_string() override;
    std::string gen_name() override;
    std::string gen_amount(unsigned& amount) override;
    std::string gen_literal() override;
    std::string gen_arr(unsigned amount, const std::string& str_amount) override;

private:
    unsigned amount_min, amount_max, defect_type;
    std::random_device device;
    std::mt19937 rng;
    std::uniform_int_distribution<unsigned> coinflip;

    CorrectGenerator corr_gen;
    bool is_empty_amount=false;
};
