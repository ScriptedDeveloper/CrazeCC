#include "return_gen.hpp"

int return_gen::generate(AST::return_ast &ret_obj) {
	auto reg = function_call_gen::get_next_register();
	auto it = generate_ast::variable::var_map.find(ret_obj.get_value());
	std::string val{};
	
	if(it == generate_ast::variable::var_map.end()) { 
		val = ret_obj.get_value();
	} else {
		val += " [";
		val += code_generator::var_mem_locations[ret_obj.get_value()];
		val += "]";
	}
		
	asm_instruction += "mov " + reg->first + ", " + val + "\n";

	curr_instruction = asm_instruction;
	return GENERATE_SUCCESS;
}
