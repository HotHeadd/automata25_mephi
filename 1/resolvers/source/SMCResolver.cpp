#include "SMCResolver.hpp"

bool is_letter(const char symbol) {
	return ('a' <= symbol <= 'z') or ('A' <= symbol <= 'Z');
}
bool is_digit(char symbol){
	return symbol >= '0' and symbol <= '9';
}

bool SMCResolver::not_too_big_name(){
	if (++name_length < 17){
		name += current_symbol;
		return true;
	}
	return false;
}

bool SMCResolver::not_too_big_amount(){
	if (++amount_length < 10){
		amount_str += current_symbol;
		return true;
	}
	return false;
}

void SMCResolver::load_amount(){
	amount_val = std::stoi(amount_str);
}

bool SMCResolver::count_amount(){
	if (++count_commas < amount_val){
		return true;
	}
	return false;
}

bool SMCResolver::is_suitable(const std::string& expression, std::unordered_map<std::string, std::string>& tokens){
	name = "";
	amount_str = "";
	name_length=0;
	amount_length=0;
	count_commas=0;
	fsm.enterStartState();
	for (char symbol : expression){
		current_symbol = symbol;
		if (is_letter(symbol)){
			fsm.letter();
		}
		else if (is_digit(symbol)){
			fsm.digit();
		}
		else if (symbol == '['){
			fsm.lsbracket();
		}
		else if (symbol == ']'){
			fsm.rsbracket();
		}
		else if (symbol == '{'){
			fsm.lfbracket();
		}
		else if (symbol == '}'){
			fsm.rfbracket();
		}
		else if (symbol == '-'){
			fsm.minus();
		}
		else if (symbol == ','){
			fsm.comma();
		}
		else if (symbol == '='){
			fsm.eq();
		}
		else{
			fsm.unknown();
		}
	}
	fsm.EOS();
	if (is_acceptable){
		tokens["name"] = name;
		tokens["amount"] = amount_str;
	}
	return is_acceptable;
}
