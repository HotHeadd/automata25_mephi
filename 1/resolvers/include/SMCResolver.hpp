#pragma once

#include "IResolver.hpp"
#include "SMCResolver_sm.h"
#include <string>

class SMCResolver : public IResolver{
public:
	SMCResolver() : fsm(*this) {}
	void Acceptable();
	void Unacceptable();
	bool not_too_big_name();
	bool not_too_big_amount();
	void load_amount();
	bool count_amount();
	bool check_bracket();

    bool is_suitable(const std::string& expression, std::unordered_map<std::string, std::string>& token_vector) override;
private:
	SMCResolverContext fsm;
	bool is_acceptable = false;

	unsigned amount_val;
	std::string amount_str;
	std::string name;

	unsigned name_length;
	unsigned amount_length;
	unsigned count_commas;
	bool from_comma;

	char current_symbol;
};
