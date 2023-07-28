#pragma once
#include <vector>
#include "../parser/ast.hpp"
#include "../parser/generate_ast.hpp"
#include <ostream>
#include <unordered_map>

class code_generator {
	public:
		code_generator(std::vector<std::shared_ptr<AST::AnyAST>> &ast_vec_p, std::string_view file_name_p) {
			ast_vec = ast_vec_p;
			file_name = file_name_p;
		}
		virtual ~code_generator() {};
		int init() {
			for(auto curr_tree : ast_vec) {
				if(std::holds_alternative<AST::variable>(*curr_tree)) {
					auto var = std::get<AST::variable>(*curr_tree);
					generate_variable(var);
				} else if(std::holds_alternative<AST::if_statement>(*curr_tree)) {
					/*
					auto statement = std::get<AST::if_statement>(curr_tree);
					generate_if_statement(statement);
					*/
				} else if(std::holds_alternative<AST::function>(*curr_tree)) {
					generate_function(std::get<AST::function>(*curr_tree));
				}
			}
			/*
			 * Ok, we have generated our asm code. Time to put inside our target file
			 */

			std::ofstream o_file(file_name.data());
			o_file << asm_content;
			return GENERATE_SUCCESS;
		}
		static constexpr int GENERATE_SUCCESS = 0;
		static constexpr int GENERATE_UNKNOWN_VARIABLE = -10;
		static constexpr int GENERATE_UNKNOWN_VARIABLE_VALUE = -11;
	private:
		std::vector<std::shared_ptr<AST::AnyAST>> ast_vec{};
		std::string_view file_name{};
		std::string asm_content{"BITS 64\n"};
		int rbp_count{}; // for the stack


		
		inline void append_asm(std::string_view expr_asm) {
			asm_content += expr_asm.data();
		}

		int generate_variable(AST::variable &var) {
			auto type = var.get_type();
			auto unconverted_value = var.get_value();
			std::string val{};
			if(var.get_type() != "bool")
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
				asm_type = "dword";
				stack_to_reserve = 4;
			} else if(type == "char" || type == "bool") {
				asm_type = "byte";
				stack_to_reserve = 1;
			} else {
				return GENERATE_UNKNOWN_VARIABLE;
			}
			/*
			 * generates asm using simple string  manipulation (totally not copied from gcc)
			 */
			var.memory_location = "rbp - " + std::to_string(rbp_count + stack_to_reserve);
			asm_type += std::string("[") + var.memory_location + "], " + val + "\n";
			var.instruction = var.instruction_type + " " + std::move(asm_type);
			append_asm(var.instruction);
			rbp_count += stack_to_reserve;
			/*
			 * Last but not least, we have to save the variable to the map
			 * in case we pass it via function params
			auto var_vec = generate_ast::function::param_map[var.get_name()];
			for(auto &i : var_vec) {
				auto func = std::get<AST::function>(*i);
				func.params.push_back(std::make_pair(std::string(var.get_name().data()), AST::AnyAST(var)));
			}
			*/
			/*
			 * adding it to the asm_content now!
			 */
			return GENERATE_SUCCESS;
		}
		/*
		int generate_if_statement(AST::if_statement &statement) {
			return GENERATE_SUCCESS;
		}
		*/
		int generate_function(AST::function &func) {
			std::string asm_instruction{};
			/*
			for(auto &i : func.params) {
				asm_instruction += "push " + std::get<AST::variable>(i.second).memory_location + "\n";
			}
			*/
			asm_instruction += std::string(func.get_name().data()) + ":";
			append_asm(asm_instruction);
			return GENERATE_SUCCESS;
		}
};
