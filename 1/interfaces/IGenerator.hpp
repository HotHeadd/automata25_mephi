#pragma once

#include <string>

class IGenerator{
public:
    virtual std::string gen_string() = 0;
    virtual std::string gen_name() = 0;
    virtual std::string gen_amount(unsigned& amount) = 0;
    virtual std::string gen_literal() = 0;
    virtual std::string gen_arr(unsigned amount, const std::string& str_amount) = 0;
};