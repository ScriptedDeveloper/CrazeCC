#include "if_gen.hpp"
#include "helper.hpp"

int if_gen::if_count{};

int if_gen::generate(AST::if_statement if_obj) {
	auto reg = function_call_gen::get_next_register();
	asm_instruction += Helper::move_to_register(reg, if_obj.compare_pair.first, true);
	auto reg2 = function_call_gen::get_next_register();
	asm_instruction += Helper::move_to_register(reg2, if_obj.compare_pair.second);
	reg->second = false;

	asm_instruction += Helper::compare(reg->first, reg2->first);
	if_count++;
	std::string curr_label = "L" + std::to_string(if_count);
	std::string second_label = "L" + std::to_string(++if_count);

	if(if_obj.__is_compare) {
		asm_instruction += "jz " + curr_label + "\n";
		asm_instruction += "jmp " + second_label + "\n";
	}
	asm_instruction += curr_label + ":" + "\n";

	for(auto i : if_obj.function_body) {
		init(i, true);	
		asm_instruction += curr_instruction;
		curr_instruction.clear();
	}
	if_count++;

	asm_instruction += second_label + ":" + "\n";
	curr_instruction += asm_instruction;
	return GENERATE_SUCCESS;
}
