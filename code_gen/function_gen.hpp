#pragma once
#include "code_gen.hpp"
#include "helper.hpp"

class function_gen : public code_generator {
	public:
		function_gen(GEN_CONSTRUCTOR_PARAMS) : code_generator(ast_vec_p, file_name_p){
			is_function_body = true;
		};
		virtual ~function_gen() {
			is_function_body = false;
		}
		int generate(AST::function &func);

	private:
		std::string asm_instruction{};
		inline void set_global_function(AST::function &func);
};


