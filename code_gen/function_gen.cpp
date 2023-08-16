#include "function_gen.hpp"
	
int function_gen::generate(AST::function &func) {
	set_global_function(func);
	asm_instruction += std::string(func.get_name().data()) + ":\n";
	asm_instruction += "push rbp\nmov rbp, rsp\n";
	auto rsp_bytes = std::to_string(get_byte_size(func));

	if(get_byte_size(func) != 0)
		asm_instruction += "sub rsp, " + rsp_bytes + "\n";

	for(auto &i : func.function_body) {
		init(i, true);
		asm_instruction += curr_instruction;
		curr_instruction.clear();
	}
	/*
	 * We have to reset the base pointer, otherwise segfault
	 */
	restore_stack_ptr(rsp_bytes);

	asm_instruction += "pop rbp\nret\n\n";
	append_asm(asm_instruction);
	return code_generator::GENERATE_SUCCESS;

}

void function_gen::restore_stack_ptr(const auto &bytes) {
	asm_instruction += "add rsp, " + bytes + "\n";
}

void function_gen::set_global_function(AST::function &func) {
	asm_instruction += std::string("global ") + func.get_name().data() + "\n";
}
