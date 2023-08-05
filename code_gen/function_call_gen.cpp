#include "function_call_gen.hpp"
#include "code_gen.hpp"
	

int function_call_gen::generate(AST::function_call) {
	std::string asm_instruction{};
		

	curr_instruction = asm_content;
	return code_generator::GENERATE_SUCCESS;
}

