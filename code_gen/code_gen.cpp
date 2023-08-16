#include "code_gen.hpp"
		

std::string code_generator::asm_content{"BITS 64\n"};
std::string code_generator::curr_instruction{};
int code_generator::rbp_count{};
bool code_generator::is_function_body{false};

int code_generator::init(AST::AnyAST variable, const bool check_variable) {
	is_function_body = check_variable;
	for(auto curr_tree_it : ast_vec) {
		auto curr_tree = *curr_tree_it;
		int ret{};
		if(check_variable) {
			curr_tree = variable;
		}
		if(std::holds_alternative<AST::variable>(curr_tree)) {
			variable_gen v(ast_vec, file_name);
			ret = v.generate(std::get<AST::variable>(curr_tree));
		} else if(std::holds_alternative<AST::if_statement>(curr_tree)) {
			/*
			auto statement = std::get<AST::if_statement>(curr_tree);
			generate_if_statement(statement);
			*/
		} else if(std::holds_alternative<AST::function>(curr_tree)) {
			function_gen f(ast_vec, file_name);
			ret = f.generate(std::get<AST::function>(curr_tree));
		} else if(std::holds_alternative<AST::function_call>(curr_tree)) {
			function_call_gen f(ast_vec, file_name);
			ret = f.generate(std::get<AST::function_call>(curr_tree));
		} else if(std::holds_alternative<AST::return_ast>(curr_tree)) {
			return_gen r(ast_vec, file_name);
			ret = r.generate(std::get<AST::return_ast>(curr_tree));
		}

		if(ret != GENERATE_SUCCESS || check_variable)
			return ret;
	}
	/*
	 * Ok, we have generated our asm code. Time to put inside our target file
	 */
	std::ofstream o_file(file_name.data());
	o_file << asm_content;
	return GENERATE_SUCCESS;
}	

int code_generator::get_byte_size(const AST::function &func) {
	int bytes{};
	for(auto &i : func.function_body) {
		if(std::holds_alternative<AST::variable>(i)) {
			auto tmp_i = i;
			bytes += std::find_if(lexer::data_sizes.begin(), lexer::data_sizes.end(), [&](auto &pair) {
				return pair.first == std::get<AST::variable>(tmp_i).get_type_str();
					})->second;
		}
	}
	return bytes;
}	

