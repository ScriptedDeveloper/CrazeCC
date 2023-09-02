#pragma once
#include "code_gen.hpp"
#include "function_call_gen.hpp"

class if_gen : public code_generator {
	public:
		if_gen(GEN_CONSTRUCTOR_PARAMS) : code_generator(ast_vec_p, file_name_p) {};
		virtual ~if_gen() {};

		int generate(AST::if_statement if_obj);

	private:
		std::string asm_instruction{};

		static int if_count; // for the labels

};
