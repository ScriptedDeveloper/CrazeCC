#pragma once
#include "code_gen.hpp"

class function_call_gen : public code_generator {
	public:
		function_call_gen(GEN_CONSTRUCTOR_PARAMS) : code_generator(ast_vec_p, file_name_p) {

		};
		
		int generate(AST::function_call);
};
