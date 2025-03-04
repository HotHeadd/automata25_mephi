#include <string>
#include <random>

class Generator{
public:
    Generator(unsigned min, unsigned max) : amount_min(min), amount_max(max), 
                                            rng(device()), coinflip(0, 1) {}
    std::string gen_correct_string();
    std::string gen_incorrect_string();
private:
    unsigned amount_min, amount_max;
    std::string gen_name();
    std::string gen_amount(unsigned& amount);
    std::string gen_literal();

    std::random_device device;
    std::mt19937 rng;
    std::uniform_int_distribution<unsigned> coinflip;
};
