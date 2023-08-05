#include "function_gen.hpp"
	
int function_gen::generate(AST::function &func) {
	std::string asm_instruction{std::string(func.get_name().data()) + ":\n"};
	asm_instruction += "push rbp\nmov rbp, rsp\n";

	if(get_byte_size(func) != 0)
		asm_instruction += "sub rsp, " + std::to_string(get_byte_size(func)) + "\n";

	for(auto &i : func.function_body) {
		init(i, true);
		asm_instruction += curr_instruction;
	}
	
	is_function_body = false;
	append_asm(asm_instruction);
	return code_generator::GENERATE_SUCCESS;

}


