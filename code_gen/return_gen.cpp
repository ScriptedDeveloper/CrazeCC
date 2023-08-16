#include "return_gen.hpp"

int return_gen::generate(AST::return_ast &ret_obj) {
	auto reg = function_call_gen::get_next_register();
	asm_instruction += "mov " + reg->first + ", " + ret_obj.get_value() + "\n";

	curr_instruction = asm_instruction;
	return GENERATE_SUCCESS;
}
