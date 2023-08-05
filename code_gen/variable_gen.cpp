#include "variable_gen.hpp"

int variable_gen::generate(AST::variable var) {
	auto type = var.get_type();
	auto unconverted_value = var.get_value();
	std::string val{};
	if(var.get_type() != "bool")
		val = std::move(*unconverted_value.begin());
	else {
		if(unconverted_value == "true")
			val = "1";
		else if(unconverted_value == "false")
			val = "0";
		else
			return code_generator::GENERATE_UNKNOWN_VARIABLE_VALUE;
	}
	std::string asm_type{};
	int stack_to_reserve{}; // amount of bytes we have to reserve for the variable
	if(type == "int") {
		asm_type = "dword";
		stack_to_reserve = 4;
	} else if(type == "char" || type == "bool") {
		asm_type = "byte";
		stack_to_reserve = 1;
	} else {
		return GENERATE_UNKNOWN_VARIABLE;
	}
	/*
	 * generates asm using simple string  manipulation (totally not copied from gcc)
	 */
	var.memory_location = "rbp - " + std::to_string(rbp_count + stack_to_reserve);
	asm_type += std::string("[") + var.memory_location + "], " + val + "\n";
	var.instruction = var.instruction_type + " " + std::move(asm_type);
	if(!is_function_body)
		append_asm(var.instruction);
	rbp_count += stack_to_reserve;
	curr_instruction = var.instruction;
	return code_generator::GENERATE_SUCCESS;
}

