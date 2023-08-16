#include "function_call_gen.hpp"
#include "code_gen.hpp"
	

std::array<std::pair<const std::string, bool>, 8> function_call_gen::register_order = 
		{std::make_pair("rax", false), 
		{"rbx", false}, {"rcx", false}, {"rdx", false}, 
		{"rsi", false}, {"rsi", false},  {"rdi", false}, {"rbp", false}};


int function_call_gen::generate(AST::function_call &call) {
	std::string asm_instruction{};
	
	for(auto &i : call.params) {
		auto curr_register = get_next_register();
		AST::variable var = std::get<AST::variable>(*i);
		auto val = std::get<std::string>(var.get_value());
		asm_instruction += "mov " + curr_register->first + ", ";
		switch(var.get_type()) {	
			case AST::variable::TYPE_CHAR : {
				asm_instruction += std::to_string(val[0] - '0');
				break;
			};	
			case AST::variable::TYPE_BOOL : {
				if(val == "true")
					asm_instruction += "1";
				else
					asm_instruction += "0";
				break;
			};
			default: {
				asm_instruction += val;
				break;
			}
		}
		asm_instruction += "\n";
	}
	asm_instruction += std::string("call ") + std::get<AST::function>(*call.get_function()).get_name().data();
	asm_instruction += "\n";
	curr_instruction = asm_instruction;
	if(!is_function_body)
		append_asm(asm_instruction);
	return code_generator::GENERATE_SUCCESS;
}

