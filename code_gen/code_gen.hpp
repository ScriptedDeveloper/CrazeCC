#pragma once
#include <vector>
#include "../parser/ast.hpp"
#include <fstream>

class code_generator {
	public:
		code_generator(std::vector<AST::AnyAST> &ast_vec_p, std::string_view file_name_p) {
			ast_vec = ast_vec_p;
			file_name = file_name_p;
		}
		virtual ~code_generator() {};
		int init() {
			for(auto curr_tree : ast_vec) {
				if(std::holds_alternative<AST::variable>(curr_tree)) {
					auto var = std::get<AST::variable>(curr_tree);
					generate_variable(var);
				} else if(std::holds_alternative<AST::if_statement>(curr_tree)) {
					auto statement = std::get<AST::if_statement>(curr_tree);
					generate_if_statement(statement);
				}
			}
			/*
			 * Ok, we have generated our asm code. Time to put inside our target file
			 */

			std::fstream o_file(file_name.data());
			o_file << asm_content;
			return GENERATE_SUCCESS;
		}
		static constexpr int GENERATE_SUCCESS = 0;
		static constexpr int GENERATE_UNKNOWN_VARIABLE = -10;
		static constexpr int GENERATE_UNKNOWN_VARIABLE_VALUE = -11;
	private:
		std::vector<AST::AnyAST> ast_vec{};
		std::string_view file_name{};
		std::string asm_content{};
		int rbp_count{}; // for the stack

		int generate_variable(AST::variable &var) {
			auto type = var.get_type();
			auto unconverted_value = var.get_value();
			std::string val{};
			if(unconverted_value.size() == 1)
				val = std::move(*unconverted_value.begin());
			else {
				if(unconverted_value == "true")
					val = "1";
				else if(unconverted_value == "false")
					val = "0";
				else
					return GENERATE_UNKNOWN_VARIABLE_VALUE;
			}
			std::string asm_type{};
			int stack_to_reserve{}; // amount of bytes we have to reserve for the variable
			if(type == "int") {
				asm_type = "BYTE PTR";
				stack_to_reserve = 4;
			} else if(type == "char" || type == "bool") {
				asm_type = "BYTE PTR";
				stack_to_reserve = 1;
			} else {
				return GENERATE_UNKNOWN_VARIABLE;
			}
			/*
			 * generates asm using simple string  manipulation (totally not copied from gcc)
			 */
			asm_type += std::string("[") + "rbp - " + std::to_string(rbp_count + stack_to_reserve) + "], " + val;
			asm_content += var.instruction_type + std::move(asm_type);
			/*
			 * adding it to the asm_content now!
			 */
			return GENERATE_SUCCESS;
		}
		
		int generate_if_statement(AST::if_statement &statement) {
			return GENERATE_SUCCESS;
		}
};
