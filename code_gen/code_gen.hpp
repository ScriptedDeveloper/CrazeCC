#pragma once
#define GEN_CONSTRUCTOR_PARAMS std::vector<std::shared_ptr<AST::AnyAST>> &ast_vec_p, std::string_view file_name_p
#include <vector>
#include "../parser/ast.hpp"
#include "../parser/generate_ast.hpp"
#include <ostream>
#include <unordered_map>

class variable_gen;
class function_gen;
class function_call_gen;
class return_gen;

class code_generator {
	public:
		code_generator(GEN_CONSTRUCTOR_PARAMS) {
			ast_vec = ast_vec_p;
			file_name = file_name_p;
		}
		virtual ~code_generator() {};

		int init(AST::AnyAST variable = AST::AnyAST(), const bool check_variable = false);

		static constexpr int GENERATE_SUCCESS = -30;
		static constexpr int GENERATE_UNKNOWN_VARIABLE = -20;
		static constexpr int GENERATE_UNKNOWN_VARIABLE_VALUE = -21;
		static std::string asm_content;
	private:
		std::vector<std::shared_ptr<AST::AnyAST>> ast_vec{};
		std::string_view file_name{};
		bool __is_debug{};


	protected:
		static bool is_function_body;
		static std::string curr_instruction; // for function body
		static int rbp_count; // for the stack

		int get_byte_size(const AST::function &func);

		inline void append_asm(std::string_view expr_asm) {
			asm_content += expr_asm.data();
		}
};

#include "function_gen.hpp"
#include "return_gen.hpp"
#include "variable_gen.hpp"
#include "function_call_gen.hpp"
