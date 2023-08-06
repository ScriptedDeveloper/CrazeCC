#pragma once
#include "code_gen.hpp"

class function_call_gen : public code_generator {
	public:
		function_call_gen(GEN_CONSTRUCTOR_PARAMS) : code_generator(ast_vec_p, file_name_p) {

		};
		
		int generate(AST::function_call &call);
	private:
		std::array<std::pair<const std::string, bool>, 8> register_order = {std::make_pair("rax", false), {"rbx", false}, 
			{"rcx", false}, {"rdx", false}, {"rsi", false}, {"rsi", false},  {"rdi", false}, {"rbp", false}};

		inline std::array<std::pair<const std::string, bool>, 8>::iterator get_next_register() {
			return std::find_if(register_order.begin(), register_order.end(), [](auto &pair) {
				return !pair.second;
					});
		}
};
