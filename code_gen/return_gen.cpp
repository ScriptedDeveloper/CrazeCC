#include "return_gen.hpp"

int return_gen::generate(AST::return_ast &ret_obj, AST::function parent) {
	std::string reg = "eax";
	auto it = generate_ast::variable::var_map.find(ret_obj.get_value());
	std::string val{};
	
	if(it == generate_ast::variable::var_map.end()) { 
		val = ret_obj.get_value();
	} else {
		val += " [";
		val += code_generator::var_mem_locations[ret_obj.get_value()];
		val += "]";
	}
		
	asm_instruction += "mov " + reg + ", " + val + "\n";
	asm_instruction += "pop rbp\n";
	asm_instruction += Helper::restore_stack_ptr(std::to_string(get_byte_size(parent)));
	asm_instruction += "ret\n";

	curr_instruction = asm_instruction;
	return GENERATE_SUCCESS;
}
