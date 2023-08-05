#pragma once
#include "code_gen.hpp"

class variable_gen : public code_generator {
	public:
		variable_gen(GEN_CONSTRUCTOR_PARAMS) : code_generator(ast_vec_p, file_name_p) {

		};

		int generate(AST::variable var);
};
