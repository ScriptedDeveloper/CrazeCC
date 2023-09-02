#include "helper.hpp"
#include "function_gen.hpp"
	
int function_gen::generate(AST::function &func) {
	set_global_function(func);
	asm_instruction += Helper::stack_setup(func.get_name());
	auto rsp_bytes = std::to_string(get_byte_size(func));

	if(get_byte_size(func) != 0)
		asm_instruction += Helper::subtract_bytes(rsp_bytes);

	for(auto &i : func.function_body) {
		init(i, true, func);
		asm_instruction += curr_instruction;
		curr_instruction.clear();
	}
	/*
	 * We have to reset the base pointer, otherwise segfault
	 */

	asm_instruction += "\n";
	append_asm(asm_instruction);
	return code_generator::GENERATE_SUCCESS;

}



void function_gen::set_global_function(AST::function &func) {
	asm_instruction += std::string("global ") + func.get_name().data() + "\n";
}
