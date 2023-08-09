#pragma once
#include "code_gen.hpp"

class function_gen : public code_generator {
	public:
		function_gen(GEN_CONSTRUCTOR_PARAMS) : code_generator(ast_vec_p, file_name_p){
			
		};
		int generate(AST::function &func);

	private:
		 std::string asm_instruction{};
		inline void restore_stack_ptr(const auto &bytes);
		inline void set_global_function(AST::function &func);
};


