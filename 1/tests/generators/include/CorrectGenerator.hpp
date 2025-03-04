#pragma once

#include "IGenerator.hpp"
#include <string>
#include <random>

class CorrectGenerator : public IGenerator{
public:
    CorrectGenerator(unsigned min, unsigned max) : amount_min(min), amount_max(max), 
                                            rng(device()){}
    std::string gen_string() override;
    std::string gen_name() override;
    std::string gen_amount(unsigned& amount) override;
    std::string gen_literal() override;
    std::string gen_arr(unsigned amount) override;
private:
    unsigned amount_min, amount_max;
    std::random_device device;
    std::mt19937 rng;
};
