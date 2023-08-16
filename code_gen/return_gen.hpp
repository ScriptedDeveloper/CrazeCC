#pragma once
#include "code_gen.hpp"
#include "../parser/ast.hpp"
#include "function_call_gen.hpp"

class return_gen : public code_generator {
	public:
		return_gen(GEN_CONSTRUCTOR_PARAMS) : code_generator(ast_vec_p, file_name_p) {};
		virtual ~return_gen() {};

		int generate(AST::return_ast &ret_obj);
	private:
		std::string asm_instruction{};
};
