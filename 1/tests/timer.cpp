#include <iostream>
#include "CorrectGenerator.hpp"
#include "IncorrectGenerator.hpp"
#include "RegexResolver.hpp"

using namespace resolvers;

int main(){ // TODO: выбор ресолвера через аргумент
    IResolver* resolver = new RegexResolver();
    delete resolver;
    return 0;
}