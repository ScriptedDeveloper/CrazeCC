#pragma once
#include "code_gen.hpp"
#include "function_call_gen.hpp"

class variable_gen : public code_generator {
	public:
		variable_gen(GEN_CONSTRUCTOR_PARAMS) : code_generator(ast_vec_p, file_name_p) {
			file_name = file_name_p;
			ast_vec = ast_vec_p;
		};

		int generate(AST::variable var);
		std::vector<std::shared_ptr<AST::AnyAST>> ast_vec{};
		std::string file_name{};
};
