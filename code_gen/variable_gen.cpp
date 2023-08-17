#include "variable_gen.hpp"
#include "code_gen.hpp"
#include "function_call_gen.hpp"

int variable_gen::generate(AST::variable var) {
	auto type = var.get_type();
	std::string val{};
	if(std::holds_alternative<std::string>(var.get_value())) {
		auto unconverted_value = std::get<std::string>(var.get_value());
		if(var.get_type() != AST::variable::TYPE_BOOL)
			val = std::move(*unconverted_value.begin());
		else {
			if(unconverted_value == "true")
				val = "1";
			else if(unconverted_value == "false")
				val = "0";
			else
				return code_generator::GENERATE_UNKNOWN_VARIABLE_VALUE;
		}
	} else {
		auto obj = std::get<AST::function_call>(var.get_value());
		function_call_gen g(ast_vec, file_name);
		auto tmp = is_function_body;
		is_function_body = true;
		g.generate(obj);
		is_function_body = tmp;
		var.instruction += g.curr_instruction;
		val += "eax"; // we save return val in eax for now
	}
	std::string asm_type{};
	int stack_to_reserve{}; // amount of bytes we have to reserve for the variable
	if(type == AST::variable::TYPE_INT) {
		asm_type = "dword";
		stack_to_reserve = 4;
	} else if(type == AST::variable::TYPE_CHAR || type == AST::variable::TYPE_BOOL) {
		asm_type = "byte";
		stack_to_reserve = 1;
	} else {
		return GENERATE_UNKNOWN_VARIABLE;
	}
	/*
	 * generates asm using simple string  manipulation (totally not copied from gcc)
	 */
	auto loc = "rbp - " + std::to_string(rbp_count + stack_to_reserve);
	code_generator::var_mem_locations[var.get_name().data()] = loc;
	asm_type += std::string("[") + loc + "], " + val + "\n";
	var.instruction += var.instruction_type + " " + std::move(asm_type);
	if(!is_function_body)
		append_asm(var.instruction);
	rbp_count += stack_to_reserve;
	curr_instruction = var.instruction;
	return code_generator::GENERATE_SUCCESS;
}

